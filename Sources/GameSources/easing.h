#ifndef INCLUDED_EASING
#define INCLUDED_EASING
#include <DirectXMath.h>
//******************************************************************************
//
//		Easing.h
//
//      Easing Function �����\
//      URL https://easings.net/ja
//
//      �����L�T�C�g�̃R�[�h���Q�l�ɂ��Ă��܂��idouble�^��S��float�^�ɕύX�ς݁j
//      URL https://qiita.com/hart_edsf/items/962ac03281b871dcc0df
//
//******************************************************************************

static float(*const ToRadian)(float) = DirectX::XMConvertToRadians;  // �p�x�����W�A����
static float(*const ToDegree)(float) = DirectX::XMConvertToDegrees;  // ���W�A�����p�x��

//�C�[�W���O�֐�
namespace easing
{
	float InQuad(float t, float totaltime, float max, float min);
	float OutQuad(float t, float totaltime, float max, float min);
	float InOutQuad(float t, float totaltime, float max, float min);
	float InCubic(float t, float totaltime, float max, float min);
	float OutCubic(float t, float totaltime, float max, float min);
	float InOutCubic(float t, float totaltime, float max, float min);
	float InQuart(float t, float totaltime, float max, float min);
	float OutQuart(float t, float totaltime, float max, float min);
	float InOutQuart(float t, float totaltime, float max, float min);
	float InQuint(float t, float totaltime, float max, float min);
	float OutQuint(float t, float totaltime, float max, float min);
	float InOutQuint(float t, float totaltime, float max, float min);
	float InSine(float t, float totaltime, float max, float min);
	float OutSine(float t, float totaltime, float max, float min);
	float InOutSine(float t, float totaltime, float max, float min);
	float InExp(float t, float totaltime, float max, float min);
	float OutExp(float t, float totaltime, float max, float min);
	float InOutExp(float t, float totaltime, float max, float min);
	float InCirc(float t, float totaltime, float max, float min);
	float OutCirc(float t, float totaltime, float max, float min);
	float InOutCirc(float t, float totaltime, float max, float min);
	float InBack(float t, float totaltime, float max, float min, float s);
	float OutBack(float t, float totaltime, float max, float min, float s);
	float OutBounce(float t, float totaltime, float max, float min);
	float InBounce(float t, float totaltime, float max, float min);
	float InOutBounce(float t, float totaltime, float max, float min);
	float Linear(float t, float totaltime, float max, float min);
}

#endif // !INCLUDED_EASING