#pragma once

//==========================================================
// ���b�V���p ���_���
//==========================================================
struct KdMeshVertex
{
	Math::Vector3	Pos;						// ���W
	Math::Vector2	UV;							// UV
	Math::Vector3	Normal;						// �@��
	unsigned int	Color = 0xFFFFFFFF;			// RGBA�F(�e�ʍ팸�̂��߁A�e�F0�`255��UINT�^)
};

//==========================================================
// ���b�V���p �ʏ��
//==========================================================
struct KdMeshFace
{
	UINT Idx[3];				// �O�p�`���\�����钸�_��Index
};

//==========================================================
// ���b�V���p �T�u�Z�b�g���
//==========================================================
struct KdMeshSubset
{
	UINT		MaterialNo = 0;		// �}�e���A��No

	UINT		FaceStart = 0;		// ��Index�@���̃}�e���A���Ŏg�p����Ă���ŏ��̖ʂ�Index
	UINT		FaceCount = 0;		// �ʐ��@FaceStart����A�����̖ʂ��g�p����Ă��邩��
};

//==========================================================
//
// ���b�V���N���X
//
//==========================================================
class KdMesh
{
public:

	//=================================================
	// �擾�E�ݒ�
	//=================================================

	// �T�u�Z�b�g���z����擾
	const std::vector<KdMeshSubset>&	GetSubsets() const { return m_subsets; }

	// ���_�̍��W�z����擾
	const std::vector<Math::Vector3>&	GetVertexPositions() const { return m_positions; }
	// �ʂ̔z����擾
	const std::vector<KdMeshFace>&		GetFaces() const { return m_faces; }

	// �����s���E�{�b�N�X�擾
	const DirectX::BoundingBox&			GetBoundingBox() const { return m_aabb; }
	// ���E���擾
	const DirectX::BoundingSphere&		GetBoundingSphere() const { return m_bs; }

	// ���b�V���f�[�^���f�o�C�X�փZ�b�g����
	void SetToDevice() const;


	//=================================================
	// �쐬�E���
	//=================================================

	// ���b�V���쐬
	// �Evertices		�c ���_�z��
	// �Efaces			�c �ʃC���f�b�N�X���z��
	// �Esubsets		�c �T�u�Z�b�g���z��
	// �߂�l			�c �����Ftrue
	bool Create(const std::vector<KdMeshVertex>& vertices, const std::vector<KdMeshFace>& faces, const std::vector<KdMeshSubset>& subsets);

	// ���
	void Release()
	{
		m_vb.Release();
		m_ib.Release();
		m_subsets.clear();
		m_positions.clear();
		m_faces.clear();
	}

	~KdMesh()
	{
		Release();
	}

	//=================================================
	// ����
	//=================================================

	// �w��T�u�Z�b�g��`��
	void DrawSubset(int subsetNo) const;

	// 
	KdMesh() {}

private:

	// ���_�o�b�t�@
	KdBuffer						m_vb;
	// �C���f�b�N�X�o�b�t�@
	KdBuffer						m_ib;

	// �T�u�Z�b�g���
	std::vector<KdMeshSubset>		m_subsets;

	// ���E�f�[�^
	DirectX::BoundingBox			m_aabb;	// �����s���E�{�b�N�X
	DirectX::BoundingSphere			m_bs;	// ���E��

	// ���W�݂̂̔z��(����)
	std::vector<Math::Vector3>		m_positions;
	// �ʏ��݂̂̔z��(����)
	std::vector<KdMeshFace>			m_faces;

private:
	// �R�s�[�֎~�p
	KdMesh(const KdMesh& src) = delete;
	void operator=(const KdMesh& src) = delete;
};

//==========================================================
// �}�e���A��(glTF�x�[�X��PBR�}�e���A��)
//==========================================================
struct KdMaterial
{
	//---------------------------------------
	// �ގ��f�[�^
	//---------------------------------------

	// ���O
	std::string					Name;

	// ��{�F
	std::shared_ptr<KdTexture>	BaseColorTex;				// ��{�F�e�N�X�`��
	Math::Vector4				BaseColor = { 1,1,1,1 };	// ��{�F�̃X�P�[�����O�W��(RGBA)

