#include "src/Application/ground/Ground.h"
#include "src/Application/define.h"
#include "src/Application/Manager/Texture/TextureManager.h"
#include "Src/Application/Manager/Model/ModelManager.h"

C_Ground::C_Ground()
{
	ground_tex = TEXTURE.GetTexture("Data/Texture/ground/metal.png");

	// 頂点の設定
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
	srd.pSysMem = &v[0];//書き込むテータの先頭アドレス
	ground_vb.Create(D3D11_BIND_VERTEX_BUFFER, sizeof(v), D3D11_USAGE_DEFAULT, &srd);

	// 行列
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
	//板ポリゴン用のシェーダーをセット
	SHADER.m_effectShader.SetToDevice();

	//頂点バッファをセット
	UINT stride = sizeof(Vertex);    //頂点ひとつあたりのバイトサイズ(Vector3+Vector2+Vector4(float型×9個))
	UINT offset = 0;    //オフセット
	D3D.GetDevContext()->IASetVertexBuffers(0, 1, ground_vb.GetAddress(), &stride, &offset);

	//プリミティブトポロジー（頂点の利用法）をセット
	D3D.GetDevContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// テクスチャをセット
	D3D.GetDevContext()->PSSetShaderResources(0, 1, ground_tex->GetSRViewAddress());

	//描画
	SHADER.m_effectShader.SetWorldMatrix(ground_mat);
	SHADER.m_effectShader.WriteToCB();
	D3D.GetDevContext()->Draw(4, 0);
}