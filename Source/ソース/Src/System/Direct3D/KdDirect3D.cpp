#include "System/KdSystem.h"

void KdDirect3D::GetViewport(Math::Viewport & out) const
{
	UINT numVPs = 1;
	D3D11_VIEWPORT vp;
	m_pDeviceContext->RSGetViewports(&numVPs, &vp);
	out = vp;
}

bool KdDirect3D::Init(HWND hWnd, int w, int h, bool deviceDebug, std::string& errMsg)
{
	HRESULT hr;

	//=====================================================
	// �t�@�N�g���[�쐬(�r�f�I �O���t�B�b�N�̐ݒ�̗񋓂�w��Ɏg�p�����I�u�W�F�N�g)
	//=====================================================
	hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&m_pGIFactory));
	if (FAILED(hr)) {
		errMsg = "�t�@�N�g���[�쐬���s";

		Release();
		return false;
	}

	//=====================================================
	//�f�o�C�X����(��Ƀ��\�[�X�쐬���Ɏg�p����I�u�W�F�N�g)
	//=====================================================
	UINT creationFlags = 0;

	if (deviceDebug) {
		// Direct3D�̃f�o�b�O��L���ɂ���(�������d�����ׂ����G���[���킩��)
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}

	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_1,	// Direct3D 11.1  ShaderModel 5
		D3D_FEATURE_LEVEL_11_0,	// Direct3D 11    ShaderModel 5
		D3D_FEATURE_LEVEL_10_1,	// Direct3D 10.1  ShaderModel 4
		D3D_FEATURE_LEVEL_10_0,	// Direct3D 10.0  ShaderModel 4
		D3D_FEATURE_LEVEL_9_3,	// Direct3D 9.3   ShaderModel 3
		D3D_FEATURE_LEVEL_9_2,	// Direct3D 9.2   ShaderModel 3
		D3D_FEATURE_LEVEL_9_1,	// Direct3D 9.1   ShaderModel 3
	};

	// �f�o�C�X�ƂŃf�o�C�X�R���e�L�X�g���쐬
	D3D_FEATURE_LEVEL futureLevel;
	hr = D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				creationFlags,
				featureLevels,
				_countof(featureLevels),
				D3D11_SDK_VERSION,
				&m_pDevice,
				&futureLevel,
				&m_pDeviceContext);
	if FAILED( hr )
	{
		errMsg = "Direct3D11�f�o�C�X�쐬���s";

		Release();
		return false;
	}

	//=====================================================
	// �X���b�v�`�F�C���쐬(�t�����g�o�b�t�@�ɕ\���\�ȃo�b�N�o�b�t�@��������)
	//=====================================================
	DXGI_SWAP_CHAIN_DESC DXGISwapChainDesc = {};		// �X���b�v�`�F�[���̐ݒ�f�[�^
	DXGISwapChainDesc.BufferDesc.Width = w;
	DXGISwapChainDesc.BufferDesc.Height = h;
	DXGISwapChainDesc.BufferDesc.RefreshRate.Numerator  = 0;
	DXGISwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	DXGISwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGISwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	DXGISwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;// DXGI_MODE_SCALING_CENTERED;	// DXGI_MODE_SCALING_UNSPECIFIED
	DXGISwapChainDesc.SampleDesc.Count = 1;
	DXGISwapChainDesc.SampleDesc.Quality = 0;
	DXGISwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	DXGISwapChainDesc.BufferCount = 2;
	DXGISwapChainDesc.OutputWindow = hWnd;
	DXGISwapChainDesc.Windowed = TRUE;
	DXGISwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	DXGISwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	if(FAILED(m_pGIFactory->CreateSwapChain(m_pDevice, &DXGISwapChainDesc, &m_pGISwapChain))){
		errMsg = "�X���b�v�`�F�C���쐬���s";

		Release();
		return false;
	}

	// �X���b�v�`�F�C������o�b�N�o�b�t�@�擾
	ID3D11Texture2D* pBackBuffer;
	if (FAILED(m_pGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer))) {
		errMsg = "�o�b�N�o�b�t�@�擾���s";

		Release();
		return false;
	}

	// �o�b�N�o�b�t�@���\�[�X����r���[���쐬
	m_backBuffer = std::make_shared<KdTexture>();
	if (m_backBuffer->Create(pBackBuffer) == false)
	{
		errMsg = "�o�b�N�o�b�t�@�쐬���s";
		Release();
		return false;
	}

	pBackBuffer->Release();

	/*
	// ALT+Enter�Ńt���X�N���[���s�ɂ���
	{
		IDXGIDevice* pDXGIDevice;
		m_pDevice->QueryInterface<IDXGIDevice>(&pDXGIDevice);

		IDXGIAdapter* pDXGIAdapter;
		pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);

		IDXGIFactory* pIDXGIFactory;
		pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pIDXGIFactory);

		pIDXGIFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);

		pDXGIDevice->Release();
		pDXGIAdapter->Release();
		pIDXGIFactory->Release();
	}
	*/

	//=========================================================
	// Z�o�b�t�@�쐬
	//=========================================================
	{
		// �o�b�t�@�쐬
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.Format = DXGI_FORMAT_R32_TYPELESS;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		desc.Width = (UINT)w;
		desc.Height = (UINT)h;
		desc.CPUAccessFlags = 0;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		m_zBuffer = std::make_shared<KdTexture>();
		if (m_zBuffer->Create(desc) == false)
		{
			errMsg = "Z�o�b�t�@�쐬���s";
			Release();
			return false;
		}
	}

	//=========================================================
	// �o�b�N�o�b�t�@�AZ�o�b�t�@��`��^�[�Q�b�g�Ƃ��ăf�o�C�X�R���e�L�X�g�փZ�b�g����
	//=========================================================
	{
		ID3D11RenderTargetView* rtvs[] = { m_backBuffer->GetRTView() };
		m_pDeviceContext->OMSetRenderTargets(1, rtvs, m_zBuffer->GetDSView());
	}

	//=========================================================
	// �r���[�|�[�g�̐ݒ�
	//=========================================================
	{
		// �r���[�|�[�g�̐ݒ�
		D3D11_VIEWPORT vp;
		vp.Width = (float)w;
		vp.Height = (float)h;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_pDeviceContext->RSSetViewports(1, &vp);
	}

	//=========================================================
	// �u�����h�X�e�[�g�Z�b�g���Z�b�g���Ă���
	//=========================================================

	// �u�����h�X�e�[�g�Z�b�g
	ID3D11BlendState* bs = CreateBlendState(KdBlendMode::Alpha);
	GetDevContext()->OMSetBlendState(bs, Math::Color(0, 0, 0, 0), 0xFFFFFFFF);
	bs->Release();

	// �T���v���[�X�e�[�g�Z�b�g
	ID3D11SamplerState* ss0 = CreateSamplerState(KdSamplerFilterMode::Anisotropic, 4, KdSamplerAddressingMode::Wrap, false);
	// �e�V�F�[�_�փZ�b�g
	GetDevContext()->VSSetSamplers(0, 1, &ss0);
	GetDevContext()->PSSetSamplers(0, 1, &ss0);
	GetDevContext()->GSSetSamplers(0, 1, &ss0);
	GetDevContext()->CSSetSamplers(0, 1, &ss0);
	ss0->Release();

	ID3D11SamplerState* ss1 = CreateSamplerState(KdSamplerFilterMode::Anisotropic, 4, KdSamplerAddressingMode::Clamp, false);
	// �e�V�F�[�_�փZ�b�g
	GetDevContext()->VSSetSamplers(1, 1, &ss1);
	GetDevContext()->PSSetSamplers(1, 1, &ss1);
	GetDevContext()->GSSetSamplers(1, 1, &ss1);
	GetDevContext()->CSSetSamplers(1, 1, &ss1);
	ss1->Release();

	// ���X�^���C�U�[�X�e�[�g�Z�b�g
	ID3D11RasterizerState* rs = CreateRasterizerState(D3D11_CULL_NONE, D3D11_FILL_SOLID, true, false);
	GetDevContext()->RSSetState(rs);
	rs->Release();

	// �[�x�X�e���V���X�e�[�g�Z�b�g
	ID3D11DepthStencilState* ds = CreateDepthStencilState(true, true);
	GetDevContext()->OMSetDepthStencilState(ds, 0);
	ds->Release();

	//=========================================================
	// 1x1�̔��e�N�X�`���쐬
	//=========================================================
	{
		// 0xAABBGGRR
		auto col = Math::Color(1, 1, 1, 1).RGBA();
		D3D11_SUBRESOURCE_DATA srd;
		srd.pSysMem = &col;
		srd.SysMemPitch = 4;
		srd.SysMemSlicePitch = 0;

		m_texWhite = std::make_shared<KdTexture>();
		m_texWhite->Create(1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, 1, &srd);
	}

	// DrawVertices�p���_�o�b�t�@���쐬
	UINT bufferSize = 80;
	for (int i = 0; i < 10; i++)	// 
	{
		m_tempFixedVertexBuffer[i].Create(D3D11_BIND_VERTEX_BUFFER, bufferSize, D3D11_USAGE_DYNAMIC, nullptr);

		bufferSize *= 2;	// �e�ʂ�{�ɂ��Ă���
	}


	return true;
}

