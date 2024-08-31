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

	void Initialize(GoriLib::Physics* _physics); //初期化
	void Finalize(GoriLib::Physics* _physics);
	void Update(GoriLib::Physics* _physics); //更新
	void OnCollide(const GoriLib::Collidable& _colider); //移動ベクトルの修正
	const void Draw		 ()const;

	/*getter*/
	const int	   GetHP()const;
	const VECTOR GetPosition()const;//座標の取得
	const VECTOR GetHeadPosition()const;//座標の取得
	const int GetAttackNumber()const;
	const int GetHitNumber()const;
	void SetHitNumber(const int _attackNumber);
	const bool IsAttack()const;
	void OnIsHitAttack();
private:
	/*内部処理関数*/
	EnemyManager();//コンストラクタ
	~EnemyManager();//デストラクタ

	/*メンバ変数*/
	Boss* boss;//ボス
};

