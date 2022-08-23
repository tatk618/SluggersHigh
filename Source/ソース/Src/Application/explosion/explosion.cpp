#include "explosion.h"
#include "../define.h"
#include "../Manager/Texture/TextureManager.h"
#include "../main.h"
#include "../enemies/drone/drone.h"
#include "../Manager/sound/SoundManager.h"

explosionClass::explosionClass(const Math::Vector3& _enemy_pos)
{
	explosion_tex = TEXTURE.GetTexture("Data/Texture/tex/Smoke.png");

	explosion_anime = 0;

	Vertex v[4];	//頂点の設定

	explosion_vb.Create(D3D11_BIND_VERTEX_BUFFER, sizeof(v), D3D11_USAGE_DYNAMIC, nullptr);

	explosion_pos = _enemy_pos;

	SOUND.Play3D("Data/Sound/SoundEffect/bomb.wav", explosion_pos, 0.3f);
}

explosionClass::~explosionClass()
{

}

bool explosionClass::Update()
{

	//爆発アニメーションを進行させる
	explosion_anime++;

	//アニメーションの進行度が６３を超えると最初に戻る
	if (explosion_anime >= 63)
	{
		explosion_anime = 0;
		return false;
	}

	rotMat = SHADER.m_cb7_Camera.Work().mV;
	rotMat._41 = 0.0f;	//x
	rotMat._42 = 0.0f;	//y
	rotMat._43 = 0.0f;	//z

	//逆行列にする
	rotMat = rotMat.Invert();
	transMat = DirectX::XMMatrixTranslation(explosion_pos.x,explosion_pos.y,explosion_pos.z);
	scalMat = DirectX::XMMatrixScaling(5, 5, 5);

	//各行列の合成
	explosion_mat = scalMat * rotMat * transMat;
	return true;
}

void explosionClass::Draw()
{
	Vertex v[4];	//頂点の設定

//横幅10、高さ10の四角形ポリゴン
	v[0].pos = { -5,5,0 };
	v[1].pos = { 5,5,0 };
	v[2].pos = { -5,-5,0 };
	v[3].pos = { 5,-5,0 };

	v[0].color = { 1,1,1,1 };	//Red,Green,Blue,Alpha(不透明度 1=表示、0=非表示)
	v[1].color = { 1,1,1,1 };
	v[2].color = { 1,1,1,1 };
	v[3].color = { 1,1,1,1 };

	v[0].uv = { (float)(explosion_anime % 8) * 32.0f / explosion_tex->GetWidth(),(float)(explosion_anime / 8) * 32.0f / explosion_tex->GetHeight() };
	v[1].uv = { (float)(explosion_anime % 8 + 1) * 32.0f / explosion_tex->GetWidth(),(float)(explosion_anime / 8) * 32.0f / explosion_tex->GetHeight() };
	v[2].uv = { (float)(explosion_anime % 8) * 32.0f / explosion_tex->GetWidth(), (float)(explosion_anime / 8 + 1) * 32.0f / explosion_tex->GetHeight() };
	v[3].uv = { (float)(explosion_anime % 8 + 1) * 32.0f / explosion_tex->GetWidth(), (float)(explosion_anime / 8 + 1) * 32.0f / explosion_tex->GetHeight() };

	explosion_vb.WriteData(&v[0], sizeof(v));


	SHADER.m_effectShader.SetToDevice();

	SHADER.m_effectShader.SetWorldMatrix(explosion_mat);
	SHADER.m_effectShader.WriteToCB();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	D3D.GetDevContext()->IASetVertexBuffers(0, 1, explosion_vb.GetAddress(), &stride, &offset);

	D3D.GetDevContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	D3D.GetDevContext()->PSSetShaderResources(0, 1, explosion_tex->GetSRViewAddress());

	D3D.GetDevContext()->Draw(4, 0);
}