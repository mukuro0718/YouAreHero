//====================================================
// @brief ルクスリオ用ビヘイビアツリー
//====================================================
#pragma once
#include "Singleton.h"

class BehaviorTree;
class BehaviorTreeNode;
class DemonBehaviorTree : public BehaviorTree
{
public:
	 DemonBehaviorTree	  ();//コンストラクタ
	~DemonBehaviorTree	  ();//デストラクタ

	/*列挙体*/
	enum class ActionType
	{
		DYING				= 0, //デス
		IDLE				= 1, //待機
	};
		  void Initialize	()					override;		//初期化
		  void Update		(Character& _chara) override;		//更新
	const void Draw			()					const override;	//描画

	/*getter/setter*/
private:
	/*内部処理関数*/

	/*メンバ変数*/
	BehaviorTreeNode* action_Dying;	//demonアクション
	BehaviorTreeNode* action_Idle;	//demonアクション
};