	// �������A�e��
	std::shared_ptr<KdTexture>	MetallicRoughnessTex;		// B:������ G:�e��
	float						Metallic = 0.0f;			// �������̃X�P�[�����O�W��
	float						Roughness = 1.0f;			// �e���̃X�P�[�����O�W��

	// ���Ȕ���
	std::shared_ptr<KdTexture>	EmissiveTex;				// ���Ȕ����e�N�X�`��
	Math::Vector3				Emissive = { 0,0,0 };		// ���Ȕ����̃X�P�[�����O�W��(RGB)

	// �@���}�b�v
	std::shared_ptr<KdTexture>	NormalTex;

	KdMaterial()
	{
		BaseColorTex			= D3D.GetWhiteTex();
		MetallicRoughnessTex	= D3D.GetWhiteTex();
		EmissiveTex				= D3D.GetWhiteTex();
	}
};

//============================
// �A�j���[�V����
//============================
struct KdAnimationData
{
	// �A�j���[�V������
	std::string				Name;
	// �A�j���̒���
	float					MaxLength = 0;

	// �N�H�[�^�j�I���L�[
	struct KeyQuaternion {
		float				Time = 0;		// ����
		Math::Quaternion	Quat;			// �N�H�[�^�j�I���f�[�^

		Math::Quaternion	PrevTangent;	// CUBICSPLINE��ԂȂǂŎg�p
		Math::Quaternion	NextTangent;	// CUBICSPLINE��ԂȂǂŎg�p
	};
	// �x�N�g���L�[
	struct KeyVector3 {
		float				Time = 0;		// ����
		Math::Vector3		Vec;			// 3D�x�N�g���f�[�^

		Math::Vector3		PrevTangent;	// CUBICSPLINE��ԂȂǂŎg�p
		Math::Vector3		NextTangent;	// CUBICSPLINE��ԂȂǂŎg�p
	};
	// �P�m�[�h�̃A�j���[�V�����f�[�^
	struct Node
	{
		int							NodeOffset = -1;	// �Ώۃm�[�h��Offset

		// �e�`�����l��
		std::vector<KeyQuaternion>	Rotations;		// ��]�L�[���X�g
		std::vector<KeyVector3>		Scales;			// �g��L�[���X�g
		std::vector<KeyVector3>		Translations;	// �ʒu�L�[���X�g
	};

	// �A�j���[�V�����f�[�^
	std::vector<std::shared_ptr<Node>>		Nodes;

};

class KdModel
{
public:
	struct Node;
	using kdGLTFNode = std::shared_ptr<Node>;

	bool Load(const std::string& filename);

	//==========================================================
	// �擾
	//==========================================================

	// �S�m�[�h�z��擾
	const std::vector<Node>& GetAllNodes() const { return m_allNodes; }
	// �S�m�[�h���̃��[�g�m�[�h�݂̂̎Q�Ɣz��擾
	const std::vector<Node>& GetOnlyRootNodes() const { return m_onlyRootNodes; }
	// �}�e���A���z��擾
	const std::vector<KdMaterial>& GetMaterials() const { return m_materials; }
	// �A�j���[�V�����f�[�^���X�g
	//const std::vector<KdAnimationData>& GetAnimations() const { return m_animations; }


	void Release()
	{
//		m_mesh = nullptr;
		m_materials.clear();
		m_allNodes.clear();
		m_onlyRootNodes.clear();
	}

	struct Node
	{
		// �m�[�h��
		std::string				Name;
		// �I�t�Z�b�g
		int						Offset = -1;
		// ���W
		Math::Matrix			LocalTransform;
		// ���b�V��
		std::shared_ptr <KdMesh>	Mesh;
	};

private:
//	std::shared_ptr <KdMesh>	m_mesh;
	// �S�m�[�h�z��
	std::vector<Node>				m_allNodes;
	// m_allNodes���A���[�g�m�[�h�݂̂̎Q��
	std::vector<Node>				m_onlyRootNodes;

	// �}�e���A���z��
	std::vector<KdMaterial>			m_materials;

	// �A�j���[�V�����f�[�^���X�g
//	std::vector<KdAnimationData>	m_animations;
};
