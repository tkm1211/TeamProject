#include "SwordLocus.hlsli"

VS_OUT main(float4 position : POSITION, float4 normal : NORMAL, float2 texcoord : TEXCOORD)
{
    VS_OUT vout;
    vout.position = mul(position, view_projection);
    normal.w = 0;
    //float4 N = normalize(mul(normal, world));
    float4 L = normalize(-light_direction);

    vout.color = material_color /** max(0.25f, dot(L, N))*/;
    vout.color.a = material_color.a;
    vout.texcoord = texcoord;
    return vout;
}