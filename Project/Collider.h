//========================================================
// @brief コライダークラス
// 各コライダークラスはこのクラスを継承する
//========================================================
#pragma once

class Collider
{
public:
	Collider();//コンストラクタ
	~Collider(){}//デストラクタ

	void SetSphere(const VECTOR _position, const float _radius);
	void SetAABB(const VECTOR _max, const VECTOR _min);
	void SetOBB(const VECTOR _max, const VECTOR _min, const VECTOR _position, const VECTOR _rotation);

	const void DrawHitSphere()const;
	const void DrawHitBox()const;

	/*getter*/
	const float GetMin(const int _axis)const;
	const float GetMax(const int _axis)const;
	const void GetAxisValue(const int _axis, float& _value1, float& _value2)const;
	const float GetAxisValue(const int _axis)const;
	const VECTOR GetDirection(const int _axis)const;
	const float GetLength(const int _axis)const;

	/*メンバ変数*/
	//Sphere
	VECTOR position;
	float radius;
	//AABB
	VECTOR max;
	VECTOR min;
	//OBB
	MATRIX rotation;

private:
	/*列挙体*/
	enum class Axis
	{
		X = 0,
		Y = 1,
		Z = 2,
	};
};

