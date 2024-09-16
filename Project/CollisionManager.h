//===============================================================
// @brief 物理、衝突判定するオブジェクトを登録し、
// 物理移動、衝突を通知する
//===============================================================
#pragma once
#include "Singleton.h"

class ColliderData;
class CollisionManager : public Singleton<CollisionManager>
{
public:
	friend class Singleton<CollisionManager>;
	virtual ~CollisionManager(){}//デストラクタ


	void Entry(ColliderData& _data);//衝突物の登録
	void Exit(ColliderData& _data);//登録解除
	void Update();//更新(登録オブジェクトの物理移動、衝突通知)
private:
	static constexpr float GRAVITY = -0.01f;//重力
	static constexpr float MAX_GRAVITY_ACCEL = -0.15f;//最大重力加速度
	static constexpr int BEFORE_FIX_INFO_COLOR = 0x0000ff;//補正前情報色
	static constexpr int AIM_INFO_COLOR = 0x0000aa;//補正前予定情報色
		static constexpr int AFTER_FIX_INFO_COLOR = 0xff00ff;//補正後情報色
	static constexpr int DIV_NUM = 16;
	static constexpr int MAX_CHECK_COUNT = 1000;

	std::list<ColliderData*> collidables;//登録されたCollidableのリスト

	struct OnCollideInfo
	{
		ColliderData& owner;
		ColliderData& colider;
	};

	/*内部処理関数*/
	CollisionManager(){}//コンストラクタ
	std::vector<OnCollideInfo> CheckColide();
	bool IsCollide(ColliderData& _objectA, ColliderData& _objectB);
	//位置補正、決定
	void FixNextPosition(ColliderData& _primary, ColliderData& _secondary);
	void FixPosition();
};

