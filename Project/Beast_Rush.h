//=====================================================
// @brief �ˌ��A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class AttackCapsuleColliderData;
class Beast_Rush : public ActionNode
{
public:
	 Beast_Rush();//�R���X�g���N�^
	~Beast_Rush();//�f�X�g���N�^

	void		Initialize	();													//������
	NodeState	Update		(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	const void	Draw		()const;											//�`��

private:	
	/*�\����*/
	//�A�j���[�V�����̒i�K
	enum class AnimationStage
	{
		START,
		LOOP,
		NORMAL_END,
		ANGRY_END,
	};

	/*�萔*/
	const float STOP_DISTANCE;//��~����
	const short FOOTSTEPS_SOUND_FRAME;//�������Đ�����Ԋu

	/*�����o�ϐ�*/
	short									 attackStartCount;				 //�U���J�n�t���[��
	short									 attackEndCount;				 //�U���I���t���[��
	short									 frameCount;					 //�t���[���J�E���g
	short									 frameIndexUsedCapsuleDirection1;//�u���X�̕��������߂邽�߂̃t���[���ԍ�
	short									 frameIndexUsedCapsuleDirection2;//�u���X�̕��������߂邽�߂̃t���[���ԍ�
	AnimationStage							 stage;							 //�A�j���[�V�����̒i�K
	std::map<AnimationStage, int>			 animationSet;					 //�A�j���[�V�����̃Z�b�g
	std::map<AnimationStage, AnimationStage> nextStageSet;					 //���̃X�e�[�W�̃Z�b�g
	AttackCapsuleColliderData*				 collider;						 //�U���R���C�_�[
};

