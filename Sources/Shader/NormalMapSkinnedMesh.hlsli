
struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 color    : COLOR;
	float2 tex      : TEXCOORD;

	float3 wNormal : TEXCOORD1;
	float3 wPos    : TEXCOORD2;

	float3 vT : TEXCOORD3;
	float3 vB : TEXCOORD4;
	float3 vN : TEXCOORD5;
};

#define MAX_BONES 128

cbuffer CONSTANT_BUFFER : register(b0)
{
	row_major float4x4 world_view_projection;
	row_major float4x4 world;
	float4 material_color;
	float4 light_direction;
	row_major float4x4 bone_transforms[MAX_BONES];
};

cbuffer CONSTANT_BUFFER_LIGHT : register(b2)
{
	float4 LightColor;	 //ライトの色
	float4 LightDir;	 //ライトの方向
	float4 AmbientColor; //環境光
	float4 EyePos;		 //カメラ座標
};