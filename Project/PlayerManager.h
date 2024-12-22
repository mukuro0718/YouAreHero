//=================================================
// @brief プレイヤーマネージャークラス
//=================================================
#pragma once
#include "Singleton.h"

class Character;
class CharacterData;
class Rigidbody;
class PlayerManager : public Singleton<PlayerManager>
{
public:
	friend class Singleton<PlayerManager>;

		  void Initialize(); //初期化
		  void Finalize  ();
		  void Update	 (); //更新
	const void Draw		 ()const;					   //描画

	/*getter*/
	const int			 GetNowState	 ()const;
	const CharacterData& GetCharacterData()const;//キャラクターデータの取得
	const Rigidbody&	 GetRigidbody	 ()const;//リジッドボディの取得
	const int			 GetHP			 ()const;//HPの取得
	const float			 GetStamina		 ()const;//スタミナの取得
	const int			 GetHealOrbNum	 ()const;//回復回数の取得
	const int			 GetModelHandle	 ()const;//モデルハンドルの取得
	const bool			 GetIsAlive		 ()const;//生存フラグの取得
	const bool			 GetIsLockOn	 ()const;//ロックオンフラグの取得
	const bool			 GetIsDrawSword	 ()const;//抜刀フラグの取得
private:
	/*内部処理関数*/
	PlayerManager();//コンストラクタ
	~PlayerManager();//デストラクタ

	/*メンバ変数*/
	Character* player;//プレイヤー
};

