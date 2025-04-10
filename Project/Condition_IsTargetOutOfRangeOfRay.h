//===========================================================
// @brief �ڕW�Ƃ̋������萔�ȏゾ������
//===========================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsTargetOutOfRangeOfRay : public ConditionNode
{
public:
	Condition_IsTargetOutOfRangeOfRay(const float _maxToleranceRange, const float _minToleranceRange);//�R���X�g���N�^
	~Condition_IsTargetOutOfRangeOfRay();//�f�X�g���N�^

	NodeState Update(BehaviorTree& _tree, Character& _chara) override;//�X�V
private:
	const float MAX_TOLERANCE_RANGE;//�ŏ����e�͈�
	const float MIN_TOLERANCE_RANGE;//�ő勖�e�͈�
};

