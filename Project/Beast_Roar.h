//=====================================================
// @brief 咆哮アクション
// 通常→怒りへの状態変化を行うときの咆哮
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Beast_Roar : public ActionNode
{
public:
	 Beast_Roar();//コンストラクタ
	~Beast_Roar();//デストラクタ

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//更新
private:
	short frameCount;		  //フレームカウント
	short roarFrame;		  //咆哮のフレーム
	bool  isFinishedFirstRoar;//最初の咆哮が終了しているか
};

