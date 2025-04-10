//=====================================================
// @brief 右足攻撃アクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class AttackCapsuleColliderData;
class Beast_RightFootAttack : public ActionNode
{
public:
	 Beast_RightFootAttack();//コンストラクタ
	~Beast_RightFootAttack();//デストラクタ

	void		Initialize	();													//初期化
	NodeState	Update		(BehaviorTree& _tree, Character& _chara)override;	//更新
	const void	Draw		()const;											//描画
private:
	short						attackStartCount;				//攻撃開始フレーム
	short						attackEndCount;					//攻撃終了フレーム
	short						frameCount;						//フレームカウント
	short						frameIndexUsedCapsuleDirection1;//カプセルを決めるためのフレーム番号
	short						frameIndexUsedCapsuleDirection2;//カプセルを決めるためのフレーム番号
	AttackCapsuleColliderData* collider;						//攻撃コライダー
};

