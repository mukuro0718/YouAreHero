//===========================================================
// @brief �E�����ߍU��������s���R���f�B�V�����m�[�h
//===========================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsNowLevelIsSameAsTheSpecifiedLevel : public ConditionNode
{
public:
	 Condition_IsNowLevelIsSameAsTheSpecifiedLevel(const int _specifiedLevel);//�R���X�g���N�^
	~Condition_IsNowLevelIsSameAsTheSpecifiedLevel();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara) override;//�X�V
private:
	const int SPECIFIED_LEVEL;
};

