//=====================================================
// @brief �����_���Z���N�^�[�m�[�h
// �q�m�[�h�������_���őI������
//=====================================================
#pragma once

class BehaviorTree;
class DecoratorNode;
class Character;
class Decorator_Inverse : public DecoratorNode
{
public:
			 Decorator_Inverse();//�R���X�g���N�^
	virtual ~Decorator_Inverse();//�f�X�g���N�^

	NodeState Update(BehaviorTree& _tree, Character& _chara) override;//�X�V
};

