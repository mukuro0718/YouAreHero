//=====================================================
// @brief �o�^���Ă���A�N�V���������s
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Mage_PlayCurrentBattleAction : public ActionNode
{
public:
	 Mage_PlayCurrentBattleAction();//�R���X�g���N�^
	~Mage_PlayCurrentBattleAction();//�f�X�g���N�^

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//�X�V
};

