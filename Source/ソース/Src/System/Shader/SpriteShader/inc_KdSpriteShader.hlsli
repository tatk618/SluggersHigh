// �萔�o�b�t�@
cbuffer cbSprite : register(b0)
{
    row_major float4x4  g_mTransform;    // �ϊ��s��
    float4              g_color;
};

cbuffer cbProjection : register(b1)
{
    row_major float4x4  g_mProj;    // �ˉe�ϊ��s��
};

// ���_�V�F�[�_�o�͗p
struct VSOutput
{
	float4 Pos : SV_Position;
	float2 UV  : TEXCOORD0;
};
