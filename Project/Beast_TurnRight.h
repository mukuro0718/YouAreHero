//=====================================================
// @brief �E�ɉ��A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Beast_TurnRight : public ActionNode
{
public:
	 Beast_TurnRight();//�R���X�g���N�^
	~Beast_TurnRight();//�f�X�g���N�^

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//�X�V
};

