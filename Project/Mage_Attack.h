//=====================================================
// @brief 攻撃アクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class AttackCapsuleColliderData;
class Mage_Attack : public ActionNode
{
public:
	 Mage_Attack();//コンストラクタ
	~Mage_Attack();//デストラクタ

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//更新
	void	  Initialize()override;						//初期化
private:
	short						attackStartCount;				//攻撃開始フレーム
	short						attackEndCount;					//攻撃終了フレーム
	short						frameCount;						//フレームカウント
	AttackCapsuleColliderData*  collider;						//攻撃コライダー
	short						frameIndexUsedCapsuleDirection1;//攻撃カプセルを決めるためのフレーム番号
	short						frameIndexUsedCapsuleDirection2;//攻撃カプセルを決めるためのフレーム番号
	float						startAnimationPlayTime;			//アニメーションの最初の再生時間
};

