//===========================================
// @brief �A�N�V�����m�[�h
// �Q�[���L�����N�^�[�̎��ۂ̂ӂ�܂�������
//===========================================
#pragma once

class BehaviorTree;
class BehaviorTreeNode;
class Character;
class ActionNode abstract : public BehaviorTreeNode
{
public:
			 ActionNode();//�R���X�g���N�^
	virtual ~ActionNode(){}//�f�X�g���N�^

	virtual NodeState Update(BehaviorTree& _tree, Character& _chara) abstract;//�X�V
protected:
	static constexpr short BREATH_HIT_COUNT = 10;//�u���X�U���̃q�b�g�t���O���ċN�I�ɗ��Ă�J�E���g

	short rotateAnimationType;	//��]�p�A�j���[�V�����i����͋��ʁj
	short animationType;		//�A�j���[�V�����̎��
	short actionType;			//�A�N�V�����̎��
	short interval;				//�C���^�[�o��
	float animationPlayTime;	//�A�j���[�V�����Đ�����
	float rotatePlayTime;		//��]�A�j���[�V�����Đ�����
	float maxSpeed;				//�ő呬�x
	float accel;				//�����x
	float decel;				//�����x
	bool  isFixRotate;			//��]���̏C���i�A�N�V�����J�n���Ƀv���C���[�̂ق��������悤�ɂ���j

};

