struct PSInputShadow
{
	float4 Position : SV_POSITION;
	float4 Depth : TEXCOORD1;//ê[ìxíl
};

cbuffer CONSTANT_BUFFER : register(b0)
{
	row_major float4x4 world_view_projection;
	row_major float4x4 world;
	float4 material_color;
	float4 light_direction;
};