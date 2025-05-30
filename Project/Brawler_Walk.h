//=====================================================
// @brief 歩きアクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Brawler_Walk : public ActionNode
{
public:
	 Brawler_Walk();//コンストラクタ
	~Brawler_Walk();//デストラクタ

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//更新
private:
	/*静的定数*/
	static constexpr float THRESHOLD = 0.1f;

	/*メンバ変数*/
	short frameCount;		//フレームカウント
	short walkSoundCount;	//歩きの音を出すカウント
};

