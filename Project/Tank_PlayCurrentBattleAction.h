//=====================================================
// @brief �o�^���Ă���A�N�V���������s
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Tank_PlayCurrentBattleAction : public ActionNode
{
public:
	 Tank_PlayCurrentBattleAction();//�R���X�g���N�^
	~Tank_PlayCurrentBattleAction();//�f�X�g���N�^

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//�X�V
};

