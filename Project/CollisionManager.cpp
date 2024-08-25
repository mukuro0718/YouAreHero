#include <DxLib.h>
#include "CollisionManager.h"
#include "UseMath.h"
#include "Vector4.h"
#include "Collider.h"
#include "PlayerManager.h"
#include "EnemyManager.h"

CollisionManager::CollisionManager()
	: isHit(false)
	, debugMax{ 0.0f,0.0f,0.0f }
	, debugMin{ 0.0f,0.0f,0.0f }
{

}

/// <summary>
/// 当たり判定
/// </summary>
void CollisionManager::HitCheck()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy  = Singleton<EnemyManager>::GetInstance();

	/*プレイヤーとボスとの当たり判定*/
	//bool isHit = HitCheck_AABB_Sphere(enemy.GetCollider(), player.GetCollider());
	bool isHit = HitCheck_OBB_Sphere(enemy.GetCollider(), player.GetCollider());
	this->isHit = isHit;
}

/// <summary>
/// AABBとSphereの当たり判定
/// </summary>
bool CollisionManager::HitCheck_AABB_Sphere(const Collider& _aabb, const Collider& _sphere)
{
	/*AABBとsphereの最近接点を求める*/
	float length = GetLengthAABBToPoint(_aabb, _sphere);

	/*最近接点が０ということは、sphereの中心座標がAABBの中にあるということなので早期リターン*/
	if (length == 0.0f)return true;

	/*最近接点がsphereの半径よりも長かったら当たっている*/
	if (length < _sphere.radius) return true;

	return false;
}

/// <summary>
/// OBBボックス当たり判定
/// </summary>
bool CollisionManager::HitCheck_OBB_Sphere(const Collider& _obb, const Collider& _sphere)
{
	float OBBToSphere = VSize(VSub(_obb.position, _sphere.position));

	/*OBBとsphereの最近接点を求める*/
	float length = GetLengthOBBToPoint(_obb, _sphere);

	/*最近接点が０ということは、sphereの中心座標がOBBの中にあるということなので早期リターン*/
	if (length == 0.0f)return true;

	/*最近接点がsphereの半径よりも長かったら当たっている*/
	if (length < _sphere.radius) return true;

	return false;
}

/// <summary>
/// AABBオブジェクトと1点の最近接点を求める
/// </summary>
float CollisionManager::GetLengthAABBToPoint(const Collider& _aabb, const Collider& _point)
{
	float SqLen = 0;   // 長さのべき乗の値を格納
	int i;
	for (i = 0; i < 3; i++)
	{
		// 各軸で点が最小値以下もしくは最大値以上ならば、差を考慮
		if (_point.GetAxisValue(i) < _aabb.GetMin(i))  // i=0はX、1はY、2はZの意味です
			SqLen += (_point.GetAxisValue(i) - _aabb.GetMin(i)) * (_point.GetAxisValue(i) - _aabb.GetMin(i));
		if (_point.GetAxisValue(i) > _aabb.GetMax(i))
			SqLen += (_point.GetAxisValue(i) - _aabb.GetMax(i)) * (_point.GetAxisValue(i) - _aabb.GetMax(i));
	}
	return static_cast<float>(sqrt(SqLen));
}

float CollisionManager::GetLengthOBBToPoint(const Collider& _obb, const Collider& _point)
{
	/*最終的に長さを求めるベクトル*/
	VECTOR vector = { 0.0f, 0.0f, 0.0f };

	/*各軸についてはみ出た部分のベクトルを算出*/
	for (int i = 0; i < 3; i++)
	{
		float length = _obb.GetLength(i);
		if (length <= 0) continue;  // L=0は計算できない

		VECTOR pointToObb = VSub(_obb.position, _point.position);
		VECTOR direction = _obb.GetDirection(i);
		float dot = VDot(pointToObb, direction);
		float size = dot / length;

		// sの値から、はみ出した部分があればそのベクトルを加算
		size = static_cast<float>(fabs(size));
		if (size > 1.0f)
		{
			//はみ出した部分のベクトル算出
			float scale = (1.0f - size) * length;
			vector = VAdd(vector, VScale(direction, scale));
		}
	}

	/*長さを出力*/
	return VSize(vector);
}

const void CollisionManager::DebugDrawHit()const
{
	DrawCube3D(debugMax, debugMin, GetColor(0, 0, 255), GetColor(0, 0, 255), FALSE);
	if (this->isHit)
	{
		//printfDx("HIT\n");
	}
	else
	{
		//printfDx("NOT_HIT\n");
	}
}
