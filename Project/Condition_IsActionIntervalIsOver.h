//===========================================================
// @brief �w��̃A�N�V�����̃C���^�[�o�����I�����Ă��邩�𔻒肷��R���f�B�V�����m�[�h
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsActionIntervalIsOver : public ConditionNode
{
public:
	 Condition_IsActionIntervalIsOver(const int _actionIndex);//�R���X�g���N�^
	~Condition_IsActionIntervalIsOver();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
private:
	const int ACTION_INDEX;//�A�N�V�����ԍ�
};

