//====================================================
// @brief モルナクト用ビヘイビアツリー
//====================================================
#pragma once
#include "Singleton.h"

class BehaviorTreeNode;
class DragonBehaviorTree : public Singleton<DragonBehaviorTree>
{
public:
	friend class Singleton<DragonBehaviorTree>;

	/*列挙体*/
	//キャラクターの状態
	enum class DragonState
	{
		NORMAL = 0,//通常
		TIRED = 1,//スタミナが切れている状態
	};
	//アクションの種類
	enum class ActionType
	{
		DYING					  = 0, //デス
		IDLE					  = 1, //待機
		ROAR					  = 2, //咆哮(通常→怒り状態の変化)
		WALK					  = 3, //歩き
		TURN_LEFT				  = 4, //左を向く
		TURN_RIGHT				  = 5, //右を向く
		BREATH_SMASH			  = 6, //ブレス＋叩きつけ
		SMASH_ROTATE			  = 7, //叩きつけ＋回転攻撃
		SWEEP_SMASH				  = 8, //なぎ払い＋叩きつけ
		ROTATE_SWEEP			  = 9,//回転攻撃＋なぎ払い
		SWEEP_BREATH			  = 10,//なぎ払い＋ブレス
		BREATH_SMASH_ROTATE		  = 11,//ブレス＋叩きつけ＋回転攻撃
		ROTATE_SWEEP_SMASH		  = 12,//回転攻撃＋なぎ払い＋叩きつけ
		SWEEP_BREATH_SMASH		  = 13,//なぎ払い＋ブレス＋叩きつけ
		BREATH_SMASH_SWEEP_ROTATE = 14,//ブレス＋叩きつけ＋なぎ払い＋回転攻撃
		SWEEP_SMASH_ROTATE_BREATH = 15,//なぎ払い＋叩きつけ＋回転攻撃＋ブレス
		ROTATE_SWEEP_BREATH_SMASH = 16,//回転攻撃＋なぎ払い＋ブレス＋叩きつけ
	};
		  void Initialize	();		//初期化
		  void Update		();		//更新
	const void Draw			()const;//描画

	/*getter/setter*/
	const int	GetInterval			(const int _index)const { return this->intervalSet[_index]; }			 //インターバルの取得
	const float GetToTargetDistance	()const					{ return this->toTargetDistance; }				 //目標へのベクトルを取得
	const float GetInnerProduct		()const					{ return this->innerProductOfDirectionToTarget; }//目標へのベクトルの内積を取得
	const int	GetCurrentAction	()const					{ return this->currentAction; }					 //現在のアクションを取得
	const bool  GetIsActive			()const					{ return this->isActive; }
	void SetCurrentAction(const int _action)					{ this->currentAction = _action; }		//現在のアクションの設定
	void SetInterval	 (const int _index, const int _set = 0) { this->intervalSet[_index] = _set; }	//インターバルの設定
	void CalcStamina	 (const int _value)						{ this->stamina += _value; }			//スタミナの計算
private:
	/*内部処理関数*/
		  DragonBehaviorTree	();//コンストラクタ
		 ~DragonBehaviorTree	();//デストラクタ
	void UpdateMemberVariables	();//メンバ変数の更新

	/*メンバ変数*/
	std::array<int, static_cast<int>(ActionType::ROTATE_SWEEP_BREATH_SMASH) + 1>	intervalSet;					//インターバル
	BehaviorTreeNode*																Selector_DyingOrBattleOrIdle;	//ビヘイビアツリーのrootノード
	BehaviorTreeNode*																debugActionNode;				//デバック用のアクションノード(TODO:マスタ版では消す)
	DragonState																		state;							//ボスの状態
	float																			toTargetDistance;				//目標までの距離
	float																			innerProductOfDirectionToTarget;//向きベクトルと目標ベクトルの内積
	int																				currentAction;					//現在のアクション
	int																				stamina;						//スタミナ
	bool																			isActive;						//アクティブ状態か（スタミナが切れたら最大値になるまでfalse:ノンアクティブ状態になる。）
	BehaviorTreeNode::NodeState														prevNodeState;					//前のノードから帰ってきた状態
};