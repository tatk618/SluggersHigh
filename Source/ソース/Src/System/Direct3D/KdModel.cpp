#include "System/KdSystem.h"

#include "KdModel.h"

#include "KdGLTFLoader.h"

//=============================================================
//
// Mesh
//
//=============================================================

void KdMesh::SetToDevice() const
{
	// ���_�o�b�t�@�Z�b�g
	UINT stride = sizeof(KdMeshVertex);	// 1���_�̃T�C�Y
	UINT offset = 0;					// �I�t�Z�b�g
	D3D.GetDevContext()->IASetVertexBuffers(0, 1, m_vb.GetAddress(), &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�Z�b�g
	D3D.GetDevContext()->IASetIndexBuffer(m_ib.GetBuffer(), DXGI_FORMAT_R32_UINT, 0);

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	D3D.GetDevContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool KdMesh::Create(const std::vector<KdMeshVertex>& vertices, const std::vector<KdMeshFace>& faces, const std::vector<KdMeshSubset>& subsets)
{
	Release();

	//------------------------------
	// �T�u�Z�b�g���
	//------------------------------
	m_subsets = subsets;

	//------------------------------
	// ���_�o�b�t�@�쐬
	//------------------------------
	if(vertices.size() > 0)
	{
		// �������ރf�[�^
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &vertices[0];				// �o�b�t�@�ɏ������ޒ��_�z��̐擪�A�h���X
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		// �o�b�t�@�쐬
		if (FAILED(m_vb.Create(D3D11_BIND_VERTEX_BUFFER, sizeof(KdMeshVertex) * vertices.size(), D3D11_USAGE_DEFAULT, &initData)))
		{
			Release();
			return false;
		}

		// ���W�݂̂̔z��
		m_positions.resize(vertices.size());
		for (UINT i = 0; i < m_positions.size(); i++)
		{
			m_positions[i] = vertices[i].Pos;
		}

		// AA���E�f�[�^�쐬
		DirectX::BoundingBox::CreateFromPoints(m_aabb, m_positions.size(), &m_positions[0], sizeof(Math::Vector3));
		// ���E���f�[�^�쐬
		DirectX::BoundingSphere::CreateFromPoints(m_bs, m_positions.size(), &m_positions[0], sizeof(Math::Vector3));
	}	

	//------------------------------
	// �C���f�b�N�X�o�b�t�@�쐬
	//------------------------------
	if(faces.size() > 0)
	{
		// �������ރf�[�^
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &faces[0];				// �o�b�t�@�ɏ������ޒ��_�z��̐擪�A�h���X
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		// �o�b�t�@�쐬
		if (FAILED(m_ib.Create(D3D11_BIND_INDEX_BUFFER, faces.size() * sizeof(KdMeshFace), D3D11_USAGE_DEFAULT, &initData)))
		{
			Release();
			return false;
		}

		// �ʏ��R�s�[
		m_faces = faces;
	}

	return true;
}


void KdMesh::DrawSubset(int subsetNo) const
{
	// �͈͊O�̃T�u�Z�b�g�̓X�L�b�v
	if (subsetNo >= (int)m_subsets.size())return;
	// �ʐ���0�Ȃ�`��X�L�b�v
	if (m_subsets[subsetNo].FaceCount == 0)return;

	// �`��
	D3D.GetDevContext()->DrawIndexed(m_subsets[subsetNo].FaceCount * 3, m_subsets[subsetNo].FaceStart * 3, 0);
}

bool KdModel::Load(const std::string& filename)
{
	Release();

	// ���f���t�@�C���̂���t�H���_���擾
	std::string nowDir = KdGetDirFromPath(filename);
	nowDir += "/";

	//-------------------------------
	// glTF���f���ǂݍ���
	//-------------------------------
	auto model = KdLoadGLTFModel(filename);
	if (model == nullptr)return false;

	// �S�}�e���A��
	m_materials.resize(model->Materials.size());
	for (UINT mti = 0; mti < m_materials.size(); mti++)
	{
		// GLTF�}�e���A���f�[�^
		const auto& gltfMaterial = model->Materials[mti];

		// �}�e���A���f�[�^�Z�b�g
		m_materials[mti].Name = gltfMaterial.Name;

		// ��{�F
		m_materials[mti].BaseColor = gltfMaterial.BaseColor;
		m_materials[mti].BaseColorTex = std::make_shared<KdTexture>();
		if (m_materials[mti].BaseColorTex->Load(nowDir + gltfMaterial.BaseColorTexture) == false)
		{
			m_materials[mti].BaseColorTex = D3D.GetWhiteTex();
		}


		// �������E�e��
		m_materials[mti].Metallic = gltfMaterial.Metallic;
		m_materials[mti].Roughness = gltfMaterial.Roughness;
		m_materials[mti].MetallicRoughnessTex = std::make_shared<KdTexture>();
		if (m_materials[mti].MetallicRoughnessTex->Load(nowDir + gltfMaterial.MetallicRoughnessTexture) == false)
		{
			m_materials[mti].MetallicRoughnessTex = D3D.GetWhiteTex();
		}


		// ���Ȕ���
		m_materials[mti].Emissive = gltfMaterial.Emissive;
		m_materials[mti].EmissiveTex = std::make_shared<KdTexture>();
		if (m_materials[mti].EmissiveTex->Load(nowDir + gltfMaterial.EmissiveTexture) == false)
		{
			m_materials[mti].EmissiveTex = D3D.GetWhiteTex();
		}


		// �@���}�b�v
		m_materials[mti].NormalTex = std::make_shared<KdTexture>();
		if (m_materials[mti].NormalTex->Load(nowDir + gltfMaterial.NormalTexture) == false)
		{

		}

	}

	//-------------------------------
	// �S�m�[�h�쐬
	//-------------------------------
	m_allNodes.resize(model->Nodes.size());

	for (UINT ni = 0; ni < model->Nodes.size(); ni++)
	{
		// glTF�m�[�h�f�[�^
		const auto& srcNode = model->Nodes[ni];

		// �m�[�h��{���
//		std::shared_ptr <KdGLTFNode> newNode = std::make_shared<KdGLTFNode>();
//		m_allNodes.push_back(newNode);

		m_allNodes[ni].Name = srcNode->Name;
		m_allNodes[ni].LocalTransform = srcNode->LocalTransform;
		m_allNodes[ni].Offset = ni;

//		newNode->Name = srcNode->Name;						// ���O
//		newNode->Offset = srcNode->Offset;					// 
//		newNode->LocalTransform = srcNode->LocalTransform;	// �s��
//		newNode->Transform = srcNode->WorldTransform;		// �s��

		// ���b�V��
		if (srcNode->IsMesh)
		{
			m_allNodes[ni].Mesh = std::make_shared<KdMesh>();
			// �쐬
			m_allNodes[ni].Mesh->Create(srcNode->Mesh.Vertices, srcNode->Mesh.Faces, srcNode->Mesh.Subsets);
		}

	}

	/*
	//-------------------
	// �e�q�\�z
	//-------------------
	for (UINT ni = 0; ni < model->Nodes.size(); ni++)
	{
		const auto& srcNode = model->Nodes[ni];

		// �q�o�^
		const auto& srcChildren = srcNode->Children;
		m_allNodes[ni].Children.resize(srcChildren.size());
		for (UINT ci = 0; ci < srcChildren.size(); ci++)
		{
			int childIndex = srcChildren[ci]->Offset;

			m_allNodes[ni]->Children[ci] = m_allNodes[childIndex];
		}
	}
	*/

	//-------------------
	// ���[�g�m�[�h�݂̂̎Q��
	//-------------------
	m_onlyRootNodes.resize(model->RootNodes.size());
	for (UINT ni = 0; ni < m_onlyRootNodes.size(); ni++)
	{
		int idx = model->RootNodes[ni]->Offset;
		m_onlyRootNodes[ni] = m_allNodes[idx];
	}

	//-------------------
	// �A�j���[�V�����f�[�^
	//-------------------
	//m_animations = model->Animations;

	// 
	/*
	for (UINT ni = 0; ni < model->Nodes.size(); ni++)
	{
		// glTF�m�[�h�f�[�^
		const auto& srcNode = model->Nodes[ni];

		// ���b�V��
		if (srcNode->IsMesh)
		{
			m_mesh = std::make_shared<KdMesh>();
			m_mesh->Create(srcNode->Mesh.Vertices, srcNode->Mesh.Faces, srcNode->Mesh.Subsets);
//			break;
		}
	}
	*/
	return true;
}
