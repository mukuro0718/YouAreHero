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
	//アクションの種類
	enum class ActionType
	{
		DYING	= 0, //デス
		IDLE	= 1, //待機
		ROAR	= 2, //咆哮(通常→怒り状態の変化)
		WALK	= 3, //歩き
		SMASH	= 4, //叩きつけ
		SWEEP	= 5, //なぎ払い
		ROTATE	= 6, //回転攻撃
		BREATH	= 7, //ブレス
	};
		  void Initialize		();							//初期化
		  void Update			();							//更新
	const void Draw				()const;					//描画

	/*getter/setter*/
	const int		  GetInterval				 (const int _index)const { return this->intervalSet[_index]; }			  //インターバルの取得
	const float		  GetToTargetDistance		 ()const				 { return this->toTargetDistance; }				  //目標へのベクトルを取得
	const float		  GetInnerProduct			 ()const				 { return this->innerProductOfDirectionToTarget; }//目標へのベクトルの内積を取得
	const int		  GetCurrentAction			 ()const				 { return this->currentAction; }				  //現在のアクションを取得
	const bool		  GetIsSelectedBattleAction	 ()const				 { return this->isSelectedBattleAction; }		  //部位破壊されているか
	const bool		  GetIsCancelAction			 ()const				 { return this->isCancelAction; }				  //キャンセルフラグの取得
	const float		  GetDotOfDirAndToTarget	 ()const				 { return this->innerProductOfDirectionToTarget; }//内積の取得
	BehaviorTreeNode& GetBattleAction			 ()						 { return *this->currentBattleAction; }			  //現在実行中のバトルアクション
	void SetCurrentAction	(const int _action)						{ this->currentAction = _action; }		//現在のアクションの設定
	void SetInterval		(const int _index, const int _set = 0)	{ this->intervalSet[_index] = _set; }	//インターバルの設定
	void OnIsCancelAction	()										{ this->isCancelAction = true; }
	void OffIsCancelAction	()										{ this->isCancelAction = false; }
	void EntryCurrentBattleAction(BehaviorTreeNode& _action);	//バトルアクションの登録
	void ExitCurrentBattleAction();								//バトルアクションの解除
private:
	/*内部処理関数*/
		  DragonBehaviorTree	();//コンストラクタ
		 ~DragonBehaviorTree	();//デストラクタ
	void UpdateMemberVariables	();//メンバ変数の更新
	void CreateBehaviorTree		();//ビヘイビアツリーを作成

	/*メンバ変数*/
	std::array<int, static_cast<int>(ActionType::BREATH) + 1>	intervalSet;					//インターバル
	BehaviorTreeNode*											Selector_DyingOrBattleOrIdle;	//ビヘイビアツリーのrootノード
	BehaviorTreeNode*											debugActionNode;				//デバック用のアクションノード(TODO:マスタ版では消す)
	BehaviorTreeNode*											currentBattleAction;			//現在のバトルアクション
	BehaviorTreeNode::NodeState									prevNodeState;					//前のノードから帰ってきた状態
	float														toTargetDistance;				//目標までの距離
	float														innerProductOfDirectionToTarget;//向きベクトルと目標ベクトルの内積
	int															currentAction;					//現在のアクション
	int															attackCount;					//攻撃回数
	bool														isSelectedBattleAction;			//リアクションが選択されているか
	bool														isCancelAction;					//アクションキャンセルフラグ
};