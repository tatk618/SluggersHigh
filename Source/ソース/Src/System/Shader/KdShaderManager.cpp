#include "System/KdSystem.h"

#include "KdShaderManager.h"

void KdShaderManager::Init()
{

	//============================================
	// �V�F�[�_
	//============================================

	// ��{�V�F�[�_
	m_standardShader.Init();
	// �G�t�F�N�g�V�F�[�_
	m_effectShader.Init();
	// 2D�X�v���C�g�V�F�[�_
	m_spriteShader.Init();

	//============================================
	// �萔�o�b�t�@
	//============================================

	// �J����
	m_cb7_Camera.Create();
	D3D.GetDevContext()->VSSetConstantBuffers( 7,   1, m_cb7_Camera.GetAddress());
	D3D.GetDevContext()->PSSetConstantBuffers( 7,   1, m_cb7_Camera.GetAddress());

	// �����r���[�s��
	m_cb7_Camera.Work().mV = Math::Matrix::CreateTranslation(0, 0, -5);
	m_cb7_Camera.Work().mV = m_cb7_Camera.Work().mV.Invert();

	// �����ˉe�s��
	m_cb7_Camera.Work().mP = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60), D3D.GetBackBuffer()->GetAspectRatio(), 0.01f, 1000);


	// ���C�g
	m_cb8_Light.Create();
	D3D.GetDevContext()->VSSetConstantBuffers( 8,	1, m_cb8_Light.GetAddress());
	D3D.GetDevContext()->PSSetConstantBuffers( 8,	1, m_cb8_Light.GetAddress());

}

void KdShaderManager::Release()
{
	m_standardShader.Release();
	m_effectShader.Release();
	m_spriteShader.Release();

	m_cb7_Camera.Release();
	m_cb8_Light.Release();

}
