#include "System/KdSystem.h"

#include "KdSpriteShader.h"

bool KdSpriteShader::Init()
{
	Release();

	//-------------------------------------
	// ���_�V�F�[�_
	//-------------------------------------
	{
		// �R���p�C���ς݂̃V�F�[�_�[�w�b�_�[�t�@�C�����C���N���[�h
		#include "KdSpriteShader_VS.inc"

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
		};

		// ���_�C���v�b�g���C�A�E�g�쐬
		if (FAILED(D3D.GetDev()->CreateInputLayout(
			&layout[0],
			layout.size(),
			compiledBuffer,
			sizeof(compiledBuffer),
			&m_VLayout))
		){
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
		#include "KdSpriteShader_PS.inc"

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
	m_cb1.Create();

	//-------------------------------------
	// �X�e�[�g�쐬
	//-------------------------------------
	m_smp0_Point = D3D.CreateSamplerState(KdSamplerFilterMode::Point, 0, KdSamplerAddressingMode::Wrap, false);
	m_smp0_Linear = D3D.CreateSamplerState(KdSamplerFilterMode::Linear, 0, KdSamplerAddressingMode::Wrap, false);

	m_ds = D3D.CreateDepthStencilState(false, false);

	m_rs = D3D.CreateRasterizerState(D3D11_CULL_NONE, D3D11_FILL_SOLID, true, false);

	//�t�H���g�֌W
	spriteBatch = new DirectX::SpriteBatch(D3D.GetDevContext());

	return true;
}

void KdSpriteShader::Release()
{
	KdSafeRelease(m_VS);
	KdSafeRelease(m_PS);
	KdSafeRelease(m_VLayout);
	m_cb0.Release();

	KdSafeRelease(m_ds);
	KdSafeRelease(m_rs);
	KdSafeRelease(m_smp0_Point);
	KdSafeRelease(m_smp0_Linear);

	KdSafeRelease(m_saveState.DS);
	KdSafeRelease(m_saveState.RS);
	KdSafeRelease(m_saveState.Smp0);

	KdSafeDelete(spriteBatch);
}

void KdSpriteShader::Begin(bool linear, bool disableZBuffer)
{
	// ����Begin���Ă���
	if (m_isBegin)return;
	m_isBegin = true;

	//---------------------------------------
	// 2D�p���ˉe�s��쐬
	//---------------------------------------
	UINT pNumVierports = 1;
	D3D11_VIEWPORT vp;
	D3D.GetDevContext()->RSGetViewports(&pNumVierports, &vp);
	m_mProj2D = DirectX::XMMatrixOrthographicLH(vp.Width, vp.Height, 0, 1);

	// �萔�o�b�t�@��������
	m_cb1.Work().mProj = m_mProj2D;
	m_cb1.Write();

	//---------------------------------------
	// ���݂̃X�e�[�g�L��
	//---------------------------------------
	// DepthStencilState�L��
	D3D.GetDevContext()->OMGetDepthStencilState(&m_saveState.DS, &m_saveState.StencilRef);
	// RasterizerState�L��
	D3D.GetDevContext()->RSGetState(&m_saveState.RS);
	// SamplerState�L��
	D3D.GetDevContext()->PSGetSamplers(0, 1, &m_saveState.Smp0);

	//---------------------------------------
	// �g�p����X�e�[�g���Z�b�g
	//---------------------------------------
	// Z����AZ�������ݖ����̃X�e�[�g���Z�b�g
	if (disableZBuffer) {
		D3D.GetDevContext()->OMSetDepthStencilState(m_ds, 0);
	}
	// Sampler�X�e�[�g���Z�b�g
	if (linear) {
		D3D.GetDevContext()->PSSetSamplers(0, 1, &m_smp0_Linear);
	}
	else {
		D3D.GetDevContext()->PSSetSamplers(0, 1, &m_smp0_Point);
	}
	// Rasterizer�X�e�[�g���Z�b�g
	D3D.GetDevContext()->RSSetState(m_rs);

	//---------------------------------------
	// �V�F�[�_
	//---------------------------------------

	// �V�F�[�_���Z�b�g
	D3D.GetDevContext()->VSSetShader(m_VS, 0, 0);
	D3D.GetDevContext()->PSSetShader(m_PS, 0, 0);

	// ���_���C�A�E�g�Z�b�g
	D3D.GetDevContext()->IASetInputLayout(m_VLayout);

	// �萔�o�b�t�@�Z�b�g
	D3D.GetDevContext()->VSSetConstantBuffers(0, 1, m_cb0.GetAddress());
	D3D.GetDevContext()->PSSetConstantBuffers(0, 1, m_cb0.GetAddress());

	D3D.GetDevContext()->VSSetConstantBuffers(1, 1, m_cb1.GetAddress());
	D3D.GetDevContext()->PSSetConstantBuffers(1, 1, m_cb1.GetAddress());
}

void KdSpriteShader::End()
{
	if (!m_isBegin)return;
	m_isBegin = false;

	//---------------------------------------
	// �L�����Ă��X�e�[�g�ɖ߂�
	//---------------------------------------
	D3D.GetDevContext()->OMSetDepthStencilState(m_saveState.DS, m_saveState.StencilRef);
	D3D.GetDevContext()->RSSetState(m_saveState.RS);
	D3D.GetDevContext()->PSSetSamplers(0, 1, &m_saveState.Smp0);
	KdSafeRelease(m_saveState.DS);
	KdSafeRelease(m_saveState.RS);
	KdSafeRelease(m_saveState.Smp0);
}

void KdSpriteShader::DrawTex(const KdTexture* tex, int x, int y, int w, int h, const Math::Rectangle* srcRect, const Math::Color* color, const Math::Vector2& pivot)
{
	if (tex == nullptr)return;

	// �����J�n���Ă��Ȃ��ꍇ�͊J�n����(�Ō��End())
	bool bBgn = m_isBegin;
	if (!bBgn)Begin();

	// �e�N�X�`��(ShaderResourceView)�Z�b�g
	D3D.GetDevContext()->PSSetShaderResources(0, 1, tex->GetSRViewAddress());

	// �F
	if (color) {
		m_cb0.Work().Color = *color;
	}
	m_cb0.Write();

	// UV
	Math::Vector2 uvMin = { 0, 0 };
	Math::Vector2 uvMax = { 1, 1 };
	if (srcRect)
	{
		uvMin.x = srcRect->x / (float)tex->GetInfo().Width;
		uvMin.y = srcRect->y / (float)tex->GetInfo().Height;

		uvMax.x = (srcRect->x + srcRect->width) / (float)tex->GetInfo().Width;
		uvMax.y = (srcRect->y + srcRect->height) / (float)tex->GetInfo().Height;
	}

	// ���_�쐬
	float x1 = (float)x;
	float y1 = (float)y;
	float x2 = (float)(x + w);
	float y2 = (float)(y + h);

	// ��_(Pivot)�Ԃ񂸂炷
	x1 -= pivot.x * w;
	x2 -= pivot.x * w;
	y1 -= pivot.y * h;
	y2 -= pivot.y * h;

	Vertex vertex[] = {
		{ {x1, y1, 0},	{uvMin.x, uvMax.y} },
		{ {x1, y2, 0},	{uvMin.x, uvMin.y} },
		{ {x2, y1, 0},	{uvMax.x, uvMax.y} },
		{ {x2, y2, 0},	{uvMax.x, uvMin.y} }

	};

	// �`��
	D3D.DrawVertices(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 4, vertex, sizeof(Vertex));

	// �Z�b�g�����e�N�X�`�����������Ă���
	ID3D11ShaderResourceView* srv = nullptr;
	D3D.GetDevContext()->PSSetShaderResources(0, 1, &srv);

	// ���̊֐���Begin�����ꍇ�́AEnd���Ă���
	if (!bBgn)End();
}

void KdSpriteShader::DrawPoint(int x, int y, const Math::Color* color)
{
	// �����J�n���Ă��Ȃ��ꍇ�͊J�n����(�Ō��End())
	bool bBgn = m_isBegin;
	if (!bBgn)Begin();

	// ���e�N�X�`��
	D3D.GetDevContext()->PSSetShaderResources(0, 1, D3D.GetWhiteTex()->GetSRViewAddress());

	// �F
	if (color) {
		m_cb0.Work().Color = *color;
	}
	m_cb0.Write();


	// �`��
	Vertex vertex[] = {
		{ {(float)x, (float)y, 0},	{0, 0} },
	};
	D3D.DrawVertices(D3D_PRIMITIVE_TOPOLOGY_POINTLIST, 1, vertex, sizeof(Vertex));

	// ���̊֐���Begin�����ꍇ�́AEnd���Ă���
	if (!bBgn)End();
}

void KdSpriteShader::DrawLine(int x1, int y1, int x2, int y2, const Math::Color* color)
{
	// �����J�n���Ă��Ȃ��ꍇ�͊J�n����(�Ō��End())
	bool bBgn = m_isBegin;
	if (!bBgn)Begin();

	// ���e�N�X�`��
	D3D.GetDevContext()->PSSetShaderResources(0, 1, D3D.GetWhiteTex()->GetSRViewAddress());

	// �F
	if (color) {
		m_cb0.Work().Color = *color;
	}
	m_cb0.Write();


	// �`��
	Vertex vertex[] = {
		{ {(float)x1, (float)y1, 0},	{0, 0} },
		{ {(float)x2, (float)y2, 0},	{1, 0} },
	};
	D3D.DrawVertices(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, 2, vertex, sizeof(Vertex));

	// ���̊֐���Begin�����ꍇ�́AEnd���Ă���
	if (!bBgn)End();
}

void KdSpriteShader::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Math::Color* color, bool fill)
{
	// �����J�n���Ă��Ȃ��ꍇ�͊J�n����(�Ō��End())
	bool bBgn = m_isBegin;
	if (!bBgn)Begin();

	// ���e�N�X�`��
	D3D.GetDevContext()->PSSetShaderResources(0, 1, D3D.GetWhiteTex()->GetSRViewAddress());

	// �F
	if (color) {
		m_cb0.Work().Color = *color;
	}
	m_cb0.Write();


	// �`��
	Vertex vertex[] = {
		{ {(float)x1, (float)y1, 0},	{0, 0} },
		{ {(float)x2, (float)y2, 0},	{1, 0} },
		{ {(float)x3, (float)y3, 0},	{0, 0} },
		{ {(float)x1, (float)y1, 0},	{1, 0} },
	};
	D3D.DrawVertices(
		fill ? D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP : D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
		4, vertex, sizeof(Vertex));


	// ���̊֐���Begin�����ꍇ�́AEnd���Ă���
	if (!bBgn)End();
}

