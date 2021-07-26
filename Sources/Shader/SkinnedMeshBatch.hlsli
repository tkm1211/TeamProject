
//struct VS_IN
//{
//	float4 position				: POSITION;
//	float4 normal				: NORMAL;
//	float2 tex					: TEXCOORD;
//	float4 boneWeights			: WEIGHTS;
//	uint4  boneIndices			: BONES;
//	row_major float4x4 wvp		: MATRIX01;	// インスタンスごとに設定される行列
//	row_major float4x4 world	: MATRIX02;	// インスタンスごとに設定される行列
//	uint instanceID				: SV_InstanceID;	// インスタンスＩＤ
//};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 color    : COLOR;
	float2 tex      : TEXCOORD;
};

#define MAX_FRAME 32
#define MAX_BONES 32

cbuffer CONSTANT_BUFFER : register( b0 )
{
	float4 materialColor;
	float4 lightDirection;
};

cbuffer BONE_CONSTANT_BUFFER_01 : register( b7 )
{
	row_major float4x4 boneTransforms1[MAX_FRAME][MAX_BONES];
};

cbuffer BONE_CONSTANT_BUFFER_02 : register( b8 )
{
	row_major float4x4 boneTransforms2[MAX_FRAME][MAX_BONES];
};

cbuffer BONE_CONSTANT_BUFFER_03 : register( b9 )
{
	row_major float4x4 boneTransforms3[MAX_FRAME][MAX_BONES];
};