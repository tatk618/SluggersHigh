#pragma once

//===========================================
//
// �֗��@�\
//
//===========================================

// ComPtr�Z�k��
using Microsoft::WRL::ComPtr;

// �Z�p�n�Z�k��
namespace Math = DirectX::SimpleMath;

// �p�x�ϊ�
inline constexpr float KdToRadians(float fDegrees) { return fDegrees * (3.141592654f / 180.0f); }
inline constexpr float KdToDegrees(float fRadians) { return fRadians * (180.0f / 3.141592654f); }

// ���S��Release���邽�߂̊֐�
template<class T>
void KdSafeRelease(T*& p)
{
	if (p)
	{
		p->Release();
		p = nullptr;
	}
}

// ���S��Delete���邽�߂̊֐�
template<class T>
void KdSafeDelete(T*& p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}

// �t�@�C���p�X����A�e�f�B���N�g���܂ł̃p�X���擾
inline std::string KdGetDirFromPath(const std::string &path)
{
	const std::string::size_type pos = std::max<signed>(path.find_last_of('/'), path.find_last_of('\\'));
	return (pos == std::string::npos) ? std::string() : path.substr(0, pos + 1);
}

//=====================================================
//
// �e�N�X�`��
//
//=====================================================

// �摜�t�@�C����ǂݍ��݁A���\�[�X�f�[�^��Ԃ�
// �Efilepath		�c �摜�t�@�C����
// �EbindFlags		�c ���\�[�X�o�C���h�t���O
// �EgenerateMipmap	�c �~�b�v�}�b�v��������H
// �Eusage			�c �g�p�@
// �EcpuAccessFlags	�c CPU����A�N�Z�X�\�ɂ��邩 �����Ӂ� �g�p�\����usage�̐ݒ�ɂ��
// �EmiscFlags		�c ���̑��t���O
// �߂�l�F�����c2D�摜���\�[�X�̃A�h���X�@���s�cnullptr
ID3D11Texture2D* KdLoadTexture2D(const std::string& filepath, UINT bindFlags = D3D11_BIND_SHADER_RESOURCE, bool generateMipmap = false, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT cpuAccessFlags = 0, UINT miscFlags = 0);

// 2D�摜(resource)���\�[�X����A�œK�ȃr���[���쐬����
// �Eresource		�c 2D�摜���\�[�X
// �EppSRV			�c �쐬���ꂽShaderResourceView���󂯎�邽�߂̕ϐ��̃A�h���X
// �EppRTV			�c �쐬���ꂽRenderTargetView���󂯎�邽�߂̕ϐ��̃A�h���X
// �EppDSV			�c �쐬���ꂽDepthStencilView���󂯎�邽�߂̕ϐ��̃A�h���X
bool KdCreateViewsFromTexture2D(ID3D11Texture2D* resource, ID3D11ShaderResourceView** ppSRV, ID3D11RenderTargetView** ppRTV, ID3D11DepthStencilView** ppDSV);

// view����摜�����擾����
void KdGetTextureInfo(ID3D11View* view, D3D11_TEXTURE2D_DESC& outDesc);
