//====================================================
// @brief タンク型の敵用ビヘイビアツリー
//====================================================
#pragma once

class BehaviorTree;
class BehaviorTreeNode;
class TankEnemyBehaviorTree : public BehaviorTree
{
public:
	TankEnemyBehaviorTree();//コンストラクタ
	~TankEnemyBehaviorTree();//デストラクタ

	/*列挙体*/
	//アクションの種類
	enum class ActionType
	{
		DYING	 = 0, //デス
		IDLE	 = 1, //待機
		WALK	 = 2, //歩く
		ATTACK	 = 3, //弱攻撃
		REACTION = 4, //リアクション
	};
		  void Initialize()					 override;		//初期化
		  void Update	 (Character& _chara) override;		//更新
	const void Draw		 ()					 const override;//描画

private:
	/*内部処理関数*/
	void UpdateMemberVariables	(Character& _chara);//メンバ変数の更新
	void CreateBehaviorTree		();					//ビヘイビアツリーを作成
};