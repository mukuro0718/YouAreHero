//=====================================================
// @brief �_�C�A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Dragon_Dying : public ActionNode
{
public:
	 Dragon_Dying();//�R���X�g���N�^
	~Dragon_Dying();//�f�X�g���N�^

	NodeState Update(BehaviorTree& _tree, Character& _chara)override;//�X�V
};

