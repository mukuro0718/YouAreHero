//=====================================================
// @brief ��u���X�A�N�V����
//=====================================================
#pragma once

class ActionNode;
class AttackCapsuleColliderData;
class Beast_WeakBreath : public ActionNode
{
public:
	 Beast_WeakBreath();//�R���X�g���N�^
	~Beast_WeakBreath();//�f�X�g���N�^

	NodeState Update()override;//�X�V
	const void Draw()const;	//�`��
private:
	short						attackStartCount;				//�U���J�n�t���[��
	short						attackEndCount;					//�U���I���t���[��
	short						frameCount;						//�t���[���J�E���g
	short						frameIndexUsedCapsuleDirection1;//�u���X�̕��������߂邽�߂̃t���[���ԍ�
	short						frameIndexUsedCapsuleDirection2;//�u���X�̕��������߂邽�߂̃t���[���ԍ�
	AttackCapsuleColliderData*	collider;						//�U���R���C�_�[
	float						breathLength;					//�u���X�̒���
};
