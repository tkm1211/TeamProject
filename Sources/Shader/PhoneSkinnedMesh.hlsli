struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 color    : COLOR;
	float2 tex      : TEXCOORD;

	float3 wNormal : TEXCOORD1;
	float3 wPos    : TEXCOORD2;
};

#define MAX_BONES 128

//-----------------------------------------
// ポイントライト構造体
//-----------------------------------------
struct POINTLIGHT {
	float index;
	float range;
	float type;
	float dumy;
	float4 pos;
	float4 color;
};
#define POINTMAX 96
//-----------------------------------------
// スポットライト構造体
//-----------------------------------------
struct SPOTLIGHT {
	float index;
	float range;//光の届く範囲
	float type; //有効か無効か
	float inner_corn;
	float outer_corn;
	float dumy0;
	float dumy1;
	float dumy2;
	float4 pos;
	float4 color;
	float4 dir;
};
#define SPOTMAX 32

cbuffer CONSTANT_BUFFER : register(b0)
{
	row_major float4x4 world_view_projection;
	row_major float4x4 world;
	float4 material_color;
	float4 light_direction;
	row_major float4x4 bone_transforms[MAX_BONES];
};

cbuffer CONSTANT_BUFFER_LIGHT_01 : register(b2)
{
	float4 LightColor;					//ライトの色
	float4 LightDir;					//ライトの方向
	float4 AmbientColor;				//環境光
	float4 EyePos;						//カメラ座標
};

cbuffer CONSTANT_BUFFER_LIGHT_02 : register(b3)
{
	POINTLIGHT PointLight[POINTMAX];	//ポイントライト配列
	SPOTLIGHT SpotLight[SPOTMAX];		//スポットライト配列
};