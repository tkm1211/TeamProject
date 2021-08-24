#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>

#undef max
#undef min

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/string.hpp>


class OBJ3D
{
protected:
	DirectX::XMFLOAT3 pos;    // 中心座標
	DirectX::XMFLOAT3 speed;  // 速度
	DirectX::XMFLOAT3 scale;  // 中心座標からの半径
	DirectX::XMFLOAT3 angle;  // オイラー角
	DirectX::XMFLOAT4 color;  // 色
	DirectX::XMFLOAT3 adjust; // 描画位置補正
	bool exists;			  // 存在フラグ

public:
	OBJ3D() :
		pos(0.0f, 0.0f, 0.0f),
		speed(0.0f, 0.0f, 0.0f),
		scale(1.0f, 1.0f, 1.0f),
		angle(0.0f, 0.0f, 0.0f),
		color(1.0f, 1.0f, 1.0f, 1.0f),
		adjust(0.0f, 0.0f, 0.0f),
		exists(false)
	{}
	~OBJ3D() 
	{
	
	}

public:
	void Init()
	{
#if 1
		pos    = DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f );
		speed  = DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f );
		scale  = DirectX::XMFLOAT3( 1.0f, 1.0f, 1.0f );
		angle  = DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f );
		adjust = DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f );
		color  = DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
		exists = false;
#endif
	}
	void UnInit()
	{
		pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		speed = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
		angle = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		adjust = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		exists = false;
	}

public: // Get関数
	DirectX::XMFLOAT3 GetPos()    { return pos;		}
	DirectX::XMFLOAT3 GetSpeed()  { return speed;	}
	DirectX::XMFLOAT3 GetScale()  { return scale;	}
	DirectX::XMFLOAT3 GetAngle()  { return angle;	}
	DirectX::XMFLOAT4 GetColor()  { return color;	}
	DirectX::XMFLOAT3 GetAdjust() { return adjust;	}
	bool GetExists()			  { return exists;	}

	// ワールド変換行列の取得関数
	DirectX::XMMATRIX GetWorldMatrix();

public: // Set関数
	void SetPos( DirectX::XMFLOAT3 _pos )		{ pos = _pos;		 }
	void SetPosX( float _posX )					{ pos.x = _posX;	 }
	void SetPosY( float _posY )					{ pos.y = _posY;	 }
	void SetPosZ( float _posZ )					{ pos.z = _posZ;	 }
	void SetSpeed( DirectX::XMFLOAT3 _speed )	{ speed = _speed;	 }
	void SetScale( DirectX::XMFLOAT3 _scale )	{ scale = _scale;	 }
	void SetAngle( DirectX::XMFLOAT3 _angle )	{ angle = _angle;	 }
	void SetAngleY( float _angleY )				{ angle.y = _angleY; }
	void SetColor( DirectX::XMFLOAT4 _color )	{ color = _color;	 }
	void SetAdjust( DirectX::XMFLOAT3 _adjust ) { adjust = _adjust;  }
	void SetExists( bool _exists )				{ exists = _exists;  }
	void SetAllData
	(
		DirectX::XMFLOAT3 _pos,
		DirectX::XMFLOAT3 _speed,
		DirectX::XMFLOAT3 _scale,
		DirectX::XMFLOAT3 _angle,
		DirectX::XMFLOAT4 _color,
		DirectX::XMFLOAT3 _adjust,
		bool _exists
	)
	{
		pos = _pos;
		speed = _speed;
		scale = _scale;
		angle = _angle;
		color = _color;
		adjust = _adjust;
		exists = _exists;
	}

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(
			cereal::make_nvp("pos.x", pos.x),
			cereal::make_nvp("pos.y", pos.y),
			cereal::make_nvp("pos.z", pos.z),
			cereal::make_nvp("speed.x", speed.x),
			cereal::make_nvp("speed.y", speed.y),
			cereal::make_nvp("speed.z", speed.z),
			cereal::make_nvp("scale.x", scale.x),
			cereal::make_nvp("scale.y", scale.y),
			cereal::make_nvp("scale.z", scale.z),
			cereal::make_nvp("angle.x", angle.x),
			cereal::make_nvp("angle.y", angle.y),
			cereal::make_nvp("angle.z", angle.z),
			cereal::make_nvp("color.r", color.x),
			cereal::make_nvp("color.g", color.y),
			cereal::make_nvp("color.b", color.z),
			cereal::make_nvp("color.a", color.w),
			cereal::make_nvp("adjust.x", adjust.x),
			cereal::make_nvp("adjust.y", adjust.y),
			cereal::make_nvp("adjust.z", adjust.z)
		);
	}
};


