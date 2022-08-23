#include "System/KdSystem.h"

#include "KdUtility.h"

ID3D11Texture2D* KdLoadTexture2D(const std::string& filepath, UINT bindFlags, bool generateMipmap, D3D11_USAGE usage, UINT cpuAccessFlags, UINT miscFlags)
{
	// �t�@�C������WideChar�֕ϊ�
	std::wstring wFilename = sjis_to_wide(filepath);

	// ��DirectX Tex���C�u�������g�p���ĉ摜��ǂݍ���

	DirectX::TexMetadata meta;
	DirectX::ScratchImage image;

	bool bLoaded = false;

	// WIC�摜�ǂݍ���
	//  WIC_FLAGS_ALL_FRAMES �c gif�A�j���Ȃǂ̕����t���[����ǂݍ���ł����
	if (SUCCEEDED(DirectX::LoadFromWICFile(wFilename.c_str(), DirectX::WIC_FLAGS_ALL_FRAMES, &meta, image)))
	{
		bLoaded = true;
	}

	// DDS�摜�ǂݍ���
	if (bLoaded == false) {
		if (SUCCEEDED(DirectX::LoadFromDDSFile(wFilename.c_str(), DirectX::DDS_FLAGS_NONE, &meta, image)))
		{
			bLoaded = true;
		}
	}

	// TGA�摜�ǂݍ���
	if (bLoaded == false) {
		if (SUCCEEDED(DirectX::LoadFromTGAFile(wFilename.c_str(), &meta, image)))
		{
			bLoaded = true;
		}
	}

	// HDR�摜�ǂݍ���
	if (bLoaded == false) {
		if (SUCCEEDED(DirectX::LoadFromHDRFile(wFilename.c_str(), &meta, image)))
		{
			bLoaded = true;
		}
	}

	// �ǂݍ��ݎ��s
	if (bLoaded == false)
	{
		return nullptr;
	}

	// �~�b�v�}�b�v����
	if (meta.mipLevels == 1 && generateMipmap)
	{
		DirectX::ScratchImage mipChain;
		if (SUCCEEDED(DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_DEFAULT, 0, mipChain)))
		{
			image.Release();
			image = std::move(mipChain);
		}
	}

	//------------------------------------
	// �e�N�X�`�����\�[�X�쐬
	//------------------------------------
	ID3D11Texture2D* tex2D = nullptr;
	if (FAILED(DirectX::CreateTextureEx(
		D3D.GetDev(),						// Direct3D Device
		image.GetImages(),
		image.GetImageCount(),
		image.GetMetadata(),
		usage,								// Usage
		bindFlags,							// Bind Flags
		cpuAccessFlags,						// CPU Access Flags
		miscFlags,							// MiscFlags
		false,								// ForceSRGB
		(ID3D11Resource**)&tex2D)
	)) {
		return nullptr;
	}

	return tex2D;
}