void KdSpriteShader::DrawCircle(int x, int y, int radius, const Math::Color* color, bool fill)
{
	if (radius <= 0)return;

	// �����J�n���Ă��Ȃ��ꍇ�͊J�n����(�Ō��End())
	bool bBgn = m_isBegin;
	if (!bBgn)Begin();

	// ���e�N�X�`��
	D3D.GetDevContext()->PSSetShaderResources(0, 1, D3D.GetWhiteTex()->GetSRViewAddress());

	// �F
	if (color) {
		m_cb0.Work().Color = *color;
	}
	m_cb0.Write();

	// ���_
	if (fill)
	{
		int faceNum = radius + 1;
		if (faceNum > 300)faceNum = 300;
		std::vector<Vertex> vertex(faceNum * 3);		// ���a�ɂ�蒸�_���𒲐�

		// �`��
		for (int i = 0; i < faceNum; i++)
		{
			int idx = i * 3;
			vertex[idx].Pos.x = (float)x;
			vertex[idx].Pos.y = (float)y;

			vertex[idx+1].Pos.x = x + cos(DirectX::XMConvertToRadians(i * (360.0f / (faceNum - 1)))) * (float)radius;
			vertex[idx+1].Pos.y = y + sin(DirectX::XMConvertToRadians(i * (360.0f / (faceNum - 1)))) * (float)radius;
			vertex[idx+1].Pos.z = 0;

			vertex[idx+2].Pos.x = x + cos(DirectX::XMConvertToRadians((i+1) * (360.0f / (faceNum - 1)))) * (float)radius;
			vertex[idx+2].Pos.y = y + sin(DirectX::XMConvertToRadians((i+1) * (360.0f / (faceNum - 1)))) * (float)radius;
			vertex[idx+2].Pos.z = 0;
		}

		D3D.DrawVertices(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)vertex.size(), &vertex[0], sizeof(Vertex));
	}
	else
	{
		int numVertex = radius + 1;
		if (numVertex > 300)numVertex = 300;
		std::vector<Vertex> vertex(numVertex);		// ���a�ɂ�蒸�_���𒲐�

		// �`��
		for (int i = 0; i < numVertex; i++)
		{
			vertex[i].Pos.x = x + cos(DirectX::XMConvertToRadians(i * (360.0f / (numVertex - 1)))) * (float)radius;
			vertex[i].Pos.y = y + sin(DirectX::XMConvertToRadians(i * (360.0f / (numVertex - 1)))) * (float)radius;
			vertex[i].Pos.z = 0;
		}

		D3D.DrawVertices(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, numVertex, &vertex[0], sizeof(Vertex));
	}

	// ���̊֐���Begin�����ꍇ�́AEnd���Ă���
	if (!bBgn)End();
}

