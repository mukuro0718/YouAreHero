//=================================================
// @brief ボスマネージャークラス
//=================================================
#pragma once
#include "Singleton.h"

class Boss;
class Collider;
class EnemyManager : public Singleton<EnemyManager>
{
public:
	friend class Singleton<EnemyManager>;

	void Initialize();
	void Update();
	const void Draw()const;

	/*getter*/
	const VECTOR GetPosition()const;//座標の取得
	const Collider GetCollider();
private:
	/*内部処理関数*/
	EnemyManager();//コンストラクタ
	~EnemyManager();//デストラクタ

	/*メンバ変数*/
	Boss* boss;//ボス
};

