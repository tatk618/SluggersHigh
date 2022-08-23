#include "inc_KdSpriteShader.hlsli"

//============================================
// 2D�`�� ���_�V�F�[�_
//============================================
VSOutput main(  float4 pos : POSITION,
			    float2 uv : TEXCOORD0
){
	VSOutput Out;

	// ���_���W���ˉe�ϊ�
	Out.Pos = mul(pos, g_mTransform);
	Out.Pos = mul(Out.Pos, g_mProj);

	// UV
	Out.UV = uv;

	return Out;
}
