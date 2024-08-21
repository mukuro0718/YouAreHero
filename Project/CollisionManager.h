//========================================================
// @brief コリジョンマネージャークラス
// 衝突判定関数の実装と管理を行う
// http://marupeke296.com/COL_3D_No12_OBBvsPoint.html
// を参考にしました。
//========================================================
#pragma once
#include "Singleton.h"

class Collider;
class CollisionManager : public Singleton<CollisionManager>
{
public:
	CollisionManager();//コンストラクタ
	~CollisionManager(){}//デストラクタ

	//これを呼ぶことで、Singletonクラスで実装したインスタンスの取得を呼ぶことができる
	friend class Singleton<CollisionManager>;

	const void DebugDrawHit()const;

	void HitCheck();//当たり判定

	bool HitCheck_AABB_Sphere(const Collider& _aabb,const Collider& _sphere);//AABBボックス当たり判定
	bool HitCheck_OBB_Sphere(const Collider& _aabb, const Collider& _sphere);//OBBボックス当たり判定

private:
	/*内部処理関数*/
	float GetLengthAABBToPoint(const Collider& _aabb, const Collider& _point);//AABBオブジェクトと1点の最近接点を求める
	float GetLengthOBBToPoint(const Collider& _aabb, const Collider& _point);//OBBオブジェクトと1点の最近接点を求める


	/*メンバ変数*/
	bool isHit;//デバック時に当たったかどうかを表示（本来は使用しない）
	VECTOR debugMax;
	VECTOR debugMin;
};

