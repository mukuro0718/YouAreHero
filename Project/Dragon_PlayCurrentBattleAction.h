//=====================================================
// @brief �o�^���Ă���A�N�V���������s
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Dragon_PlayCurrentBattleAction : public ActionNode
{
public:
	 Dragon_PlayCurrentBattleAction();//�R���X�g���N�^
	~Dragon_PlayCurrentBattleAction();//�f�X�g���N�^

	NodeState Update(BehaviorTree& _tree, Character& _chara)override;//�X�V
};

