//=====================================================
// @brief �X�[�p�[�m���@�A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class AttackSphereColliderData;
class Beast_SuperNova : public ActionNode
{
public:
	 Beast_SuperNova();//�R���X�g���N�^
	~Beast_SuperNova();//�f�X�g���N�^

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
	AnimationStage							 stage;							//�A�j���[�V�����̒i�K
	std::map<AnimationStage, int>			 animationSet;					//�A�j���[�V�����̃Z�b�g
	std::map<AnimationStage, AnimationStage> nextStageSet;					//���̃X�e�[�W�̃Z�b�g
	short									 effectStartCount;				//�U���J�n�t���[��
	short									 attackStartCount;				//�U���J�n�t���[��
	short									 attackEndCount;				//�U���J�n�t���[��
	short									 frameCount;					//�t���[���J�E���g
	short									 frameIndexUsedSpherePosition;	//�J�����̒��S�����߂邽�߂̃t���[���ԍ�
	AttackSphereColliderData*				 collider;						//�U���R���C�_�[
};

