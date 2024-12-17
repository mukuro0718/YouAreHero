//=====================================================
// @brief スーパーノヴァアクション
//=====================================================
#pragma once

class ActionNode;
class Beast_SuperNova : public ActionNode
{
public:
	 Beast_SuperNova();//コンストラクタ
	~Beast_SuperNova();//デストラクタ

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

