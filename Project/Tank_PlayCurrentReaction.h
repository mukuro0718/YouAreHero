//=====================================================
// @brief �ۑ������A�N�V���������s
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Tank_PlayCurrentReaction : public ActionNode
{
public:
	 Tank_PlayCurrentReaction();//�R���X�g���N�^
	~Tank_PlayCurrentReaction();//�f�X�g���N�^

	void		Initialize	();													//������
	NodeState	Update		(BehaviorTree& _tree, Character& _chara)override;	//�X�V
};

