//====================================================
// @brief プレイヤーコントローラークラス
// プレイヤーの状態を定義し、行うべきアクションを決定する
//====================================================
#pragma once

class PlayerController
{
public:
	 PlayerController();
	~PlayerController();

	void Initialize();
	bool StateChanger(const bool _isCancelAction, const bool _isEndAction, const bool _isDrawSword, const CharacterData& _data, const int _healCount);
	const int GetNowState()const { return this->nowState; }
	const int GetPrevState()const { return this->prevState; }
	void SynchroState() { this->prevState = this->nowState; }
	enum class PlayerState
	{
		IDLE					= 0,
		AVOID					= 1,
		DEATH					= 2,
		BLOCK					= 3,
		STAGGER					= 4,//のけぞり
		BLOCK_STAGGER			= 5,//ガード時のけぞり
		KNOCK_DOWN				= 6,//ノックダウン
		KNOCK_UP				= 7,//ノックダウン
		HEAL					= 8,
		RUN						= 9,
		RUNNING_OUT_OF_STAMINA	= 10,
		WALK					= 11,
		COMBO_1					= 12,
		COMBO_2					= 13,
		COMBO_3					= 14,
		STRONG_ATTACK			= 15,
		DRAW_SWORD_1			= 16,
		DRAW_SWORD_2			= 17
	};
private:
	/*列挙体*/
	enum class ActionPriority
	{
		VERY_LOW,
		LOW,
		MEDIUM,
		HIGH,
		CRITICAL,
	};

	/*内部処理関数*/
	const bool CanAction(const float _stamina, const float _staminaConsumed)const;//アクションができるか

	/*メンバ変数*/
	vector<short>		priority;				 //アクションごとの優先順位
	map<short,list<short>> stateTheIsCancel;	 //キャンセルフラグの状態
	int					nowState;				 //現在の状態
	int					prevState;				 //前の状態
	float				blockStaminaConsumption; //防御時の消費スタミナ
	float				avoidStaminaConsumption; //回避時の消費スタミナ
	float				runStaminaConsumption;	 //ダッシュ時の消費スタミナ
	float strongAttackStaminaConsumption;//強攻撃消費スタミナ
	float weakAttackStaminaConsumption1;//弱攻撃消費スタミナ
	float weakAttackStaminaConsumption2;//弱攻撃消費スタミナ
	float weakAttackStaminaConsumption3;//弱攻撃消費スタミナ
};