void KdDirect3D::Release()
{
	for (auto&& n : m_tempFixedVertexBuffer)
	{
		n.Release();
	}
	m_tempVertexBuffer.Release();

	m_backBuffer = nullptr;
	m_zBuffer = nullptr;
	m_texWhite = nullptr;

	KdSafeRelease(m_pGISwapChain);
	KdSafeRelease(m_pGIAdapter);
	KdSafeRelease(m_pGIFactory);
	KdSafeRelease(m_pDeviceContext);
	KdSafeRelease(m_pDevice);

}

ID3D11DepthStencilState* KdDirect3D::CreateDepthStencilState(bool zEnable, bool zWriteEnable) const
{
	// Z�o�b�t�@�̓���ݒ�
	D3D11_DEPTH_STENCIL_DESC desc = {};

	desc.DepthEnable = zEnable;							// �[�x�e�X�g���g�p����
	desc.DepthWriteMask = zWriteEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	desc.StencilEnable = FALSE;
	desc.StencilReadMask = 0;
	desc.StencilWriteMask = 0;

	// �X�e�[�g�I�u�W�F�N�g���쐬
	ID3D11DepthStencilState* state = nullptr;
	if (FAILED(m_pDevice->CreateDepthStencilState(&desc, &state)))return nullptr;

	return state;
}

