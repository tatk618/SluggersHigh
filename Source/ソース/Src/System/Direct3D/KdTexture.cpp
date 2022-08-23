#include "System/KdSystem.h"

#include "KdTexture.h"



// ���\�[�X���擾

ID3D11Texture2D* KdTexture::GetResource() const
{
	if (m_srv == nullptr)return nullptr;

	ID3D11Resource* res;
	m_srv->GetResource(&res);

	ID3D11Texture2D* tex2D;
	if (FAILED(res->QueryInterface<ID3D11Texture2D>(&tex2D)))
	{
		res->Release();
		return nullptr;
	}
	res->Release();
	tex2D->Release();

	return tex2D;
}

bool KdTexture::Load(const std::string & filename, bool renderTarget, bool depthStencil, bool generateMipmap)
{
	Release();
	if (filename.empty())return false;

	//------------------------------------
	// �摜�ǂݍ���
	//------------------------------------

	// Bind Flags
	UINT bindFlags = 0;
	bindFlags |= D3D11_BIND_SHADER_RESOURCE;
	if (renderTarget)bindFlags |= D3D11_BIND_RENDER_TARGET;
	if (depthStencil)bindFlags |= D3D11_BIND_DEPTH_STENCIL;


	ID3D11Texture2D* resource;
	resource = KdLoadTexture2D(filename, bindFlags, generateMipmap, D3D11_USAGE_DEFAULT, 0, 0);
	if (resource == nullptr)
	{
		Release();
		return false;
	}

	//------------------------------------
	// �e�N�X�`�����\�[�X(m_resource)����A�e�r���[���쐬����
	//------------------------------------
	if (KdCreateViewsFromTexture2D(resource, &m_srv, &m_rtv, &m_dsv) == false)
	{
		resource->Release();
		Release();
		return false;
	}

	// �摜���擾
	resource->GetDesc(&m_desc);
	resource->Release();

	m_filepath = filename;

	return true;
}

bool KdTexture::Create(ID3D11Texture2D* pTexture2D)
{
	Release();
	if (pTexture2D == nullptr)return false;

	//---------------------------------------------
	// �摜���\�[�X����A�e�r���[���쐬����
	//---------------------------------------------
	if (KdCreateViewsFromTexture2D(pTexture2D, &m_srv, &m_rtv, &m_dsv) == false) {
		Release();
		return false;
	}

	// �t�@�C�����L��
	m_filepath = "";

	// �摜���擾
	pTexture2D->GetDesc(&m_desc);

	return true;

}

bool KdTexture::Create(const D3D11_TEXTURE2D_DESC & desc, const D3D11_SUBRESOURCE_DATA * fillData)
{
	Release();

	//--------------------------------------------
	// 2D�e�N�X�`���̐���
	//--------------------------------------------
	ID3D11Texture2D* resource;
	HRESULT hr = D3D.GetDev()->CreateTexture2D(&desc, fillData, &resource);
	if (FAILED(hr)) {
		Release();
		return false;
	}

	//--------------------------------------------
	// m_pTexture2D����A�e�r���[���쐬����
	//--------------------------------------------
	if (KdCreateViewsFromTexture2D(resource, &m_srv, &m_rtv, &m_dsv) == false)
	{
		Release();
		return false;
	}

	// �摜���擾
	resource->GetDesc(&m_desc);
	resource->Release();

	return true;
}

bool KdTexture::Create(int w, int h, DXGI_FORMAT format, UINT arrayCnt, const D3D11_SUBRESOURCE_DATA * fillData)
{
	Release();

	// �쐬����2D�e�N�X�`���ݒ�
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.Format = format;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.Width = (UINT)w;
	desc.Height = (UINT)h;
	desc.CPUAccessFlags = 0;
	desc.MipLevels = 1;
	desc.ArraySize = arrayCnt;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	// �쐬
	if (Create(desc, fillData) == false)return false;

	return true;
}

void KdTexture::SetSRView(ID3D11ShaderResourceView* srv)
{
	if (srv == nullptr)return;

	Release();
	m_srv = srv;	// �Z�b�g
	m_srv->AddRef();// �Q�ƃJ�E���^�𑝂₵�Ă���

	// �摜���擾
	GetResource()->GetDesc(&m_desc);
}

void KdTexture::Release()
{
	KdSafeRelease(m_srv);
	KdSafeRelease(m_rtv);
	KdSafeRelease(m_dsv);

	m_filepath = "";
}
