#include "inc_KdSpriteShader.hlsli"

// �e�N�X�`��
Texture2D g_inputTex : register(t0);
// �T���v��
SamplerState g_ss : register(s0);


//============================================
// 2D�`�� �s�N�Z���V�F�[�_
//============================================
float4 main(VSOutput In) : SV_Target0
{
    // �e�N�X�`���F�擾
    float4 texCol = g_inputTex.Sample(g_ss, In.UV);
    //�A���t�@�e�X�g
    if (texCol.a < 0.1f) discard;

	// �e�N�X�`���F * �w��F
    return texCol * g_color;
}
