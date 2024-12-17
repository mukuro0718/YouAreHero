//=====================================================
// @brief ブレスアクション
//=====================================================
#pragma once

class ActionNode;
class Beast_Breath : public ActionNode
{
public:
	 Beast_Breath();//コンストラクタ
	~Beast_Breath();//デストラクタ

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

