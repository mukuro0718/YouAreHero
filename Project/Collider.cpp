#include <DxLib.h>
#include "Vector4.h"
#include "Collider.h"

/// <summary>
/// コンストラクタ
/// </summary>
Collider::Collider()
	: position{ 0.0f,0.0f,0.0f }
	, radius  (0.0f)
	, max	  { 0.0f,0.0f,0.0f }
	, min	  { 0.0f,0.0f,0.0f }
	, rotation{ 0.0f,0.0f,0.0f }
{

}

/// <summary>
/// 情報のセット
/// </summary>
void Collider::SetSphere(const VECTOR _position, const float _radius)
{
	this->position = _position;
	this->radius = _radius;
}
void Collider::SetAABB(const VECTOR _max, const VECTOR _min)
{
	this->max = _max;
	this->min = _min;
	this->rotation = { 0.0f,0.0f,0.0f };
}
void Collider::SetOBB(const VECTOR _max, const VECTOR _min, const VECTOR _center,const VECTOR _rotation)
{
	this->position = _center;
	float xRotation = _rotation.x * 180.0f / DX_PI_F;
	float yRotation = _rotation.y * 180.0f / DX_PI_F;
	float zRotation = _rotation.z * 180.0f / DX_PI_F;
	this->rotation = MMult(MGetRotX(xRotation), MGetRotY(yRotation));
	this->rotation = MMult(rotation, MGetRotZ(zRotation));

	this->max = _max;
	this->min = _min;
}

/// <summary>
/// 当たり判定の描画
/// </summary>
/// <returns></returns>
const void Collider::DrawHitSphere()const
{
	DrawSphere3D(this->position, this->radius, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
}
const void Collider::DrawHitBox()const
{
	VECTOR position[8];

	position[0] = VSub(VGet(this->max.x, this->max.y, this->max.z), this->position);
	position[1] = VSub(VGet(this->max.x, this->min.y, this->max.z), this->position);
	position[2] = VSub(VGet(this->min.x, this->min.y, this->max.z), this->position);
	position[3] = VSub(VGet(this->min.x, this->max.y, this->max.z), this->position);
	position[4] = VSub(VGet(this->max.x, this->max.y, this->min.z), this->position);
	position[5] = VSub(VGet(this->max.x, this->min.y, this->min.z), this->position);
	position[6] = VSub(VGet(this->min.x, this->min.y, this->min.z), this->position);
	position[7] = VSub(VGet(this->min.x, this->max.y, this->min.z), this->position);

	for (int i = 0; i < 8; i++)
	{
		position[i] = VTransform(position[i], this->rotation);
		position[i] = VAdd(position[i], this->position);
	}

	DrawLine3D(position[0], position[1], GetColor(0, 255, 0));
	DrawLine3D(position[0], position[3], GetColor(0, 255, 0));
	DrawLine3D(position[2], position[1], GetColor(0, 255, 0));
	DrawLine3D(position[2], position[3], GetColor(0, 255, 0));
	DrawLine3D(position[4], position[5], GetColor(0, 255, 0));
	DrawLine3D(position[4], position[7], GetColor(0, 255, 0));
	DrawLine3D(position[6], position[5], GetColor(0, 255, 0));
	DrawLine3D(position[6], position[7], GetColor(0, 255, 0));
	DrawLine3D(position[0], position[4], GetColor(0, 255, 0));
	DrawLine3D(position[1], position[5], GetColor(0, 255, 0));
	DrawLine3D(position[2], position[6], GetColor(0, 255, 0));
	DrawLine3D(position[3], position[7], GetColor(0, 255, 0));
}

/// <summary>
/// 最小値の取得
/// </summary>
const float Collider::GetMin(const int _axis)const
{
	/*判定する値の変数*/
	float value1, value2;

	/*値の取得*/
	GetAxisValue(_axis, value1, value2);
	
	/*最小値の判定*/
	if (value1 > value2)
	{
		return value2;
	}
	return value1;
}
/// <summary>
/// 最大値の取得
/// </summary>
const float Collider::GetMax(const int _axis)const
{
	/*判定する値の変数*/
	float value1, value2;

	/*値の取得*/
	GetAxisValue(_axis, value1, value2);
	
	/*最大値の判定*/
	if (value1 > value2)
	{
		return value1;
	}
	return value2;
}

/// <summary>
/// 軸ごとの値の取得
/// </summary>
const void Collider::GetAxisValue(const int _axis, float& _value1, float& _value2)const
{
	switch (_axis)
	{
	case static_cast<int>(Axis::X):
		_value1 = this->max.x;
		_value2 = this->min.x;
		break;
	case static_cast<int>(Axis::Y):
		_value1 = this->max.y;
		_value2 = this->min.y;
		break;
	case static_cast<int>(Axis::Z):
		_value1 = this->max.z;
		_value2 = this->min.z;
		break;
	default:
		_value1 = 0.0f;
		_value2 = 0.0f;
		break;
	}
}
const float Collider::GetAxisValue(const int _axis)const
{
	float out;
	switch (_axis)
	{
	case static_cast<int>(Axis::X):
		out = this->position.x;
		break;
	case static_cast<int>(Axis::Y):
		out = this->position.y;
		break;
	case static_cast<int>(Axis::Z):
		out = this->position.z;
		break;
	default:
		out = 0.0f;
		break;
	}
	return out;
}

/// <summary>
/// 最大から最小へのベクトル
/// </summary>
const VECTOR Collider::GetDirection(const int _axis)const
{
	VECTOR vector1, vector2;

	vector1 = this->min;
	switch (_axis)
	{
	case static_cast<int>(Axis::X):
		vector2 = VGet(this->max.x, this->min.y, this->min.z);
		break;
	case static_cast<int>(Axis::Y):
		vector2 = VGet(this->min.x, this->max.y, this->min.z);
			break;
	case static_cast<int>(Axis::Z):
		vector2 = VGet(this->min.x, this->min.y, this->max.z);
			break;
	}
	return VNorm(VTransform(VSub(vector2, vector1), this->rotation));
}

/// <summary>
/// 最大から最小へのベクトルの長さ
/// </summary>
const float Collider::GetLength(const int _axis)const
{
	VECTOR vector1, vector2;

	vector1 = this->max;
	switch (_axis)
	{
	case static_cast<int>(Axis::X):
		vector2 = VGet(this->min.x, this->max.y, this->max.z);
		break;
	case static_cast<int>(Axis::Y):
		vector2 = VGet(this->max.x, this->min.y, this->max.z);
		break;
	case static_cast<int>(Axis::Z):
		vector2 = VGet(this->max.x, this->max.y, this->min.z);
		break;
	}
	return VSize(VSub(vector2, vector1)) * 0.5f;
}
