//=====================================================
// @brief ���K�A�N�V����
// �ʏ큨�{��ւ̏�ԕω����s���Ƃ��̙��K
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Beast_Roar : public ActionNode
{
public:
	 Beast_Roar();//�R���X�g���N�^
	~Beast_Roar();//�f�X�g���N�^

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//�X�V
private:
	short frameCount;		  //�t���[���J�E���g
	short roarFrame;		  //���K�̃t���[��
	bool  isFinishedFirstRoar;//�ŏ��̙��K���I�����Ă��邩
};