ID3D11RasterizerState* KdDirect3D::CreateRasterizerState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, bool depthClipEnable, bool scissorEnable) const
{
	// ���X�^���C�Y�̓���ݒ�
	D3D11_RASTERIZER_DESC desc = {};
	desc.FillMode = fillMode;
	desc.CullMode = cullMode;
	desc.FrontCounterClockwise = FALSE;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0;
	desc.SlopeScaledDepthBias = 0;
	desc.DepthClipEnable = depthClipEnable;
	desc.ScissorEnable = scissorEnable;
	desc.MultisampleEnable = FALSE;
	desc.AntialiasedLineEnable = FALSE;

	// �X�e�[�g�I�u�W�F�N�g�쐬
	ID3D11RasterizerState* state = nullptr;
	if (FAILED(m_pDevice->CreateRasterizerState(&desc, &state)))return nullptr;

	return state;
}

ID3D11SamplerState* KdDirect3D::CreateSamplerState(KdSamplerFilterMode filterType, UINT maxAnisotropy, KdSamplerAddressingMode addressingMode, bool comparisonModel) const
{
	D3D11_SAMPLER_DESC desc = {};
	// �t�B���^
	switch (filterType) {
		// Point
	case KdSamplerFilterMode::Point:
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		break;
		// Linear
	case KdSamplerFilterMode::Linear:
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		break;
		// Anisotropic
	case KdSamplerFilterMode::Anisotropic:
		desc.Filter = D3D11_FILTER_ANISOTROPIC;
		desc.MaxAnisotropy = maxAnisotropy;
		break;
	}

	// �A�h���b�V���O���[�h
	switch (addressingMode) {
		// Wrap
	case KdSamplerAddressingMode::Wrap:
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		break;
		// Clamp
	case KdSamplerAddressingMode::Clamp:
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		break;
	}

	// ��r���[�h�T���v��
	if (comparisonModel) {
		desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;	// �Ƃ肠��������͐��`�t�B���^�Œ��
	}
	// �ʏ�T���v��
	else {
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	}

	// 
	desc.MipLODBias = 0;
	desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = desc.BorderColor[3] = 0;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	// �X�e�[�g�I�u�W�F�N�g�쐬
	ID3D11SamplerState* state = nullptr;
	if (FAILED(m_pDevice->CreateSamplerState(&desc, &state)))return nullptr;

	return state;
}

