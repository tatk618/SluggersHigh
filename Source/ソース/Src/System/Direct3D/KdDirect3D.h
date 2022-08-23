#pragma once

class KdTexture;

// �T���v���X�e�[�g
enum class KdSamplerFilterMode {
	Point,			// �|�C���g�t�B���^
	Linear,			// ���`�t�B���^
	Anisotropic,	// �ٕ����t�B���^
};

enum class KdSamplerAddressingMode {
	Wrap,			// ���b�v���[�h
	Clamp,			// �N�����v���[�h
};

// �u�����h���[�h�p�萔
enum class KdBlendMode {
	NoBlend,		// �u�����h�����Ȃ�
	Alpha,			// �������u�����h
	Add				// ���Z�u�����h
};

//=======================================================================================
//
// Direct3D�N���X
//
//	Direct3D11�ɕK�v�ȕϐ���A�悭�s��������܂Ƃ߂Ă��܂��B
//
//
//=======================================================================================
class KdDirect3D {
public:

	//==============================================================
	//
	// �擾
	//
	//==============================================================

	// Direct3D�f�o�C�X�擾
	ID3D11Device*				GetDev() { return m_pDevice; }

	// Direct3D�f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext*		GetDevContext() { return m_pDeviceContext; }

	// SwapChain�擾
	IDXGISwapChain*				GetSwapChain() { return m_pGISwapChain; }

	// �o�b�N�o�b�t�@�擾
	std::shared_ptr<KdTexture>	GetBackBuffer() { return m_backBuffer; }

	// Z�o�b�t�@�擾
	std::shared_ptr<KdTexture>	GetZBuffer() { return m_zBuffer; }

	// �r���[�|�[�g�擾
	void						GetViewport(Math::Viewport& out) const;

	// 1x1 ���e�N�X�`���擾
	std::shared_ptr<KdTexture>	GetWhiteTex() const { return m_texWhite; }

	//==============================================================
	//
	// �������E���
	//
	//==============================================================

	// ������
	// �EhWnd			�c �E�B���h�E�̃n���h��
	// �Ew				�c X�𑜓x
	// �Eh				�c Y�𑜓x
	// �EdebugDevice	�c �f�o�b�O���[�h
	// �EerrMsg			�c (�o��)�G���[���b�Z�[�W�������Ă���
	bool Init(HWND hWnd, int w, int h, bool deviceDebug, std::string& errMsg);

	// ���
	void Release();

	//==============================================================
	//
	// �p�C�v���C���X�e�[�g�쐬�֐�
	//
	//==============================================================

	// �[�x�X�e���V���X�e�[�g�쐬
	// �Edevice				�c D3D�f�o�C�X
	// �EzEnable			�c �[�x�e�X�g�L��
	// �EzWriteEnable		�c �[�x�������ݗL��
	ID3D11DepthStencilState* CreateDepthStencilState(bool zEnable, bool zWriteEnable) const;

