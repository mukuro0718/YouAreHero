//=====================================================
// @brief 突撃アクション
//=====================================================
#pragma once

class ActionNode;
class Beast_Rush : public ActionNode
{
public:
	 Beast_Rush();//コンストラクタ
	~Beast_Rush();//デストラクタ

	NodeState Update()override;//更新
private:	
	/*構造体*/
	//アニメーションの段階
	enum class AnimationStage
	{
		START,
		LOOP,
		NORMAL_END,
		ANGRY_END,
	};

	/*メンバ変数*/
	AnimationStage stage;
	std::map<AnimationStage, int>			 animationSet;//アニメーションのセット
	std::map<AnimationStage, AnimationStage> nextStageSet;//次のステージのセット
};

