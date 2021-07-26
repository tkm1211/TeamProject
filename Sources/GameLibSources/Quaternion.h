#pragma once

/*
///////////////////////////////////////////////
// クォータニオン→回転行列変換
//
// m11-m33 : 回転行列成分（出力）
// qx, qy, qz, qw : クォータニオン成分
//
// ※注意：
// 行列成分はDirectX形式（行方向が軸の向き）です
// OpenGL形式（列方向が軸の向き）の場合は
// 転置した値を格納するようにして下さい。

void transformQuaternionToRotMat(
	float& m11, float& m12, float& m13,
	float& m21, float& m22, float& m23,
	float& m31, float& m32, float& m33,
	float qx, float qy, float qz, float qw
) {
	m11 = 1.0f - 2.0f * qy * qy - 2.0f * qz * qz;
	m12 = 2.0f * qx * qy + 2.0f * qw * qz;
	m13 = 2.0f * qx * qz - 2.0f * qw * qy;

	m21 = 2.0f * qx * qy - 2.0f * qw * qz;
	m22 = 1.0f - 2.0f * qx * qx - 2.0f * qz * qz;
	m23 = 2.0f * qy * qz + 2.0f * qw * qx;

	m31 = 2.0f * qx * qz + 2.0f * qw * qy;
	m32 = 2.0f * qy * qz - 2.0f * qw * qx;
	m33 = 1.0f - 2.0f * qx * qx - 2.0f * qy * qy;
}


///////////////////////////////////////////////
// クォータニオン→回転行列変換
//
// m1-m3 : 回転行列成分（出力）
// q : クォータニオン成分(x,y,z,w)
//
// ※注意：
// 行列成分はDirectX形式（行方向が軸の向き）です
// OpenGL形式（列方向が軸の向き）の場合は
// 転置した値を格納するようにして下さい。

void transformQuaternionToRotMat(
	float m1[3],
	float m2[3],
	float m3[3],
	const float q[4]
) {
	transformQuaternionToRotMat(
		m1[0], m1[1], m1[2],
		m2[0], m2[1], m2[2],
		m3[0], m3[1], m3[2],
		q[0], q[1], q[2], q[3]
	);
}


///////////////////////////////////////////////
// クォータニオン→回転行列変換
//
// m : 回転行列成分（出力）
// q : クォータニオン成分(x, y, z, w)
//
// ※注意：
// 行列成分はDirectX形式（行方向が軸の向き）です
// OpenGL形式（列方向が軸の向き）の場合は
// 転置した値を格納するようにして下さい。

void transformQuaternionToRotMat(
	float m[16],
	const float q[4]
) {
	memset(m, 0, sizeof(float) * 16);
	transformQuaternionToRotMat(
		m[0], m[1], m[2],
		m[4], m[5], m[6],
		m[8], m[9], m[10],
		q[0], q[1], q[2], q[3]
	);
	m[15] = 1.0f;
}


///////////////////////////////////////////////
// 回転行列→クォータニオン変換
//
// qx, qy, qz, qw : クォータニオン成分（出力）
// m11-m33 : 回転行列成分
//
// ※注意：
// 行列成分はDirectX形式（行方向が軸の向き）です
// OpenGL形式（列方向が軸の向き）の場合は
// 転置した値を入れて下さい。

bool transformRotMatToQuaternion(
	float& qx, float& qy, float& qz, float& qw,
	float m11, float m12, float m13,
	float m21, float m22, float m23,
	float m31, float m32, float m33
) {
	// 最大成分を検索
	float elem[4]; // 0:x, 1:y, 2:z, 3:w
	elem[0] = m11 - m22 - m33 + 1.0f;
	elem[1] = -m11 + m22 - m33 + 1.0f;
	elem[2] = -m11 - m22 + m33 + 1.0f;
	elem[3] = m11 + m22 + m33 + 1.0f;

	unsigned biggestIndex = 0;
	for (int i = 1; i < 4; i++) {
		if (elem[i] > elem[biggestIndex])
			biggestIndex = i;
	}

	if (elem[biggestIndex] < 0.0f)
		return false; // 引数の行列に間違いあり！

	// 最大要素の値を算出
	float* q[4] = { &qx, &qy, &qz, &qw };
	float v = sqrtf(elem[biggestIndex]) * 0.5f;
	*q[biggestIndex] = v;
	float mult = 0.25f / v;

	switch (biggestIndex) {
	case 0: // x
		*q[1] = (m12 + m21) * mult;
		*q[2] = (m31 + m13) * mult;
		*q[3] = (m23 - m32) * mult;
		break;
	case 1: // y
		*q[0] = (m12 + m21) * mult;
		*q[2] = (m23 + m32) * mult;
		*q[3] = (m31 - m13) * mult;
		break;
	case 2: // z
		*q[0] = (m31 + m13) * mult;
		*q[1] = (m23 + m32) * mult;
		*q[3] = (m12 - m21) * mult;
		break;
	case 3: // w
		*q[0] = (m23 - m32) * mult;
		*q[1] = (m31 - m13) * mult;
		*q[2] = (m12 - m21) * mult;
		break;
	}

	return true;
}

///////////////////////////////////////////////
// 回転行列→クォータニオン変換
//
// qx, qy, qz, qw : クォータニオン成分（出力）
// m1[3] : 回転行列成分 m11 - m13
// m2[3] : 回転行列成分 m21 - m23
// m3[3] : 回転行列成分 m31 - m33
//
// ※注意：
// 行列成分はDirectX形式（行方向が軸の向き）です
// OpenGL形式（列方向が軸の向き）の場合は
// 転置した値を入れて下さい。

bool transformRotMatToQuaternion(
	float q[4],
	const float m1[3],
	const float m2[3],
	const float m3[3]
) {
	return transformRotMatToQuaternion(
		q[0], q[1], q[2], q[3],
		m1[0], m1[1], m1[2],
		m2[0], m2[1], m2[2],
		m3[0], m3[1], m3[2]
	);
}


// クォータニオンを行列に変換
// lpM <= lpQ

void FUTL_QuatToMatrix(DirectX::XMFLOAT4X4* lpM, DirectX::XMFLOAT4* lpQ)
{
	float qw, qx, qy, qz;
	float x2, y2, z2;
	float xy, yz, zx;
	float wx, wy, wz;

	qw = lpQ->w; qx = lpQ->x; qy = lpQ->y; qz = lpQ->z;

	x2 = 2.0f * qx * qx;
	y2 = 2.0f * qy * qy;
	z2 = 2.0f * qz * qz;

	xy = 2.0f * qx * qy;
	yz = 2.0f * qy * qz;
	zx = 2.0f * qz * qx;

	wx = 2.0f * qw * qx;
	wy = 2.0f * qw * qy;
	wz = 2.0f * qw * qz;

	lpM->_11 = 1.0f - y2 - z2;
	lpM->_11 = xy - wz;
	lpM->_12 = zx + wy;
	lpM->_13 = 0.0f;

	lpM->_21 = xy + wz;
	lpM->_22 = 1.0f - z2 - x2;
	lpM->_23 = yz - wx;
	lpM->_24 = 0.0f;

	lpM->_31 = zx - wy;
	lpM->_32 = yz + wx;
	lpM->_33 = 1.0f - x2 - y2;
	lpM->_34 = 0.0f;

	lpM->_41 = lpM->_42 = lpM->_43 = 0.0f;
	lpM->_44 = 1.0f;
}
*/