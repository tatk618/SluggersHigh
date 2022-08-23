#include "src/Application/ground/Ground.h"
#include "src/Application/define.h"
#include "src/Application/Manager/Texture/TextureManager.h"
#include "Src/Application/Manager/Model/ModelManager.h"

C_Ground::C_Ground()
{
	ground_tex = TEXTURE.GetTexture("Data/Texture/ground/metal.png");

	// ���_�̐ݒ�
	Vertex v[4];
	v[0].pos = { -10.0f, 0.0f,  10.0f };
	v[1].pos = { 10.0f, 0.0f,  10.0f };
	v[2].pos = { -10.0f, 0.0f, -10.0f };
	v[3].pos = { 10.0f, 0.0f, -10.0f };

	v[0].color = { 1,1,1,1 };
	v[1].color = { 1,1,1,1 };
	v[2].color = { 1,1,1,1 };
	v[3].color = { 1,1,1,1 };

	v[0].uv = { 0.0f,  0.0f };
	v[1].uv = { 1.0f,  0.0f };
	v[2].uv = { 0.0f,  1.0f };
	v[3].uv = { 1.0f,  1.0f };

	D3D11_SUBRESOURCE_DATA srd = {};
	srd.pSysMem = &v[0];//�������ރe�[�^�̐擪�A�h���X
	ground_vb.Create(D3D11_BIND_VERTEX_BUFFER, sizeof(v), D3D11_USAGE_DEFAULT, &srd);

	// �s��
	Math::Matrix Transmat;
	Transmat = DirectX::XMMatrixTranslation(0, 0, 0);

	Math::Matrix Scalmat;
	Scalmat = DirectX::XMMatrixScaling(100, 100, 100);

	ground_mat = Scalmat * Transmat;
}

C_Ground::~C_Ground()
{
}

void C_Ground::Update()
{

}

void C_Ground::Draw()
{
	//�|���S���p�̃V�F�[�_�[���Z�b�g
	SHADER.m_effectShader.SetToDevice();

	//���_�o�b�t�@���Z�b�g
	UINT stride = sizeof(Vertex);    //���_�ЂƂ�����̃o�C�g�T�C�Y(Vector3+Vector2+Vector4(float�^�~9��))
	UINT offset = 0;    //�I�t�Z�b�g
	D3D.GetDevContext()->IASetVertexBuffers(0, 1, ground_vb.GetAddress(), &stride, &offset);

	//�v���~�e�B�u�g�|���W�[�i���_�̗��p�@�j���Z�b�g
	D3D.GetDevContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �e�N�X�`�����Z�b�g
	D3D.GetDevContext()->PSSetShaderResources(0, 1, ground_tex->GetSRViewAddress());

	//�`��
	SHADER.m_effectShader.SetWorldMatrix(ground_mat);
	SHADER.m_effectShader.WriteToCB();
	D3D.GetDevContext()->Draw(4, 0);
}