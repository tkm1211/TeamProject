#include "easing.h"

//==============================================================================
//
//		Easing.cpp
//
//      Easing Function �����\
//      URL https://easings.net/ja
//
//      �����L�T�C�g�̃R�[�h���Q�l�ɂ��Ă��܂��idouble�^��S��float�^�ɕύX�ς݁j
//      URL https://qiita.com/hart_edsf/items/962ac03281b871dcc0df
//
//==============================================================================


//�C�[�W���O�֐�
namespace easing
{
	float InQuad(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		return max * t*t + min;
	}
	float OutQuad(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		return -max * t*(t - 2) + min;
	}
	float InOutQuad(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		if (t / 2 < 1)
			return max / 2 * t * t + min;
		--t;
		return -max * (t * (t - 2) - 1) + min;
	}
	float InCubic(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		return max * t*t*t + min;
	}
	float OutCubic(float t, float totaltime, float max, float min)
	{
		max -= min;
		t = t / totaltime - 1;
		return max * (t*t*t + 1) + min;
	}
	float InOutCubic(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		if (t / 2 < 1)
			return max / 2 * t*t*t + min;
		t -= 2;
		return max / 2 * (t*t*t + 2) + min;
	}
	float InQuart(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		return max * t*t*t*t + min;
	}
	float OutQuart(float t, float totaltime, float max, float min)
	{
		max -= min;
		t = t / totaltime - 1;
		return -max * (t*t*t*t - 1) + min;
	}
	float InOutQuart(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		if (t / 2 < 1)
			return max / 2 * t*t*t*t + min;
		t -= 2;
		return -max / 2 * (t*t*t*t - 2) + min;
	}
	float InQuint(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		return max * t*t*t*t*t + min;
	}
	float OutQuint(float t, float totaltime, float max, float min)
	{
		max -= min;
		t = t / totaltime - 1;
		return max * (t*t*t*t*t + 1) + min;
	}
	float InOutQuint(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		if (t / 2 < 1)
			return max / 2 * t*t*t*t*t + min;
		t -= 2;
		return max / 2 * (t*t*t*t*t + 2) + min;
	}
	float InSine(float t, float totaltime, float max, float min)
	{
		max -= min;
		return -max * cos(t*ToRadian(90) / totaltime) + max + min;
	}
	float OutSine(float t, float totaltime, float max, float min)
	{
		max -= min;
		return max * sin(t*ToRadian(90) / totaltime) + min;
	}
	float InOutSine(float t, float totaltime, float max, float min)
	{
		max -= min;
		return -max / 2 * (cos(t*DirectX::XM_PI / totaltime) - 1) + min;
	}
	float InExp(float t, float totaltime, float max, float min)
	{
		max -= min;
		return t == 0.0f ? min : max * powf(2, 10 * (t / totaltime - 1)) + min;
	}
	float OutExp(float t, float totaltime, float max, float min)
	{
		max -= min;
		return t == totaltime ? max + min : max * (-powf(2, -10 * t / totaltime) + 1) + min;
	}
	float InOutExp(float t, float totaltime, float max, float min)
	{
		if (t == 0.0f)
			return min;
		if (t == totaltime)
			return max;
		max -= min;
		t /= totaltime;

		if (t / 2 < 1)
			return max / 2 * powf(2, 10 * (t - 1)) + min;
		--t;
		return max / 2 * (-powf(2, -10 * t) + 2) + min;

	}
	float InCirc(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		return -max * (sqrt(1 - t * t) - 1) + min;
	}
	float OutCirc(float t, float totaltime, float max, float min)
	{
		max -= min;
		t = t / totaltime - 1;
		return max * sqrt(1 - t * t) + min;
	}
	float InOutCirc(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		if (t / 2 < 1)
			return -max / 2 * (sqrt(1 - t * t) - 1) + min;
		t -= 2;
		return max / 2 * (sqrt(1 - t * t) + 1) + min;
	}
	float InBack(float t, float totaltime, float max, float min, float s)
	{
		max -= min;
		t /= totaltime;
		return max * t*t*((s + 1)*t - s) + min;
	}
	float OutBack(float t, float totaltime, float max, float min, float s)
	{
		max -= min;
		t = t / totaltime - 1;
		return max * (t*t*((s + 1)*t*s) + 1) + min;
	}
	float OutBounce(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;

		if (t < 1 / 2.75f)
			return max * (7.5625f*t*t) + min;
		else if (t < 2 / 2.75f)
		{
			t -= 1.5f / 2.75f;
			return max * (7.5625f*t*t + 0.75f) + min;
		}
		else if (t< 2.5f / 2.75f)
		{
			t -= 2.25f / 2.75f;
			return max * (7.5625f*t*t + 0.9375f) + min;
		}
		else
		{
			t -= 2.625f / 2.75f;
			return max * (7.5625f*t*t + 0.984375f) + min;
		}
	}
	float InBounce(float t, float totaltime, float max, float min)
	{
		return max - OutBounce(totaltime - t, totaltime, max - min, 0) + min;
	}
	float InOutBounce(float t, float totaltime, float max, float min)
	{
		if (t < totaltime / 2)
			return InBounce(t * 2, totaltime, max - min, max)*0.5f + min;
		else
			return OutBounce(t * 2 - totaltime, totaltime, max - min, 0)*0.5f + min + (max - min)*0.5f;
	}
	float Linear(float t, float totaltime, float max, float min)
	{
		return (max - min)*t / totaltime + min;
	}
}
