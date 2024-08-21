#include <DxLib.h>
#include "UseSTL.h"
#include "Transform.h"

/// <summary>
/// コンストラクタ
/// </summary>
Transform::Transform()
	: position{ 0.0f,0.0f,0.0f }
	, rotation{ 0.0f,0.0f,0.0f }
	, scale	  { 0.0f,0.0f,0.0f }
{
}

/// <summary>
/// 回転率の設定
/// </summary>
void Transform::SetRotation(const VECTOR _rotation)
{
	this->rotation.x = DegreeToRadian(_rotation.x);
	this->rotation.y = DegreeToRadian(_rotation.y);
	this->rotation.z = DegreeToRadian(_rotation.z);
}
void Transform::SetRotationOfRadian(const VECTOR _rotation)
{
	this->rotation.x = _rotation.x;
	this->rotation.y = _rotation.y;
	this->rotation.z = _rotation.z;
}
void Transform::SetRotation(const std::vector<float> _rotation)
{
	this->rotation.x = DegreeToRadian(_rotation[0]);
	this->rotation.y = DegreeToRadian(_rotation[1]);
	this->rotation.z = DegreeToRadian(_rotation[2]);
}
void Transform::SetRotation(const float _x, const float _y, const float _z)
{
	this->rotation.x = DegreeToRadian(_x);
	this->rotation.y = DegreeToRadian(_y);
	this->rotation.z = DegreeToRadian(_z);
}

/// <summary>
/// 度数をラジアンに変更
/// </summary>
const float Transform::DegreeToRadian(const float _degree)const
{
	return _degree * (DX_PI_F / 180.0f);
}
