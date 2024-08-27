//=================================================
// @brief プレイヤーマネージャークラス
//=================================================
#pragma once
#include "Singleton.h"

class Player;
class Collider;
class PlayerManager : public Singleton<PlayerManager>
{
public:
	friend class Singleton<PlayerManager>;

		  void Initialize();	 //初期化
		  void Update	 ();	 //更新
		  void Action	 ();	 //アクション
		  void		FixMoveVector(const VECTOR _fixVector);				  //移動ベクトルの修正
		  const void Draw		 ()const;//描画

	/*getter*/
		  void CalcDamage(const int _damage);
	const int GetDamage()const;
	const VECTOR   GetPosition			()const;//座標の取得
	const VECTOR   GetDirection			()const;//座標の取得
	const bool     IsMove				()const;//移動したか
	const bool	   IsAttack				()const;//攻撃したか
	const Collider GetCharacterCollider();
	const Collider GetAttackCollider();
	const int	   GetHP				()const;
	const int GetAttackNumber()const;
	const int GetHitNumber()const;
	void SetHitNumber(const int _attackNumber);

private:
	/*内部処理関数*/
	PlayerManager();//コンストラクタ
	~PlayerManager();//デストラクタ

	/*メンバ変数*/
	Player* player;//プレイヤー
};

