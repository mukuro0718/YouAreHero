//=====================================================
// @brief ���K�A�N�V����
// �ʏ큨�{��ւ̏�ԕω����s���Ƃ��̙��K
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class AttackCapsuleColliderData;
class Beast_FootComboAttack : public ActionNode
{
public:
	 Beast_FootComboAttack();//�R���X�g���N�^
	~Beast_FootComboAttack();//�f�X�g���N�^

	void		Initialize	();													//������
	NodeState	Update		(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	const void	Draw		()const;											//�`��
private:
	vector<short>				attackStartCount;				//�U���J�n�t���[��
	vector<short>				attackEndCount;					//�U���I���t���[��
	vector<short>				moveStartCount;					//�U���J�n�t���[��
	vector<short>				moveEndCount;					//�U���I���t���[��
	short						frameCount;						//�t���[���J�E���g
	short						maxAttackCount;					//�U���̍ő��
	short						attackCount;					//�U���̉�
	vector<short>				frameIndexUsedCapsuleDirection1;//�u���X�̕��������߂邽�߂̃t���[���ԍ�
	vector<short>				frameIndexUsedCapsuleDirection2;//�u���X�̕��������߂邽�߂̃t���[���ԍ�
	AttackCapsuleColliderData*  collider;						//�U���R���C�_�[
};

