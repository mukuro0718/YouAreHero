//=====================================================
// @brief �o�^���Ă���A�N�V���������s
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Brawler_PlayCurrentBattleAction : public ActionNode
{
public:
	 Brawler_PlayCurrentBattleAction();//�R���X�g���N�^
	~Brawler_PlayCurrentBattleAction();//�f�X�g���N�^

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//�X�V
};

