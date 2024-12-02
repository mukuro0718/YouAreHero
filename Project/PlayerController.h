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
	void StateChanger(const bool _isEndAction, const CharacterData& _data);
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
		HEAL					= 7,
		RUN						= 8,
		RUNNING_OUT_OF_STAMINA	= 9,
		WALK					= 10,
		COMBO_1					= 11,
		COMBO_2					= 12,
		COMBO_3					= 13,
		STRONG_ATTACK			= 14,
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
	int nowState;
	int prevState;
	vector<int> priority;//アクションごとの優先順位
};

