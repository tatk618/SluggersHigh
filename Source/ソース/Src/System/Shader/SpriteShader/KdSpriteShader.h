#pragma once

#include <SpriteFont.h>

//===================================================
//
// 2D�`��V�F�[�_
//
//===================================================
class KdSpriteShader {
public:

	struct Vertex
	{
		Math::Vector3 Pos;
		Math::Vector2 UV;
	};

	//===========================================
	// �������E���
	//===========================================

	// ������
	bool Init();

	// ���
	void Release();

	// 
	~KdSpriteShader() {
		Release();
	}

	//===========================================
	//
	// �`��n
	//
	//===========================================

	// �`��J�n
	// �X�e�[�g�̋L���E�ύX��A2D�`��p�̍s��̐ݒ�A�V�F�[�_�؂�ւ��Ȃǂ��s��
	// �Elinear			�c ���`��ԃ��[�h�ɂ���
	// �EdisableZBuffer	�c Z�o�b�t�@���g�p���Ȃ�
	void Begin(bool linear = false, bool disableZBuffer = true);

	// �`��I��
	//  Begin()�ŋL�����Ă����X�e�[�g�𕜌� 
	void End();

	// �ϊ��s��Z�b�g
	void SetMatrix(const Math::Matrix& m)
	{
		m_cb0.Work().mTransform = m;
	}

	// 2D�摜�`��(Begin�`End�ԂŎ��s����ƁA�����������オ��)
	// �Etex			�c �`�悷��e�N�X�`��(Texture)
	// �Ex				�c x���W(�s�N�Z��)
	// �Ey				�c y���W(�s�N�Z��)
	// �Ew				�c w���W(�s�N�Z��)
	// �Eh				�c h���W(�s�N�Z��)
	// �EsrcRect		�c ���摜��RECT nullptr�őS��
	// �Ecolor			�c �F(RGBA) nullptr�ŐF�̓Z�b�g���Ȃ�(�O��̕`�掞�̐F���g�p�����)
	// �Epivot			�c ��_ 0.0�`1.0�͈̔͂Ŏw�肷��
	void DrawTex(const KdTexture* tex, int x, int y, int w, int h, const Math::Rectangle* srcRect = nullptr, const Math::Color* color = &Math::Color(1, 1, 1, 1), const Math::Vector2& pivot = { 0.5, 0.5f });

	// 2D�摜�`��(Begin�`End�ԂŎ��s����ƁA�����������オ��)
	// ���ƍ�����tex�̏�񂪎g�p�����
	// �Etex			�c �`�悷��e�N�X�`��(Texture)
	// �Ex				�c x���W(�s�N�Z��)
	// �Ey				�c y���W(�s�N�Z��)
	// �EsrcRect		�c ���摜��RECT
	// �Ecolor			�c �F(RGBA)
	// �Epivot			�c ��_ 0.0�`1.0�͈̔͂Ŏw�肷��
	void DrawTex(const KdTexture* tex, int x, int y, const Math::Rectangle* srcRect = nullptr, const Math::Color* color = &Math::Color(1, 1, 1, 1), const Math::Vector2& pivot = { 0.5, 0.5f })
	{
		if (tex == nullptr)return;
		DrawTex(tex, x, y, srcRect->width, srcRect->height, srcRect, color, pivot);
	}

	//���Ɨp�V���v����
	void DrawTex(const KdTexture* tex, const Math::Rectangle& srcRect = { 0,0,0,0 }, const float alpha = 1.0f, const Math::Vector2& pivot = { 0.5, 0.5f })
	{
		if (tex == nullptr)return;
		DrawTex(tex, 0, 0, srcRect.width, srcRect.height, &srcRect, &Math::Color(1, 1, 1, alpha), pivot);
	}

	//���Ɨp�����\��
	void DrawString(DirectX::SpriteFont* spriteFont, float _x, float _y, const char _text[], const Math::Vector4& _color)
	{
		//���C�h������ɕϊ�����K�v������
		WCHAR *_wtext = new WCHAR[strlen(_text) + 1];
		mbstowcs_s(nullptr, _wtext, strlen(_text) + 1, _text, _TRUNCATE);

		//Begin�O��BlendState���擾
		ID3D11BlendState *oldBlendState = 0;
		float oldFactor[4];
		UINT oldMask = 0;
		D3D.GetDevContext()->OMGetBlendState(&oldBlendState, oldFactor, &oldMask);

		//BlendState�������p����Begin
		spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, oldBlendState);
		spriteFont->DrawString(spriteBatch, _wtext, Math::Vector2(_x + 640, -_y + 360), _color);
		spriteBatch->End();

