//=====================================================
// @brief �����_���Z���N�^�[�m�[�h
// �q�m�[�h�������_���őI������
//=====================================================
#pragma once

class BehaviorTree;
class CompositeNode;
class Character;
class RandomSelector : public CompositeNode
{
public:
			 RandomSelector();//�R���X�g���N�^
	virtual ~RandomSelector();//�f�X�g���N�^

	NodeState Update(BehaviorTree& _tree, Character& _chara) override;//�X�V
};

