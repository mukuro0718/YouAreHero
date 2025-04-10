//=====================================================
// @brief レベル上昇時咆哮アクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Beast_RaiseLevel : public ActionNode
{
public:
	 Beast_RaiseLevel();//コンストラクタ
	~Beast_RaiseLevel();//デストラクタ

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//更新
private:
	short effectStartCount;	//エフェクト再生開始フレーム
	short frameCount;		//フレームカウント
	short roarFrame;	//咆哮のフレーム
};

