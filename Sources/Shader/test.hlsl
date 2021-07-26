#include "test.hlsli"


[numthreads(2, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	OutBuffer[DTid.x].i = InBuffer[DTid.x].i + (int)InBuffer[DTid.x].f;
}