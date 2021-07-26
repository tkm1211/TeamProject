#include "MathFunction.h"


DirectX::XMMATRIX ExtractOffset(const DirectX::XMMATRIX& mWorld)
{
	return DirectX::XMMatrixTranslation(mWorld.r[3].m128_f32[0], mWorld.r[3].m128_f32[1], mWorld.r[3].m128_f32[2]);
}
DirectX::XMMATRIX ExtractScaling(const DirectX::XMMATRIX& mWorld)
{
	return DirectX::XMMatrixScaling
	(
		DirectX::XMVector3Length(DirectX::XMVECTOR{ mWorld.r[0].m128_f32[0],mWorld.r[0].m128_f32[1],mWorld.r[0].m128_f32[2] }).m128_f32[0],
		DirectX::XMVector3Length(DirectX::XMVECTOR{ mWorld.r[1].m128_f32[0],mWorld.r[1].m128_f32[1],mWorld.r[1].m128_f32[2] }).m128_f32[0],
		DirectX::XMVector3Length(DirectX::XMVECTOR{ mWorld.r[2].m128_f32[0],mWorld.r[2].m128_f32[1],mWorld.r[2].m128_f32[2] }).m128_f32[0]
	);
}
// ワールド行列から回転成分のみを抽出する
DirectX::XMMATRIX ExtractRotation(const DirectX::XMMATRIX& mWorld)
{
	DirectX::XMMATRIX mOffset = ExtractOffset(mWorld);
	DirectX::XMMATRIX mScaling = ExtractScaling(mWorld);

	DirectX::XMVECTOR det;
	// 左からScaling、右からOffsetの逆行列をそれぞれかける。
	return DirectX::XMMatrixInverse(&det, mScaling) * mWorld * XMMatrixInverse(&det, mOffset);
}


// スクリーン座標をワールド座標に変換
DirectX::XMFLOAT3* CalcScreenToWorld
(DirectX::XMFLOAT3* pout,
	int screenX,				// スクリーンX座標
	int screenY,				// スクリーンY座標
	float fZ,					// 射影空間でのZ値（0～1）
	int screenW,
	int screenH,
	DirectX::XMMATRIX* view,
	DirectX::XMMATRIX* prj)
{
	// 各行列の逆行列を算出
	DirectX::XMMATRIX invView, invPrj, vp, invViewPort;
	DirectX::XMFLOAT4X4 vpFloat4x4;

	invView = DirectX::XMMatrixInverse(NULL, *view);
	invPrj = DirectX::XMMatrixInverse(NULL, *prj);

	vp = DirectX::XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&vpFloat4x4, vp);
	vpFloat4x4._11 = screenW / 2.0f; vpFloat4x4._22 = -screenH / 2.0f;
	vpFloat4x4._41 = screenW / 2.0f; vpFloat4x4._42 = screenH / 2.0f;
	vp = DirectX::XMLoadFloat4x4(&vpFloat4x4);
	invViewPort = DirectX::XMMatrixInverse(NULL, vp);

	// 逆変換
	DirectX::XMMATRIX tmp = invViewPort * invPrj * invView;
	DirectX::XMStoreFloat3(pout, DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(screenX, screenY, fZ, 1.0f), tmp));

	return pout;
}

DirectX::XMFLOAT3 SphereLinear // 戻り値 : 補間ベクトル
(
	DirectX::XMFLOAT3 originPos, // 原点
	DirectX::XMFLOAT3 oldPos,    // 前のフレームの座標
	DirectX::XMFLOAT3 nowPos,    // 今のフレームの座標
	float t					     // 補間値（0.0f ～ 1.0f）
)
{
	DirectX::XMVECTOR start, end;
	DirectX::XMFLOAT3 out;

	start = DirectX::XMVector3Normalize(DirectX::XMVectorSet(oldPos.x - originPos.x, oldPos.y - originPos.y, oldPos.z - originPos.z, 1.0f));
	end = DirectX::XMVector3Normalize(DirectX::XMVectorSet(nowPos.x - originPos.x, nowPos.y - originPos.y, nowPos.z - originPos.z, 1.0f));

	DirectX::XMFLOAT3 _start, _end;
	_start = DirectX::XMFLOAT3(oldPos.x - originPos.x, oldPos.y - originPos.y, oldPos.z - originPos.z);
	_end = DirectX::XMFLOAT3(nowPos.x - originPos.x, nowPos.y - originPos.y, nowPos.z - originPos.z);

	float angle = acosf(DirectX::XMVectorGetX(DirectX::XMVector3Dot(start, end)));
	float sinSita = sinf(angle);
	float startPoint = sinf(angle * (1 - t));
	float endPoint = sinf(angle * t);

	if (angle == 0.0f)
	{
		out.x = 0.0f;
		out.y = 0.0f;
		out.z = 0.0f;
	}
	else
	{
		DirectX::XMFLOAT3 startFloat3, endFloat3;
		DirectX::XMStoreFloat3(&startFloat3, start);
		DirectX::XMStoreFloat3(&endFloat3, end);

		float len = sqrtf(_start.x * _start.x + _start.y * _start.y + _start.z * _start.z);

		out.x = (startPoint * startFloat3.x + endPoint * endFloat3.x) / sinSita;
		out.y = (startPoint * startFloat3.y + endPoint * endFloat3.y) / sinSita;
		out.z = (startPoint * startFloat3.z + endPoint * endFloat3.z) / sinSita;
	}

	return out;

}