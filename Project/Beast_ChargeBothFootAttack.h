//=====================================================
// @brief 両前足溜め攻撃アクション
//=====================================================
#pragma once

class ActionNode;
class Beast_ChargeBothFootAttack : public ActionNode
{
public:
	 Beast_ChargeBothFootAttack();//コンストラクタ
	~Beast_ChargeBothFootAttack();//デストラクタ

	NodeState Update()override;//更新
private:
	/*構造体*/
	//アニメーションの段階
	enum class AnimationStage
	{
		START,
		END,
	};

	/*メンバ変数*/
	AnimationStage stage;
	std::map<AnimationStage, int>			 animationSet;//アニメーションのセット
	std::map<AnimationStage, AnimationStage> nextStageSet;//次のステージのセット

};

