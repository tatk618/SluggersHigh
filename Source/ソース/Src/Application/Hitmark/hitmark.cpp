#include "../hitmark/hitmark.h"
#include "../Manager/Texture/TextureManager.h"
#include "../define.h"

hitmarkClass::hitmarkClass(const Math::Vector3& _enemy_pos)
{
	timeGetTime;

	hitmark_tex = TEXTURE.GetTexture("Data/Texture/hitmark/bonk.png");

	Vertex v[4];

	hitmark_vb.Create(D3D11_BIND_VERTEX_BUFFER, sizeof(v), D3D11_USAGE_DYNAMIC, nullptr);

	Math::Vector3 tmpmat;
	tmpmat = { 0,0,0 };

	hitmark_pos = _enemy_pos + tmpmat;

	hitmark_alpha = 1.0f;

	plus = 0.1f;

}

hitmarkClass::~hitmarkClass()
{

}

bool hitmarkClass::Update()
{
	rotMat = SHADER.m_cb7_Camera.Work().mV;
	rotMat._41 = 0.0f;	//x
	rotMat._42 = 0.0f;	//y
	rotMat._43 = 0.0f;	//z

	plus += 0.5f;
	//if (plus >= 5.0f)
	//{
	//	plus -= 4.9f;
	//}

	//逆行列にする
	rotMat = rotMat.Invert();
	transMat = DirectX::XMMatrixTranslation(hitmark_pos.x,hitmark_pos.y,hitmark_pos.z);
	scalMat = DirectX::XMMatrixScaling(plus, plus, plus);

	//各行列の合成
	hitmark_mat = scalMat * rotMat * transMat;

	count++;

	if (plus >= 2.0f)
	{
		hitmark_alpha -= 0.1f;
		if (hitmark_alpha <= 0.01f)
		{
			plus -= 1.9f;
			return false;
		}
	}
}

void hitmarkClass::Draw()
{
	Vertex v[4];	//頂点の設定

	//横幅4、高さ4の四角形ポリゴン
	v[0].pos = { -2,2,0 };
	v[1].pos = { 2,2,0 };
	v[2].pos = { -2,-2,0 };
	v[3].pos = { 2,-2,0 };

	v[0].color = { 1,1,1,hitmark_alpha };	//Red,Green,Blue,Alpha(不透明度 1=表示、0=非表示)
	v[1].color = { 1,1,1,hitmark_alpha };
	v[2].color = { 1,1,1,hitmark_alpha };
	v[3].color = { 1,1,1,hitmark_alpha };

	v[0].uv = { 0.0f / hitmark_tex->GetWidth(),0.0f / hitmark_tex->GetWidth() };
	v[1].uv = { 128.0f / hitmark_tex->GetWidth(),0.0f / hitmark_tex->GetWidth() };
	v[2].uv = { 0.0f / hitmark_tex->GetWidth(),128.0f / hitmark_tex->GetWidth() };
	v[3].uv = { 128.0f / hitmark_tex->GetWidth(),128.0f / hitmark_tex->GetWidth() };

	hitmark_vb.WriteData(&v[0], sizeof(v));

	SHADER.m_effectShader.SetToDevice();

	SHADER.m_effectShader.SetWorldMatrix(hitmark_mat);
	SHADER.m_effectShader.WriteToCB();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	D3D.GetDevContext()->IASetVertexBuffers(0, 1, hitmark_vb.GetAddress(), &stride, &offset);

	D3D.GetDevContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	D3D.GetDevContext()->PSSetShaderResources(0, 1, hitmark_tex->GetSRViewAddress());

	D3D.GetDevContext()->Draw(4, 0);
}
