//===========================================================
// @brief �ڕW�Ƃ̋������萔�ȏゾ������
//===========================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsToTargetDistanceGreaterThanConstant : public ConditionNode
{
public:
	 Condition_IsToTargetDistanceGreaterThanConstant(const float _compareDistance);//�R���X�g���N�^
	~Condition_IsToTargetDistanceGreaterThanConstant();//�f�X�g���N�^

	NodeState Update(BehaviorTree& _tree, Character& _chara) override;//�X�V
private:
	const float COMPARE_DISTANCE;//��r�������萔����
};

