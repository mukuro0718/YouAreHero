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
	const CharacterData& GetCharacterData()const;
	const Rigidbody&	 GetRigidbody	 ()const;
	const bool			 IsMove			 ()const;//移動したか
	const bool			 GetIsAttack	 ()const;//攻撃したか
	const int			 GetHP			 ()const;
	const float			 GetStamina		 ()const;
	const int			 GetModelHandle	 ()const;
private:
	/*内部処理関数*/
	PlayerManager();//コンストラクタ
	~PlayerManager();//デストラクタ

	/*メンバ変数*/
	Character* player;//プレイヤー
};

