//=====================================================
// @brief �o�^���Ă���A�N�V���������s
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Beast_PlayCurrentBattleAction : public ActionNode
{
public:
	 Beast_PlayCurrentBattleAction();//�R���X�g���N�^
	~Beast_PlayCurrentBattleAction();//�f�X�g���N�^

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//�X�V
};

