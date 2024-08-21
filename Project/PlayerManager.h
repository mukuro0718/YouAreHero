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
	const void Draw		 ()const;//描画

	/*getter*/
	const VECTOR   GetPosition			()const;//座標の取得
	const VECTOR   GetDirection			()const;//座標の取得
	const bool     IsMove				()const;//移動したか
	const bool	   IsAttack				()const;//攻撃したか
	const bool	   IsShowElementWheel	()const;//属性ホイールを表示したか
	const Collider GetCollider			();
	const int	   GetHP				()const;
	const int	   GetCurrentElement	()const;
	const float GetElementAngle()const;

private:
	/*内部処理関数*/
	PlayerManager();//コンストラクタ
	~PlayerManager();//デストラクタ

	/*メンバ変数*/
	Player* player;//プレイヤー
};

