#include "Blender.h"

void SetBlenderMode( int mode )
{
	static Blender blender( FrameWork::GetInstance().GetDevice().Get() );
	FrameWork::GetInstance().GetContext()->OMSetBlendState( blender.states[mode].Get(), nullptr, 0xFFFFFFFF );
}