#include "../redlight/Redlight.h"
#include "../Manager/Texture/TextureManager.h"
#include "../enemies/enemy/enemy.h"
#include "../main.h"
#include "../define.h"

redlightClass::redlightClass(const Math::Vector3& _enemy_pos)
{
	timeGetTime;

	Vertex v[60];

	D3D11_SUBRESOURCE_DATA srd = {};
	srd.pSysMem = &v[0];
	redlight_vb.Create(D3D11_BIND_VERTEX_BUFFER, sizeof(v), D3D11_USAGE_DYNAMIC, nullptr);

	redlight_tex = TEXTURE.GetTexture("Data/Texture/redlight/redlight2.png");

	blendadd = D3D.CreateBlendState(KdBlendMode::Add);
	blendnormal = D3D.CreateBlendState(KdBlendMode::Alpha);

	Math::Vector3 tmpmat;
	tmpmat = { 0,-1,0 };

	redlight_pos = _enemy_pos + tmpmat;

}

redlightClass::~redlightClass()
{

}

bool redlightClass::Update()
{
	rotMat = SHADER.m_cb7_Camera.Work().mV;
	rotMat._41 = 0.0f;	//x
	rotMat._42 = 0.0f;	//y
	rotMat._43 = 0.0f;	//z

	//�t�s��ɂ���
	rotMat = rotMat.Invert();
	transMat = DirectX::XMMatrixTranslation(redlight_pos.x, redlight_pos.y, redlight_pos.z);
	scalMat = DirectX::XMMatrixScaling(2,2,3);

	for (int i = 0;i < 30;i++)
	{
		redlight_mat[i] = redlight_mat[i - 1];
	}

	Vertex v[60];

	for (int i = 0;i < 30;i++)	//����30���̃��[�v
	{
		v[i * 2 + 0].pos = DirectX::XMVector3TransformCoord(Math::Vector3(-1, 0, 0), redlight_mat[i]);
		v[i * 2 + 1].pos = DirectX::XMVector3TransformCoord(Math::Vector3(1, 0, 0), redlight_mat[i]);

		v[i * 2 + 0].color = { 1,1,1,1 };
		v[i * 2 + 1].color = { 1,1,1,1 };

		v[i * 2 + 0].uv = { 0 ,(float)i / 49.0f };
		v[i * 2 + 1].uv = { 1 ,(float)i / 49.0f };

		//�����̂O�Ԗ�=>���_��[0]��[1]
		//�����̂P�Ԗ�=>���_��[2]��[3]
		//�����̂Q�Ԗ�=>���_��[4]��[5]
		//�����̂R�Ԗ�=>���_��[6]��[7]...

	}

	//�ϐ�v�̓��e��smoke_vb�ɏ�������
	redlight_vb.WriteData(&v[0], sizeof(v));

	for (int i = 29;i > 0;i--)
	{
		redlight_mat[i] = redlight_mat[i - 1];
	}

	////�e�s��̍���
	//redlight_mat = scalMat * rotMat * transMat;

	count++;

	if (count >= 60)
	{
		return false;
	}
}

void redlightClass::Draw()
{
	Vertex v[4];	//���_�̐ݒ�

	//����4�A����4�̎l�p�`�|���S��
	v[0].pos = { -2,2,0 };
	v[1].pos = { 2,2,0 };
	v[2].pos = { -2,-2,0 };
	v[3].pos = { 2,-2,0 };

	v[0].color = { 1,1,1,0.1f };	//Red,Green,Blue,Alpha(�s�����x 1=�\���A0=��\��)
	v[1].color = { 1,1,1,0.1f };
	v[2].color = { 1,1,1,0.1f };
	v[3].color = { 1,1,1,0.1f };

	v[0].uv = { 0.0f / redlight_tex->GetWidth(),0.0f / redlight_tex->GetWidth() };
	v[1].uv = { 64.0f / redlight_tex->GetWidth(),0.0f / redlight_tex->GetWidth() };
	v[2].uv = { 0.0f / redlight_tex->GetWidth(),64.0f / redlight_tex->GetWidth() };
	v[3].uv = { 64.0f / redlight_tex->GetWidth(),64.0f / redlight_tex->GetWidth() };

	redlight_vb.WriteData(&v[0], sizeof(v));

	SHADER.m_effectShader.SetToDevice();

	SHADER.m_effectShader.SetWorldMatrix(Math::Matrix::Identity);
	SHADER.m_effectShader.WriteToCB();

	D3D.GetDevContext()->OMSetBlendState(blendadd, Math::Color(0, 0, 0, 0), 0xFFFFFFFF);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	D3D.GetDevContext()->IASetVertexBuffers(0, 1, redlight_vb.GetAddress(), &stride, &offset);
	D3D.GetDevContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	D3D.GetDevContext()->PSSetShaderResources(0, 1, redlight_tex->GetSRViewAddress());

	D3D.GetDevContext()->Draw(60, 0);

	D3D.GetDevContext()->OMSetBlendState(blendnormal, Math::Color(0, 0, 0, 0), 0xFFFFFFFF);
}

void redlightClass::dronematset(Math::Matrix& _drone_mat)
{
	redlight_mat[0] = _drone_mat;
}