void KdSpriteShader::DrawBox(int x, int y, int extentX, int extentY, const Math::Color* color, bool fill)
{
	// �����J�n���Ă��Ȃ��ꍇ�͊J�n����(�Ō��End())
	bool bBgn = m_isBegin;
	if (!bBgn)Begin();

	// ���e�N�X�`��
	D3D.GetDevContext()->PSSetShaderResources(0, 1, D3D.GetWhiteTex()->GetSRViewAddress());

	// �F
	if (color) {
		m_cb0.Work().Color = *color;
	}
	m_cb0.Write();

	Math::Vector3 p1 = { (float)x - extentX, (float)y - extentY, 0 };
	Math::Vector3 p2 = { (float)x - extentX, (float)y + extentY, 0 };
	Math::Vector3 p3 = { (float)x + extentX, (float)y + extentY, 0 };
	Math::Vector3 p4 = { (float)x + extentX, (float)y - extentY, 0 };

	// �`��
	if (fill)
	{
		Vertex vertex[] = {
			{ p1, {0,0}},
			{ p2, {0,0}},
			{ p4, {0,0}},
			{ p3, {0,0}}
		};

		D3D.DrawVertices(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 4, vertex, sizeof(Vertex));
	}
	else
	{
		Vertex vertex[] = {
			{ p1, {0,0}},
			{ p2, {0,0}},
			{ p3, {0,0}},
			{ p4, {0,0}},
			{ p1, {0,0}}
		};

		D3D.DrawVertices(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, 5, vertex, sizeof(Vertex));
	}

	// ���̊֐���Begin�����ꍇ�́AEnd���Ă���
	if (!bBgn)End();
}

// �؂蔲���͈͂�ݒ肷��
// �Erect			�c �͈�

void KdSpriteShader::SetScissorRect(const Math::Rectangle& rect)
{
	// ���X�^���C�U�X�e�[�g�쐬�E�Z�b�g
	ID3D11RasterizerState* rs = D3D.CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID, true, true);
	D3D.GetDevContext()->RSSetState(rs);
	rs->Release();

	D3D11_RECT rc;
	rc.left = rect.x;
	rc.top = rect.y;
	rc.right = rect.x + rect.width;
	rc.bottom = rect.y + rect.height;
	D3D.GetDevContext()->RSSetScissorRects(1, &rc);
}

