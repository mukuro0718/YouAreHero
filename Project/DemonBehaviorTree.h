//====================================================
// @brief ルクスリオ用ビヘイビアツリー
//====================================================
#pragma once
#include "Singleton.h"

class BehaviorTreeNode;
class DemonBehaviorTree : public Singleton<DemonBehaviorTree>
{
public:
	friend class Singleton<DemonBehaviorTree>;

	/*列挙体*/
	enum class ActionType
	{
		DYING				= 0, //デス
		IDLE				= 1, //待機
	};
		  void Initialize	();		//初期化
		  void Update		();		//更新
	const void Draw			()const;//描画

	/*getter/setter*/
private:
	/*内部処理関数*/
		  DemonBehaviorTree	  ();//コンストラクタ
		 ~DemonBehaviorTree	  ();//デストラクタ

	/*メンバ変数*/
	BehaviorTreeNode* action_Dying;	//demonアクション
	BehaviorTreeNode* action_Idle;	//demonアクション
};