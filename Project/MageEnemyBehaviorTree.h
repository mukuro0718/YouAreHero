//====================================================
// @brief 魔法型の敵用ビヘイビアツリー
//====================================================
#pragma once

class BehaviorTree;
class BehaviorTreeNode;
class MageEnemyBehaviorTree : public BehaviorTree
{
public:
	 MageEnemyBehaviorTree();//コンストラクタ
	~MageEnemyBehaviorTree();//デストラクタ

	/*列挙体*/
	//アクションの種類
	enum class ActionType
	{
		DYING	 = 0, //デス
		IDLE	 = 1, //待機
		RUN		 = 2, //走る
		WALK	 = 3, //歩く
		ATTACK	 = 4, //攻撃
		REACTION = 5, //リアクション
	};
		  void Initialize()					 override;		//初期化
		  void Update	 (Character& _chara) override;		//更新
	const void Draw		 ()					 const override;//描画

private:
	/*内部処理関数*/
	void UpdateMemberVariables	(Character& _chara);//メンバ変数の更新
	void CreateBehaviorTree		();					//ビヘイビアツリーを作成
};