class OBJ3DInstance
{
protected:
	DirectX::XMFLOAT3 pos;    // 中心座標
	DirectX::XMFLOAT3 speed;  // 速度
	DirectX::XMFLOAT3 scale;  // 中心座標からの半径
	DirectX::XMFLOAT3 angle;  // オイラー角
	DirectX::XMFLOAT4 color;  // 色
	DirectX::XMFLOAT3 adjust; // 描画位置補正
	bool exists;			  // 存在フラグ
	float animationTick;	  // アニメーション用カウント
	bool isAnimation;		  // アニメーション中
	bool isLoopAnimation;	  // アニメーションループ

public:
	OBJ3DInstance() :
		pos(0.0f, 0.0f, 0.0f),
		speed(0.0f, 0.0f, 0.0f),
		scale(1.0f, 1.0f, 1.0f),
		angle(0.0f, 0.0f, 0.0f),
		color(1.0f, 1.0f, 1.0f, 1.0f),
		adjust(0.0f, 0.0f, 0.0f),
		exists(false),
		animationTick(0.0f),
		isAnimation(false),
		isLoopAnimation(false)
	{}
	~OBJ3DInstance() {}

public:
	void Init()
	{
		pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		speed = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
		angle = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		adjust = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		exists = false;
		animationTick = 0.0f;
		isAnimation = false;
		isLoopAnimation = false;
	}
	void UnInit()
	{
		pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		speed = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
		angle = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		adjust = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		exists = false;
		animationTick = 0.0f;
		isAnimation = false;
		isLoopAnimation = false;
	}

public: // Get関数
	DirectX::XMFLOAT3 GetPos() { return pos; }
	DirectX::XMFLOAT3 GetSpeed() { return speed; }
	DirectX::XMFLOAT3 GetScale() { return scale; }
	DirectX::XMFLOAT3 GetAngle() { return angle; }
	DirectX::XMFLOAT4 GetColor() { return color; }
	DirectX::XMFLOAT3 GetAdjust() { return adjust; }
	bool GetExists() { return exists; }
	float GetAnimationTick() { return animationTick; }
	bool GetIsAnimation() { return isAnimation; }
	bool GetIsLoopAnimation() { return isLoopAnimation; }

	// ワールド変換行列の取得関数
	DirectX::XMMATRIX GetWorldMatrix();

public: // Set関数
	void SetPos(DirectX::XMFLOAT3 _pos) { pos = _pos; }
	void SetPosX(float _posX) { pos.x = _posX; }
	void SetPosY(float _posY) { pos.y = _posY; }
	void SetPosZ(float _posZ) { pos.z = _posZ; }
	void SetSpeed(DirectX::XMFLOAT3 _speed) { speed = _speed; }
	void SetScale(DirectX::XMFLOAT3 _scale) { scale = _scale; }
	void SetAngle(DirectX::XMFLOAT3 _angle) { angle = _angle; }
	void SetAngleY(float _angleY) { angle.y = _angleY; }
	void SetColor(DirectX::XMFLOAT4 _color) { color = _color; }
	void SetAdjust(DirectX::XMFLOAT3 _adjust) { adjust = _adjust; }
	void SetExists(bool _exists) { exists = _exists; }
	void SetAnimationTick(float _animationTick) { animationTick = _animationTick; }
	void SetIsAnimation(bool _isAnimation) { isAnimation = _isAnimation; }
	void SetIsLoopAnimation(bool _isLoopAnimation) { isLoopAnimation = _isLoopAnimation; }
	void SetAllData
	(
		DirectX::XMFLOAT3 _pos,
		DirectX::XMFLOAT3 _speed,
		DirectX::XMFLOAT3 _scale,
		DirectX::XMFLOAT3 _angle,
		DirectX::XMFLOAT4 _color,
		DirectX::XMFLOAT3 _adjust,
		bool _exists
	)
	{
		pos = _pos;
		speed = _speed;
		scale = _scale;
		angle = _angle;
		color = _color;
		adjust = _adjust;
		exists = _exists;
	}

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(
			cereal::make_nvp("pos.x", pos.x),
			cereal::make_nvp("pos.y", pos.y),
			cereal::make_nvp("pos.z", pos.z),
			cereal::make_nvp("speed.x", speed.x),
			cereal::make_nvp("speed.y", speed.y),
			cereal::make_nvp("speed.z", speed.z),
			cereal::make_nvp("scale.x", scale.x),
			cereal::make_nvp("scale.y", scale.y),
			cereal::make_nvp("scale.z", scale.z),
			cereal::make_nvp("angle.x", angle.x),
			cereal::make_nvp("angle.y", angle.y),
			cereal::make_nvp("angle.z", angle.z),
			cereal::make_nvp("color.r", color.x),
			cereal::make_nvp("color.g", color.y),
			cereal::make_nvp("color.b", color.z),
			cereal::make_nvp("color.a", color.w),
			cereal::make_nvp("adjust.x", adjust.x),
			cereal::make_nvp("adjust.y", adjust.y),
			cereal::make_nvp("adjust.z", adjust.z)
		);
	}
};