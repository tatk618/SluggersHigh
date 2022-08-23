#include "System/KdSystem.h"

#include "KdStandardShader.h"


bool KdStandardShader::Init()
{
	//-------------------------------------
	// ���_�V�F�[�_
	//-------------------------------------
	{
		// �R���p�C���ς݂̃V�F�[�_�[�w�b�_�[�t�@�C�����C���N���[�h
		#include "KdStandardShader_VS.inc"

		// ���_�V�F�[�_�[�쐬
		if (FAILED(D3D.GetDev()->CreateVertexShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_VS))) {
			assert(0 && "���_�V�F�[�_�[�쐬���s");
			Release();
			return false;
		}

		// �P���_�̏ڍׂȏ��
		std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,		0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		// ���_���̓��C�A�E�g�쐬
		if (FAILED(D3D.GetDev()->CreateInputLayout(
			&layout[0],			// ���̓G�������g�擪�A�h���X
			layout.size(),		// ���̓G�������g��
			&compiledBuffer[0],				// ���_�o�b�t�@�̃o�C�i���f�[�^
			sizeof(compiledBuffer),			// ��L�̃o�b�t�@�T�C�Y
			&m_inputLayout))					// 
		) {
			assert(0 && "CreateInputLayout���s");
			Release();
			return false;
		}

	}

	//-------------------------------------
	// �s�N�Z���V�F�[�_
	//-------------------------------------
	{
		// �R���p�C���ς݂̃V�F�[�_�[�w�b�_�[�t�@�C�����C���N���[�h
		#include "KdStandardShader_PS.inc"

		if (FAILED(D3D.GetDev()->CreatePixelShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_PS))) {
			assert(0 && "�s�N�Z���V�F�[�_�[�쐬���s");
			Release();
			return false;
		}
	}

	//-------------------------------------
	// �萔�o�b�t�@�쐬
	//-------------------------------------
	m_cb0.Create();
	m_cb1_Material.Create();

	return true;
}

void KdStandardShader::Release()
{
	KdSafeRelease(m_VS);
	KdSafeRelease(m_PS);
	KdSafeRelease(m_inputLayout);
	m_cb0.Release();
	m_cb1_Material.Release();

}

void KdStandardShader::SetToDevice()
{
	// ���_�V�F�[�_���Z�b�g
	D3D.GetDevContext()->VSSetShader(m_VS, 0, 0);
	// ���_���C�A�E�g���Z�b�g
	D3D.GetDevContext()->IASetInputLayout(m_inputLayout);

	// �s�N�Z���V�F�[�_���Z�b�g
	D3D.GetDevContext()->PSSetShader(m_PS, 0, 0);

	// �萔�o�b�t�@���Z�b�g
	D3D.GetDevContext()->VSSetConstantBuffers(0, 1, m_cb0.GetAddress());
	D3D.GetDevContext()->PSSetConstantBuffers(0, 1, m_cb0.GetAddress());

	D3D.GetDevContext()->VSSetConstantBuffers(1, 1, m_cb1_Material.GetAddress());
	D3D.GetDevContext()->PSSetConstantBuffers(1, 1, m_cb1_Material.GetAddress());

}

