#pragma once

//====================================================
//
// �e�N�X�`���N���X
//
//====================================================
class KdTexture
{
public:

	//====================================================
	//
	// �擾
	//
	//====================================================

	// �摜�̃A�X�y�N�g��擾
	float								GetAspectRatio() const { return (float)m_desc.Width / m_desc.Height; }
	// �摜�̕����擾
	UINT								GetWidth() const { return m_desc.Width; }
	// �摜�̍������擾
	UINT								GetHeight() const { return m_desc.Height; }
	// �摜�̑S�����擾
	const D3D11_TEXTURE2D_DESC&			GetInfo() const { return m_desc; }
	// �t�@�C���p�X�擾(Load���̂�)
	const std::string&					GetFilepath() const { return m_filepath; }


	// �摜���\�[�X���擾
	ID3D11Texture2D*					GetResource() const;

	// �V�F�[�_���\�[�X�r���[�擾
	ID3D11ShaderResourceView*			GetSRView() const { return m_srv; }
	// �V�F�[�_���\�[�X�r���[�̃A�h���X�擾
	ID3D11ShaderResourceView* const*	GetSRViewAddress() const { return &m_srv; }

	// �����_�[�^�[�Q�b�g�r���[�擾
	ID3D11RenderTargetView*				GetRTView() const { return m_rtv; }
	// �����_�[�^�[�Q�b�g�r���[�̃A�h���X�擾
	ID3D11RenderTargetView* const*		GetRTViewAddress() const { return &m_rtv; }

	// �[�x�X�e���V���r���[�擾
	ID3D11DepthStencilView*				GetDSView() const { return m_dsv; }

	//====================================================
	//
	// �摜�t�@�C������e�N�X�`���쐬
	//
	//====================================================

	// �摜�t�@�C����ǂݍ���
	// �Efilename		�c �摜�t�@�C����
	// �ErenderTarget	�c �����_�[�^�[�Q�b�g�r���[�𐶐�����(�����_�[�^�[�Q�b�g�ɂ���)
	// �EdepthStencil	�c �[�x�X�e���V���r���[�𐶐�����(Z�o�b�t�@�ɂ���)
	// �EgenerateMipmap	�c �~�b�v�}�b�v��������H
	bool Load(const std::string& filename, bool renderTarget = false, bool depthStencil = false, bool generateMipmap = false);

	//====================================================
	//
	// �e�N�X�`���쐬
	//
	//====================================================

	// ���\�[�X����쐬
	// �EpTexture2D	�c �摜���\�[�X
	// �߂�l�Ftrue �c ����
	bool Create(ID3D11Texture2D* pTexture2D);

	// desc��񂩂�e�N�X�`�����\�[�X���쐬����
	// �Edesc		�c �쐬����e�N�X�`�����\�[�X�̏��
	// �EfillData	�c �o�b�t�@�ɏ������ރf�[�^�@nullptr���Ə������݂Ȃ�
	// �߂�l�Ftrue �c ����
	bool Create(const D3D11_TEXTURE2D_DESC& desc, const D3D11_SUBRESOURCE_DATA* fillData = nullptr);

	// �ʏ�e�N�X�`���Ƃ��č쐬
	// ���e�N�X�`�����\�[�X���쐬���AShaderResourceView�݂̂��쐬���܂�
	// �Ew			�c �摜�̕�(�s�N�Z��)
	// �Eh			�c �摜�̍���(�s�N�Z��)
	// �Eformat		�c �摜�̌`���@DXGI_FORMAT���g�p
	// �EarrayCnt	�c �u�e�N�X�`���z��v���g�p����ꍇ�A���̐��B1�Œʏ��1���e�N�X�`��
	// �EfillData	�c �o�b�t�@�ɏ������ރf�[�^�@nullptr���Ə������݂Ȃ�
	// �߂�l�Ftrue �c ����
	bool Create(int w, int h, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, UINT arrayCnt = 1, const D3D11_SUBRESOURCE_DATA* fillData = nullptr);

	//====================================================
	//
	// �r���[����쐬
	//
	//====================================================
	// ShaderResourceView���Z�b�g����
	void SetSRView(ID3D11ShaderResourceView* srv);

	// 
	KdTexture() {}

	//====================================================
	// ���
	//====================================================
	void Release();

	// 
	~KdTexture()
	{
		Release();
	}

private:

	// �V�F�[�_���\�[�X�r���[(�ǂݎ��p)
	ID3D11ShaderResourceView*	m_srv = nullptr;
	// �����_�[�^�[�Q�b�g�r���[(�������ݗp)
	ID3D11RenderTargetView*		m_rtv = nullptr;
	// �[�x�X�e���V���r���[(Z�o�b�t�@�p)
	ID3D11DepthStencilView*		m_dsv = nullptr;

	// �摜���
	D3D11_TEXTURE2D_DESC		m_desc = {};

	// �摜�t�@�C����(Load����p)
	std::string					m_filepath;

private:
	// �R�s�[�֎~�p
	KdTexture(const KdTexture& src) = delete;
	void operator=(const KdTexture& src) = delete;
};
