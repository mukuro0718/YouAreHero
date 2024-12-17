//=====================================================
// @brief 短い怯みアクション
//=====================================================
#pragma once

class ActionNode;
class Beast_ShortFrightening : public ActionNode
{
public:
	 Beast_ShortFrightening();//コンストラクタ
	~Beast_ShortFrightening();//デストラクタ

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
	AnimationStage							 stage;			//アニメーションの段階
	std::map<AnimationStage, int>			 animationSet;	//アニメーションのセット
	std::map<AnimationStage, AnimationStage> nextStageSet;	//次のステージのセット
};

