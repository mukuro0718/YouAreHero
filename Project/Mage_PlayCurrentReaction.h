//=====================================================
// @brief �ۑ������A�N�V���������s
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Mage_PlayCurrentReaction : public ActionNode
{
public:
	 Mage_PlayCurrentReaction();//�R���X�g���N�^
	~Mage_PlayCurrentReaction();//�f�X�g���N�^

	void		Initialize	();													//������
	NodeState	Update		(BehaviorTree& _tree, Character& _chara)override;	//�X�V
};

