//=====================================================
// @brief �u���X�A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class AttackCapsuleColliderData;
class Beast_Breath : public ActionNode
{
public:
	 Beast_Breath();//�R���X�g���N�^
	~Beast_Breath();//�f�X�g���N�^

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
		END,
	};

	/*�����o�ϐ�*/
	AnimationStage stage;
	std::map<AnimationStage, int>			 animationSet;					 //�A�j���[�V�����̃Z�b�g
	std::map<AnimationStage, AnimationStage> nextStageSet;					 //���̃X�e�[�W�̃Z�b�g
	short									 attackStartCount;				 //�U���J�n�t���[��
	short									 attackEndCount;				 //�U���I���t���[��
	short									 frameCount;					 //�t���[���J�E���g
	short									 frameIndexUsedCapsuleDirection1;//�u���X�̕��������߂邽�߂̃t���[���ԍ�
	short									 frameIndexUsedCapsuleDirection2;//�u���X�̕��������߂邽�߂̃t���[���ԍ�
	AttackCapsuleColliderData*				 collider;						 //�U���R���C�_�[
	float									 breathLength;					 //�u���X�̒���
};

