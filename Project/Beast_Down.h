//=====================================================
// @brief ダウンアクション
// 現在は実装していない
// 必要なら実装する
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Beast_Down : public ActionNode
{
public:
	 Beast_Down();//コンストラクタ
	~Beast_Down();//デストラクタ

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//更新
private:
	/*列挙体*/
	//アニメーションの段階
	enum class AnimationStage
	{
		START,
		LOOP,
		END,
	};

	/*メンバ変数*/
	AnimationStage							 stage;			//アニメーションの段階
	std::map<AnimationStage, int>			 animationSet;	//アニメーションのセット
	std::map<AnimationStage, AnimationStage> nextStageSet;	//次のステージのセット
	bool isInitialize;//初期化したか
};