		//Begin�O��BlendState�𕜌�
		D3D.GetDevContext()->OMSetBlendState(oldBlendState, oldFactor, oldMask);

		delete[] _wtext;
	}


	// �_��`��
	// �Ex				�c �_��X���W
	// �Ey				�c �_��Y���W
	// �Ecolor			�c �F(RGBA)
	void DrawPoint(int x, int y, const Math::Color* color = &Math::Color(1, 1, 1, 1));

	// 2D����`��
	// �Ex1				�c �_�P��X���W
	// �Ey1				�c �_�P��Y���W
	// �Ex2				�c �_�Q��X���W
	// �Ey2				�c �_�Q��Y���W
	// �Ecolor			�c �F(RGBA)
	void DrawLine(int x1, int y1, int x2, int y2, const Math::Color* color = &Math::Color(1, 1, 1, 1));

	// �O�p�`��`��
	// �Ex1				�c ���W�P��X���W
	// �Ey1				�c ���W�P��Y���W
	// �Ex2				�c ���W�Q��X���W
	// �Ey2				�c ���W�Q��Y���W
	// �Ex3				�c ���W�R��X���W
	// �Ey3				�c ���W�R��Y���W
	// �Efill			�c �h��Ԃ�
	void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Math::Color* color = &Math::Color(1, 1, 1, 1), bool fill = true);

	// 2D�~��`��
	// �Ex				�c �~�̒��S���W��X���W
	// �Ey				�c �~�̒��S���W��Y���W
	// �Eradius			�c �~�̔��a
	// �Ecolor			�c �F(RGBA)
	// �Efill			�c �h��Ԃ�
	void DrawCircle(int x, int y, int radius, const Math::Color* color = &Math::Color(1, 1, 1, 1), bool fill = true);

	// 2D����`��
	// �Ex				�c ���̒��S���W��X���W
	// �Ey				�c ���̒��S���W��Y���W
	// �EextentX		�c ����X�����̃n�[�t�T�C�Y
	// �EextentY		�c ����Y�����̃n�[�t�T�C�Y
	// �Ecolor			�c �F(RGBA)
	// �Efill			�c �h��Ԃ�
	void DrawBox(int x, int y, int extentX, int extentY, const Math::Color* color = &Math::Color(1, 1, 1, 1), bool fill = true);

	// �؂蔲���͈͂�ݒ肷��
	// �Erect			�c �͈�
	void SetScissorRect(const Math::Rectangle& rect);

private:

	ID3D11VertexShader*		m_VS = nullptr;				// ���_�V�F�[�_�[
	ID3D11InputLayout*		m_VLayout = nullptr;		// ���_���C�A�E�g

	ID3D11PixelShader*		m_PS = nullptr;				// �s�N�Z���V�F�[�_�[

	// �萔�o�b�t�@
	struct cbSprite {
		Math::Matrix		mTransform;
		Math::Vector4		Color = { 1, 1, 1, 1 };
	};
	KdConstantBuffer<cbSprite>	m_cb0;

	// �萔�o�b�t�@
	struct cbProjection {
		Math::Matrix		mProj;
	};
	KdConstantBuffer<cbProjection>	m_cb1;

	// Flag
	bool					m_isBegin = false;

	// 2D�p���ˉe�s��
	Math::Matrix			m_mProj2D;

	// �g�p����X�e�[�g
	ID3D11DepthStencilState*	m_ds = nullptr;
	ID3D11RasterizerState*		m_rs = nullptr;
	ID3D11SamplerState*			m_smp0_Point = nullptr;
	ID3D11SamplerState*			m_smp0_Linear = nullptr;

	// �X�e�[�g�L��/�����p
	struct SaveState {
		ID3D11DepthStencilState*	DS = nullptr;
		ID3D11RasterizerState*		RS = nullptr;
		UINT						StencilRef = 0;
		ID3D11SamplerState*			Smp0 = nullptr;
	};
	SaveState					m_saveState;

	DirectX::SpriteBatch* spriteBatch;	//�t�H���g�\���p
};


