//=====================================================
// @brief �ۑ������A�N�V���������s
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Beast_PlayCurrentReaction : public ActionNode
{
public:
	 Beast_PlayCurrentReaction();//�R���X�g���N�^
	~Beast_PlayCurrentReaction();//�f�X�g���N�^

	void		Initialize	();													//������
	NodeState	Update		(BehaviorTree& _tree, Character& _chara)override;	//�X�V
};