bool KdCreateViewsFromTexture2D(ID3D11Texture2D* resource, ID3D11ShaderResourceView** ppSRV, ID3D11RenderTargetView** ppRTV, ID3D11DepthStencilView** ppDSV)
{
	// ���\�[�X������
	if (resource == nullptr)return false;

	// �e�N�X�`���{�̂̏��擾
	D3D11_TEXTURE2D_DESC desc;
	resource->GetDesc(&desc);

	//===========================================================
	//
	// RenderTargetView���쐬����
	//
	//===========================================================

	// �����_�[�^�[�Q�b�g�t���O�����Ă鎞�̂�
	if (ppRTV && desc.BindFlags & D3D11_BIND_RENDER_TARGET)
	{
		// �쐬����r���[�̐ݒ�
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = desc.Format;	// Format
		// �P�i�̃e�N�X�`��(�ʏ�e�N�X�`��)�̏ꍇ
		if (desc.ArraySize == 1) {
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;			// �P�i�e�N�X�`��
		}
		// �e�N�X�`���z��̏ꍇ
		else {
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;		// �e�N�X�`���z��
			rtvDesc.Texture2DArray.ArraySize = desc.ArraySize;			// �v�f��
			rtvDesc.Texture2DArray.FirstArraySlice = 0;
			rtvDesc.Texture2DArray.MipSlice = 0;
		}

		// �����_�[�^�[�Q�b�g�r���[�쐬
		HRESULT hr = D3D.GetDev()->CreateRenderTargetView(resource, &rtvDesc, ppRTV);
		if (FAILED(hr))
		{
			assert(0 && "RenderTargetView�̍쐬�Ɏ��s");
			return false;
		}
	}

	//===========================================================
	//
	// ShaderResourceView�̏����쐬����
	//
	//===========================================================
	// �V�F�[�_���\�[�X�r���[�t���O�����Ă鎞�̂�
	if (ppSRV && desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
	{
		// �쐬����r���[�̐ݒ�
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

		// �e�N�X�`����Z�o�b�t�@�̏ꍇ�́A�œK�ȃt�H�[�}�b�g�ɂ���
		if (desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
		{
			switch (desc.Format) {
				// 16�r�b�g
			case DXGI_FORMAT_R16_TYPELESS:
				srvDesc.Format = DXGI_FORMAT_R16_UNORM;
				break;
				// 32�r�b�g
			case DXGI_FORMAT_R32_TYPELESS:
				srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
				break;
				// 24�r�b�g(Z�o�b�t�@) + 8�r�b�g(�X�e���V���o�b�t�@) 
			case DXGI_FORMAT_R24G8_TYPELESS:
				srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
				break;
			default:
				assert(0 && "[ShaderResource] �Ή����Ă��Ȃ��t�H�[�}�b�g�ł�");
				break;
			}
		}
		// Z�o�b�t�@�łȂ��ꍇ�́A���̂܂ܓ����t�H�[�}�b�g���g�p
		else
		{
			srvDesc.Format = desc.Format;
		}

		// �P�i�̃e�N�X�`��(�ʏ�e�N�X�`��)�̏ꍇ
		if (desc.ArraySize == 1)
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = desc.MipLevels;
			if (srvDesc.Texture2D.MipLevels <= 0)srvDesc.Texture2D.MipLevels = -1;
		}
		// �e�N�X�`���z��̏ꍇ
		else {
			// ����ɃL���[�u�}�b�v�̏ꍇ
			if (desc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE) {
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			}
			// �ʏ�e�N�X�`���z��
			else {
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			}
			srvDesc.Texture2DArray.MostDetailedMip = 0;
			srvDesc.Texture2DArray.MipLevels = desc.MipLevels;
			srvDesc.Texture2DArray.ArraySize = desc.ArraySize;	// �v�f��
			srvDesc.Texture2DArray.FirstArraySlice = 0;
		}

		// �V�F�[�_���\�[�X�r���[�쐬
		HRESULT hr = D3D.GetDev()->CreateShaderResourceView(resource, &srvDesc, ppSRV);
		if (FAILED(hr))
		{
			assert(0 && "ShaderResourceView�̍쐬�Ɏ��s");
			return false;
		}
	}

	//===========================================================
	//
	// DepthStencilView���쐬����
	//
	//===========================================================
	// Z�o�b�t�@�t���O�����Ă鎞�̂�
	if (ppDSV && desc.BindFlags & D3D11_BIND_DEPTH_STENCIL) {
		// �쐬����r���[�̐ݒ�
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

		// �e�N�X�`���[�쐬���Ɏw�肵���t�H�[�}�b�g�ƌ݊���������A�[�x�X�e���V���r���[�Ƃ��Ďw��ł���t�H�[�}�b�g���w�肷��
		switch (desc.Format) {
			// 16�r�b�g
		case DXGI_FORMAT_R16_TYPELESS:
			dsvDesc.Format = DXGI_FORMAT_D16_UNORM;
			break;
			// 32�r�b�g
		case DXGI_FORMAT_R32_TYPELESS:
			dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
			break;
			// 24�r�b�g(Z�o�b�t�@) + 8�r�b�g(�X�e���V���o�b�t�@) 
		case DXGI_FORMAT_R24G8_TYPELESS:
			dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			break;
		default:
			assert(0 && "[DepthStencil] �Ή����Ă��Ȃ��t�H�[�}�b�g�ł�");
			break;
		}

		// �P�i�̃e�N�X�`��(�ʏ�e�N�X�`��)�̏ꍇ
		if (desc.ArraySize == 1) {
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsvDesc.Texture2D.MipSlice = 0;
		}
		// �e�N�X�`���z��̏ꍇ
		else {
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
			dsvDesc.Texture2DArray.ArraySize = desc.ArraySize;
			dsvDesc.Texture2DArray.MipSlice = 0;
			dsvDesc.Texture2DArray.FirstArraySlice = 0;
		}

		//-------------------------------
		// �f�v�X�X�e���V���r���[�쐬
		//-------------------------------
		HRESULT hr = D3D.GetDev()->CreateDepthStencilView(resource, &dsvDesc, ppDSV);
		if (FAILED(hr)) {
			assert(0 && "DepthStencilView�̍쐬�Ɏ��s");
			return false;
		}
	}

	return true;
}

// view����摜�����擾����
void KdGetTextureInfo(ID3D11View* view, D3D11_TEXTURE2D_DESC& outDesc)
{
	outDesc = {};

	ID3D11Resource* res;
	view->GetResource(&res);

	ID3D11Texture2D* tex2D;
	if (SUCCEEDED(res->QueryInterface<ID3D11Texture2D>(&tex2D)))
	{
		tex2D->GetDesc(&outDesc);
		tex2D->Release();
	}
	res->Release();
}
