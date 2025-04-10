//=====================================================
// @brief 両前足溜め攻撃アクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class AttackCapsuleColliderData;
class Beast_ChargeBothFootAttack : public ActionNode
{
public:
	 Beast_ChargeBothFootAttack();//コンストラクタ
	~Beast_ChargeBothFootAttack();//デストラクタ

	void		Initialize	();													//初期化
	NodeState	Update		(BehaviorTree& _tree, Character& _chara)override;	//更新
	const void	Draw		()const;											//描画
private:
	/*構造体*/
	//アニメーションの段階
	enum class AnimationStage
	{
		START,
		END,
	};

	/*メンバ変数*/
	AnimationStage							 stage;							 //アニメーションの段階
	std::map<AnimationStage, int>			 animationSet;					 //アニメーションのセット
	std::map<AnimationStage, AnimationStage> nextStageSet;					 //次のステージのセット
	short									 attackStartCount;				 //攻撃開始フレーム
	short									 attackEndCount;				 //攻撃終了フレーム
	short									 frameCount;					 //フレームカウント
	short									 frameIndexUsedCapsuleDirection1;//ブレスの方向を決めるためのフレーム番号
	short									 frameIndexUsedCapsuleDirection2;//ブレスの方向を決めるためのフレーム番号
	AttackCapsuleColliderData*				 collider;						 //攻撃コライダー

};

