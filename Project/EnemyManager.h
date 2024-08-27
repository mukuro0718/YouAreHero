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
		  void Update	 ();
		  void Action	 ();	 //アクション
	const void Draw		 ()const;

	/*getter*/
	const int	   GetHP()const;
	const VECTOR GetPosition()const;//座標の取得
	const Collider GetCharacterCollider();
	const Collider GetAttackCollider();
	void CalcDamage(const int _damage);
	const int GetDamage()const;
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