	// ���X�^���C�U�[�X�e�[�g�쐬
	// �Edevice				�c D3D�f�o�C�X
	// �EcullMode			�c �J�����O���[�h
	// �EfillMode			�c �O�p�`�̕`�惂�[�h
	// �EdepthClipEnable	�c Z�N���b�s���O��L���ɂ���
	// �EscissorEnable		�c �؂蔲���͈͂�L���ɂ���
	ID3D11RasterizerState* CreateRasterizerState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, bool depthClipEnable, bool scissorEnable) const;

	// �T���v���[�X�e�[�g�쐬
	// �Edevice				�c D3D�f�o�C�X
	// �EfilterType			�c 0:��ԂȂ� 1:���`�t�B���^ 2:�ٕ����t�B���^
	// �EmaxAnisotropy		�c �ٕ����t�B���^���̍ő�l�@2, 4, 6, 8, 10, 12, 14, 16 �̂����ꂩ
	// �EaddressingMode		�c �e�N�X�`���A�h���b�V���O���[�h 0:Wrap 1:Clamp
	// �EcomparisonModel	�c ��r���[�hON�@�V���h�E�}�b�s���O�ȂǂŎg�p����
	ID3D11SamplerState* CreateSamplerState(KdSamplerFilterMode filterType, UINT maxAnisotropy, KdSamplerAddressingMode addressingMode, bool comparisonModel) const;

	// �u�����h�X�e�[�g�쐬
	// �Edevice				�c D3D�f�o�C�X
	// �Emode				�c �������[�h
	ID3D11BlendState* CreateBlendState(KdBlendMode mode) const;

	//==============================================================

	// ���_��`�悷��ȈՓI�Ȋ֐�
	// �Etopology		�c ���_���ǂ̂悤�Ȍ`��ŕ`�悷�邩�@D3D_PRIMITIVE_TOPOLOGY�}�N�����g�p
	// �EvertexCount	�c ���_��
	// �EpVertexStream	�c ���_�z��̐擪�A�h���X
	// �Estride			�c �P���_�̃o�C�g�T�C�Y
	void DrawVertices(D3D_PRIMITIVE_TOPOLOGY topology, int vertexCount, const void* pVertexStream, UINT stride);

	//==============================================================

	// 
	~KdDirect3D() {
		Release();
	}

private:

	ID3D11Device*				m_pDevice = nullptr;			// Direct3D11�̒��S�ɂȂ�N���X�B�S�̂̊Ǘ��ƃo�b�t�@�A�V�F�[�_�A�e�N�X�`���Ȃǂ̃��\�[�X�쐬�Ȃǂ��s���BD3D9�Ƃ͈���āA���̃N���X�͕`��֌W�̃����o�֐��������Ȃ��B
	ID3D11DeviceContext*		m_pDeviceContext = nullptr;		// �`�揈�����s���N���X�B
																// �����I�ɂ́A�����_�����O�R�}���h�ƌĂ΂��o�C�i���f�[�^���쐬���AGPU�ɑ���BGPU�������_�����O�R�}���h����͂��邱�ƂŎ��ۂ̕`�揈�����s����B

	IDXGIAdapter*				m_pGIAdapter = nullptr;			// �f�B�X�v���C �T�u�V�X�e��(1�܂��͕�����GPU�ADAC�A����уr�f�I �������[)�B
	IDXGIFactory*				m_pGIFactory = nullptr;			// �t���X�N���[���؂�ւ��ȂǂŎg�p�B
	IDXGISwapChain*				m_pGISwapChain = nullptr;		// �E�C���h�E�ւ̕\����_�u���o�b�t�@�����O�Ȃǂ��s���N���X�B�}���`�T���v�����O�A���t���b�V�����[�g�Ȃǂ̐ݒ���ł���݂����B

	// �o�b�t�@
	std::shared_ptr<KdTexture>	m_backBuffer;					// �o�b�N�o�b�t�@
	std::shared_ptr<KdTexture>	m_zBuffer;						// Z�o�b�t�@

	// DrawVertices�p ���_�o�b�t�@
	KdBuffer					m_tempFixedVertexBuffer[10];	// �Œ蒷 ���_�o�b�t�@
	KdBuffer					m_tempVertexBuffer;				// �ϒ� ���_�o�b�t�@

	//------------------------
	// �֗��e�N�X�`��
	//------------------------
	// 1x1 ���e�N�X�`��
	std::shared_ptr<KdTexture>	m_texWhite;


//-------------------------------
// �V���O���g��
//-------------------------------
private:

	KdDirect3D() {
	}

public:
	static KdDirect3D &GetInstance() {
		static KdDirect3D instance;
		return instance;
	}
};

// �ȒP�ɃA�N�Z�X�ł���悤�ɂ��邽�߂̃}�N��
#define D3D KdDirect3D::GetInstance()