ID3D11BlendState* KdDirect3D::CreateBlendState(KdBlendMode mode) const
{
	D3D11_BLEND_DESC desc = {};
	// �L��
	desc.RenderTarget[0].BlendEnable = TRUE;
	// �������݃}�X�N�@R,G,B,A�S�ďo�͂���ꍇ��ALL
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	desc.IndependentBlendEnable = FALSE;
	desc.AlphaToCoverageEnable = FALSE;

	// �u�����h�Ȃ�
	if (mode == KdBlendMode::NoBlend)
	{
		// �F�̍������@
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		// �A���t�@�̍������@
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	}
	// �������u�����h
	else if (mode == KdBlendMode::Alpha)
	{
		// �F�̍������@
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		// �A���t�@�̍������@
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	}
	// ���Z�u�����h
	else if (mode == KdBlendMode::Add)
	{
		// �F�̍������@
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		// �A���t�@�̍������@
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	}

	// �X�e�[�g�I�u�W�F�N�g�쐬
	ID3D11BlendState* state = nullptr;
	if (FAILED(m_pDevice->CreateBlendState(&desc, &state)))return nullptr;

	return state;
}

void KdDirect3D::DrawVertices(D3D_PRIMITIVE_TOPOLOGY topology,int vertexCount, const void* pVertexStream, UINT stride)
{
	
	// �v���~�e�B�u�g�|���W�[���Z�b�g
	GetDevContext()->IASetPrimitiveTopology(topology);

	// �S���_�̑��o�C�g�T�C�Y
	UINT totalSize = vertexCount * stride;


	// �œK�ȌŒ蒷�o�b�t�@������
	KdBuffer* buffer = nullptr;
	for (auto&& n : m_tempFixedVertexBuffer)
	{
		if (totalSize < n.GetBufferSize())
		{
			buffer = &n;
			break;
		}
	}
	// �ϒ��̃o�b�t�@���g�p
	if (buffer == nullptr)
	{
		buffer = &m_tempVertexBuffer;

		// ���_�o�b�t�@�̃T�C�Y���������Ƃ��́A���T�C�Y�̂��ߍč쐬����
		if (m_tempVertexBuffer.GetBufferSize() < totalSize) {
			m_tempVertexBuffer.Create(D3D11_BIND_VERTEX_BUFFER, totalSize, D3D11_USAGE_DYNAMIC, nullptr);
		}
	}

	//============================================================
	//
	// �P����DISCARD�ł̏������݁B
	//  DISCARD�́A�V���ȃo�b�t�@������ō쐬���A�O��̃o�b�t�@�͎g���I���Ɩ����ɂ�����̂ł��B
	//  �܂菑�����ޓx�ɐV�K�̃o�b�t�@�ɂȂ銴���ł��B
	//  �o�b�t�@�̃T�C�Y���傫���ƁA���̕��V�K�̃o�b�t�@���쐬���鎞�Ԃ��������Ă��܂��܂��B
	//  ��������P�������ꍇ�́A�uDISCARD��NO_OVERWRITE�̑g�ݍ��킹�Z�v�̕��@�ōs���ق����悢�ł��B
	//
	//============================================================

	// �S���_���o�b�t�@�ɏ�������(DISCARD�w��)
	buffer->WriteData(pVertexStream, totalSize);

	// ���_�o�b�t�@�[���f�o�C�X�փZ�b�g
	{
		UINT offset = 0;
		D3D.GetDevContext()->IASetVertexBuffers(0, 1, buffer->GetAddress(), &stride, &offset);
	}

	// �`��
	GetDevContext()->Draw(vertexCount, 0);
	
}


