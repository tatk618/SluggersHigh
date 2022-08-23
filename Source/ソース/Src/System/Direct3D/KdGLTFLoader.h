#pragma once

//=====================================================
//
// GLTF�`����3D���f���t�@�C����ǂݍ���
//
//=====================================================


//============================
// �}�e���A��
//============================
struct KdGLTFMaterial
{
	std::string		Name;						// �}�e���A����


	std::string		AlphaMode = "OPAQUE";		// "OPAQUE" : �����_�����O���ꂽ�o�͂͊��S�ɕs�����ŁA�A���t�@�l�͖�������܂��B
												// "MASK"   : �����_�����O���ꂽ�o�͂́A�A���t�@�l�Ǝw�肳�ꂽ�A���t�@�J�b�g�I�t�l�ɉ����āA���S�ɕs�����܂��͊��S�ɓ����ɂȂ�܂��B���̃��[�h�́A�؂̗t�⃏�C���[�t�F���X�Ȃǂ̃W�I���g�����V�~�����[�g���邽�߂Ɏg�p����܂��B
												// "BLEND"  : 

	float			AlphaCutoff = 0.5f;			// MASK���[�h���ɁA�J�b�g�I�t��臒l�Ƃ��Ďg�p�@����ȊO�̃��[�h�ł͎g�p����Ȃ�
	bool			DoubleSided = false;		// ���ʂ��H

	//------------------------------
	// PBR�ގ��f�[�^
	//------------------------------
	std::string		BaseColorTexture;			// ��{�F�e�N�X�`���̃t�@�C����
	Math::Vector4	BaseColor = {1,1,1,1};		// ��L�e�N�X�`����RGBA�̃X�P�[�����O�v�f

	// �������A�e��
	std::string		MetallicRoughnessTexture;	// ���^���b�N�ƃ��t�l�X�̃e�N�X�`���@���� = ���^���b�N �ΐ��� = ���t�l�X
	float			Metallic = 1.0f;			// ��L�e�N�X�`���̃��^���b�N�v�f�̏�Z�p�@�e�N�X�`�����������͏�Z�ł͂Ȃ����̂܂܎g�p����
	float			Roughness = 1.0f;			// ��L�e�N�X�`���̃��t�l�X�v�f�̏�Z�p�@�e�N�X�`�����������͏�Z�ł͂Ȃ����̂܂܎g�p����

	// �G�~�b�V�u�F���Ȕ��� �܂�\�ʂ�����o�������@RGB�̂ݎg�p
	std::string		EmissiveTexture;			// �G�~�b�V�u�e�N�X�`���@RGB���g�p
	Math::Vector3	Emissive = {0,0,0};			// ��L�e�N�X�`����RGB�̃X�P�[�����O�v�f

	//------------------------------
	// ���̑��e�N�X�`��
	//------------------------------
	std::string		NormalTexture;				// �@���}�b�v�e�N�X�`��
	std::string		OcclusionTexture;			// ���̎Օ��x�e�N�X�`���@�Ԑ����̂ݎg�p
};

//============================
// �m�[�h �P�̃��b�V����}�e���A���Ȃ�
//============================
struct KdGLTFNode
{
	//---------------------------
	// ��{���
	//---------------------------

	// ���O
	std::string								Name;

	// �I�t�Z�b�g
	int										Offset = 0;

	// �q���X�g
	std::vector<std::shared_ptr<KdGLTFNode>>	Children;

	// �s��
	Math::Matrix							LocalTransform;
	Math::Matrix							WorldTransform;

	//---------------------------
	// Mesh��p���
	//---------------------------
	bool									IsMesh = false;
	struct Mesh
	{
		// ���_�z��
		std::vector<KdMeshVertex>					Vertices;
		// �ʏ��z��
		std::vector<KdMeshFace>					Faces;
		// �T�u�Z�b�g���z��
		std::vector<KdMeshSubset>					Subsets;
	};
	Mesh									Mesh;

};

//============================
// ���f���f�[�^
//============================
struct KdGLTFModel
{
	// �S�m�[�h�f�[�^
	std::vector<std::shared_ptr<KdGLTFNode>>	Nodes;

	// �S�m�[�h���̃��[�g�m�[�h�݂̂̃��X�g
	std::vector<std::shared_ptr<KdGLTFNode>>	RootNodes;

	// �}�e���A���ꗗ
	std::vector<KdGLTFMaterial>					Materials;

	// �A�j���[�V�����f�[�^���X�g
//		std::vector<std::shared_ptr<AnimationData>>		Animations;

};

//===================================================
// GLTF�`����3D���f����ǂݍ���
// Loader��Tinygltf���g�p���Ă��܂��B
// github:https://github.com/syoyo/tinygltf
// 
// �Epath				�c .glfl�t�@�C���̃p�X
//===================================================
std::shared_ptr<KdGLTFModel> KdLoadGLTFModel(const std::string& path);