/*
void KdStandardShader::DrawModel(const KdModel* model)
{
	if (model == nullptr)return;

	auto mesh = model->GetMesh();
	if (mesh == nullptr)return;

	// �萔�o�b�t�@��������
	m_cb0.Write();

	// ���b�V�������Z�b�g
	mesh->SetToDevice();

	// �S�T�u�Z�b�g
	for (UINT subi = 0; subi < mesh->GetSubsets().size(); subi++)
	{
		// �ʂ��P���������ꍇ�̓X�L�b�v
		if (mesh->GetSubsets()[subi].FaceCount == 0)continue;

		// �}�e���A���Z�b�g
		const KdMaterial& material = model->GetMaterials()[ mesh->GetSubsets()[subi].MaterialNo ];

		//-----------------------
		// �}�e���A������萔�o�b�t�@�֏�������
		//-----------------------
		m_cb1_Material.Work().BaseColor = material.BaseColor;
		m_cb1_Material.Work().Emissive = material.Emissive;
		m_cb1_Material.Work().Metallic = material.Metallic;
		m_cb1_Material.Work().Roughness = material.Roughness;
		m_cb1_Material.Write();

		//-----------------------
		// �e�N�X�`���Z�b�g
		//-----------------------

		ID3D11ShaderResourceView* srvs[3] = {};

		// BaseColor
		srvs[0] = material.BaseColorTex->GetSRView();
		// Emissive
		srvs[1] = material.EmissiveTex->GetSRView();
		// Metallic Roughness
		srvs[2] = material.MetallicRoughnessTex->GetSRView();

		// �Z�b�g
		D3D.GetDevContext()->PSSetShaderResources(0, _countof(srvs), srvs);

		//-----------------------
		// �T�u�Z�b�g�`��
		//-----------------------
		mesh->DrawSubset(subi);
	}

}
*/

void KdStandardShader::DrawModel(const KdModel* model, float _alpha,const std::vector<Math::Matrix>* nodeTransforms)
{
	if (model == nullptr)return;

	//if (mesh == nullptr)return;

	// �S�m�[�h���̃��b�V����`��
	for (UINT nodei = 0; nodei < model->GetAllNodes().size(); nodei++)
	{
		// �m�[�h
		const KdModel::Node& node = model->GetAllNodes()[nodei];
		// ���b�V�������݂��Ȃ��ꍇ�̓X�L�b�v
		if (node.Mesh == nullptr)continue;

		// ���b�V�������Z�b�g
		node.Mesh->SetToDevice();

		// �s��
		if (nodeTransforms && nodei < nodeTransforms->size()) {
			m_cb0.Work().mW = (*nodeTransforms)[nodei] * m_mTransform;
		}
		else {
			m_cb0.Work().mW = node.LocalTransform * m_mTransform;
		}

		// �萔�o�b�t�@��������
		m_cb0.Write();

		// �S�T�u�Z�b�g
		//std::shared_ptr<const KdMaterial> material;

		for (UINT subi = 0; subi < node.Mesh->GetSubsets().size(); subi++)
		{
			// �ʂ��P���������ꍇ�̓X�L�b�v
			if (node.Mesh->GetSubsets()[subi].FaceCount == 0)continue;

			// �}�e���A���Z�b�g
			//const KdMaterial& material = materials[mesh->GetSubsets()[subi].MaterialNo];
			// �}�e���A���Z�b�g
			int matNo = -1;
			if (matNo != node.Mesh->GetSubsets()[subi].MaterialNo)
			{
				//material = node.Mesh->GetSubsets()[subi].MaterialData;
				KdMaterial material = model->GetMaterials()[node.Mesh->GetSubsets()[subi].MaterialNo];
				
				//-----------------------
				// �}�e���A������萔�o�b�t�@�֏�������
				//-----------------------
				material.BaseColor.w *= _alpha;
				m_cb1_Material.Work().BaseColor = material.BaseColor;
				m_cb1_Material.Work().Emissive = material.Emissive;
				m_cb1_Material.Work().Metallic = material.Metallic;
				m_cb1_Material.Work().Roughness = material.Roughness;
				m_cb1_Material.Write();

				//-----------------------
				// �e�N�X�`���Z�b�g
				//-----------------------

				ID3D11ShaderResourceView* srvs[3] = {};

				// BaseColor
				srvs[0] = material.BaseColorTex->GetSRView();
				// Emissive
				srvs[1] = material.EmissiveTex->GetSRView();
				// Metallic Roughness
				srvs[2] = material.MetallicRoughnessTex->GetSRView();

				// �Z�b�g
				D3D.GetDevContext()->PSSetShaderResources(0, _countof(srvs), srvs);
			}

			//-----------------------
			// �T�u�Z�b�g�`��
			//-----------------------
			node.Mesh->DrawSubset(subi);
		}

	}
}


