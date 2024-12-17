//=====================================================
// @brief 長い怯みアクション
//=====================================================
#pragma once

class ActionNode;
class Beast_LongFrightening : public ActionNode
{
public:
	 Beast_LongFrightening();//コンストラクタ
	~Beast_LongFrightening();//デストラクタ

	NodeState Update()override;//更新
private:
	/*構造体*/
	//アニメーションの段階
	enum class AnimationStage
	{
		START,
		LOOP,
		END,
	};

	/*メンバ変数*/
	AnimationStage stage;
	std::map<AnimationStage, int>			 animationSet;//アニメーションのセット
	std::map<AnimationStage, AnimationStage> nextStageSet;//次のステージのセット
};

