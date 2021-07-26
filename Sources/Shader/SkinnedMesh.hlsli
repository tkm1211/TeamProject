struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 color    : COLOR;
	float2 tex      : TEXCOORD;
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