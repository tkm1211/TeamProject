#include "SkinnedMeshBatch.hlsli" 

Texture2D    myTexture : register(t0); //�e�N�X�`���[
SamplerState mySampler : register(s0); //�T���v���[

float4 main(VS_OUT pin) : SV_TARGET
{
	return myTexture.Sample(mySampler, pin.tex) * pin.color;
}