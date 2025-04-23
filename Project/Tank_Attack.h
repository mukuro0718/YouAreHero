//=====================================================
// @brief 弱攻撃アクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class AttackCapsuleColliderData;
class Tank_Attack : public ActionNode
{
public:
	 Tank_Attack();//コンストラクタ
	~Tank_Attack();//デストラクタ

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//更新
	void	  Initialize()override;						//初期化
private:
	short						attackStartCount;				//攻撃開始フレーム
	short						attackEndCount;					//攻撃終了フレーム
	short						frameCount;						//フレームカウント
	AttackCapsuleColliderData*	collider;						//攻撃コライダー
	short						frameIndexUsedCapsuleDirection1;//攻撃カプセルを決めるためのフレーム番号
	short						frameIndexUsedCapsuleDirection2;//攻撃カプセルを決めるためのフレーム番号
	float						startAnimationPlayTime;			//アニメーションの最初の再生時間
};

