//====================================================
// @brief モルナクト用ビヘイビアツリー
//====================================================
#pragma once

class BehaviorTree;
class BehaviorTreeNode;
class DragonBehaviorTree : public BehaviorTree
{
public:
	 DragonBehaviorTree	();//コンストラクタ
	~DragonBehaviorTree	();//デストラクタ

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
		  void Initialize		()					override;		//初期化
		  void Update			(Character& _chara) override;		//更新
	const void Draw				()					const override;	//描画

private:
	/*内部処理関数*/
	void UpdateMemberVariables	();//メンバ変数の更新
	void CreateBehaviorTree		();//ビヘイビアツリーを作成
};