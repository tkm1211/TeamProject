struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

cbuffer CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 view_projection;
    float4 material_color;
    float4 light_direction;
};