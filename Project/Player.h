//===============================================
// @brief プレイヤークラス
//===============================================
#pragma once

class PlayerController;
class Character;
class HitStop;
class PlayerAction;
class Player : public Character
{
public:
	Player();//コンストラクタ
	~Player();//デストラクタ

	void		Initialize		 () override;		//初期化
	void		Finalize		 () override;		//後処理
	void		Update			 () override;		//更新
	const void	DrawCharacterInfo()const override;	//描画
	void		PlayAnimation	 (const int _nextAnimation, const float _playTime);
	void		DeathProcess	 ();
	const bool	CanAction		 (const float _staminaConsumed)const;//アクションができるか
	void		CalcStamina		 (const float _staminaConsumed, const float _maxStamina);			//スタミナの回復処理

	/*getter*/
	const int		GetNowState			()const;
	const bool		GetIsAttack			()const override { return false; }												//攻撃フラグの取得
	const float		GetStamina			()const;																		//スタミナの取得
	const int		GetHealCount		()const { return this->healCount; }												//回復オーブの数(今は回復回数になっている)
	CharacterData&	GetPlayerData		();																				//プレイヤーデータの取得
	Rigidbody&		GetPlayerRigidbody	();																				//リジッドボディの取得
	const bool		GetIsDrawSword		()const { return this->isDrawSword; }											//武器を出しているか
		  void		SetHealCount		(const int _count) { this->healCount = _count; }								//回復回数の設定
		  void		SetHitStop			(const int _time, const int _type, const int _delay, const float _factor);		//ヒットストップの設定
		  void		SetIsDrawSword		(const bool _set) { this->isDrawSword = _set; }									//抜刀フラグの設定
	const bool		GetIsLock			() { return this->isLock; }//ロックオンフラグ
	//アニメーションの種類
	enum class AnimationType
	{
		IDLE			= 0,
		AVOID			= 1,
		DEATH			= 2,
		BLOCK			= 3,
		REACTION		= 4,
		BLOCK_REACTION	= 5,
		WALK_FRONT		= 6,
		WALK_BACK		= 7,
		WALK_LEFT		= 8,
		WALK_RIGHT		= 9,
		RUN_FRONT		= 10,
		RUN_BACK		= 11,
		RUN_LEFT		= 12,
		RUN_RIGHT		= 13,
		RUN_180_TURN	= 14,
		COMBO_1			= 15,
		HEAL			= 16,
		COMBO_2			= 17,
		COMBO_3			= 18,
		SKILL			= 19,
		DOWN_REACTION	= 20,
		DOWN_UP			= 21,
		DRAW_SWORD_1	= 22,
		DRAW_SWORD_2	= 23,
	};

private:
	/*静的定数*/
	static constexpr float	SHADOW_HEIGHT	= 10.0f;//影を投影する高さ
	static constexpr float	SHADOW_SIZE		= 6.0f;	//かげのサイズ
	
	/*メンバ変数*/
	PlayerController*		controller;		//コントローラー
	vector<PlayerAction*>	action;			//アクション
	HitStop*				hitStop;		//ヒットストップ
	int						healCount;		//回復可能数
	bool					isDrawSword;	//剣を抜いているか
	int						frameTime;		//処理までにかかった時間
	bool isLock;
	bool isPrevPushLS;
};

