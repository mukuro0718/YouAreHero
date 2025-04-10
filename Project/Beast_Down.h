//=====================================================
// @brief �_�E���A�N�V����
// ���݂͎������Ă��Ȃ�
// �K�v�Ȃ��������
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Beast_Down : public ActionNode
{
public:
	 Beast_Down();//�R���X�g���N�^
	~Beast_Down();//�f�X�g���N�^

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//�X�V
private:
	/*�񋓑�*/
	//�A�j���[�V�����̒i�K
	enum class AnimationStage
	{
		START,
		LOOP,
		END,
	};

	/*�����o�ϐ�*/
	AnimationStage							 stage;			//�A�j���[�V�����̒i�K
	std::map<AnimationStage, int>			 animationSet;	//�A�j���[�V�����̃Z�b�g
	std::map<AnimationStage, AnimationStage> nextStageSet;	//���̃X�e�[�W�̃Z�b�g
	bool isInitialize;//������������
};

