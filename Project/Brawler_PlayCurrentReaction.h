//=====================================================
// @brief �ۑ������A�N�V���������s
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Brawler_PlayCurrentReaction : public ActionNode
{
public:
	 Brawler_PlayCurrentReaction();//�R���X�g���N�^
	~Brawler_PlayCurrentReaction();//�f�X�g���N�^

	void		Initialize	();													//������
	NodeState	Update		(BehaviorTree& _tree, Character& _chara)override;	//�X�V
};

