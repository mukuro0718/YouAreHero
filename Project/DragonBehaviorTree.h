//====================================================
// @brief �����i�N�g�p�r�w�C�r�A�c���[
//====================================================
#pragma once
#include "Singleton.h"

class BehaviorTreeNode;
class DragonBehaviorTree : public Singleton<DragonBehaviorTree>
{
public:
	friend class Singleton<DragonBehaviorTree>;

	/*�񋓑�*/
	//�A�N�V�����̎��
	enum class ActionType
	{
		DYING	= 0, //�f�X
		IDLE	= 1, //�ҋ@
		ROAR	= 2, //���K(�ʏ큨�{���Ԃ̕ω�)
		WALK	= 3, //����
		SMASH	= 4, //�@����
		SWEEP	= 5, //�Ȃ�����
		ROTATE	= 6, //��]�U��
		BREATH	= 7, //�u���X
	};
		  void Initialize		();							//������
		  void Update			();							//�X�V
	const void Draw				()const;					//�`��

	/*getter/setter*/
	const int		  GetInterval				 (const int _index)const { return this->intervalSet[_index]; }			  //�C���^�[�o���̎擾
	const float		  GetToTargetDistance		 ()const				 { return this->toTargetDistance; }				  //�ڕW�ւ̃x�N�g�����擾
	const float		  GetInnerProduct			 ()const				 { return this->innerProductOfDirectionToTarget; }//�ڕW�ւ̃x�N�g���̓��ς��擾
	const int		  GetCurrentAction			 ()const				 { return this->currentAction; }				  //���݂̃A�N�V�������擾
	const bool		  GetIsSelectedBattleAction	 ()const				 { return this->isSelectedBattleAction; }		  //���ʔj�󂳂�Ă��邩
	const bool		  GetIsCancelAction			 ()const				 { return this->isCancelAction; }				  //�L�����Z���t���O�̎擾
	const float		  GetDotOfDirAndToTarget	 ()const				 { return this->innerProductOfDirectionToTarget; }//���ς̎擾
	BehaviorTreeNode& GetBattleAction			 ()						 { return *this->currentBattleAction; }			  //���ݎ��s���̃o�g���A�N�V����
	void SetCurrentAction	(const int _action)						{ this->currentAction = _action; }		//���݂̃A�N�V�����̐ݒ�
	void SetInterval		(const int _index, const int _set = 0)	{ this->intervalSet[_index] = _set; }	//�C���^�[�o���̐ݒ�
	void OnIsCancelAction	()										{ this->isCancelAction = true; }
	void OffIsCancelAction	()										{ this->isCancelAction = false; }
	void EntryCurrentBattleAction(BehaviorTreeNode& _action);	//�o�g���A�N�V�����̓o�^
	void ExitCurrentBattleAction();								//�o�g���A�N�V�����̉���
private:
	/*���������֐�*/
		  DragonBehaviorTree	();//�R���X�g���N�^
		 ~DragonBehaviorTree	();//�f�X�g���N�^
	void UpdateMemberVariables	();//�����o�ϐ��̍X�V
	void CreateBehaviorTree		();//�r�w�C�r�A�c���[���쐬

	/*�����o�ϐ�*/
	std::array<int, static_cast<int>(ActionType::BREATH) + 1>	intervalSet;					//�C���^�[�o��
	BehaviorTreeNode*											Selector_DyingOrBattleOrIdle;	//�r�w�C�r�A�c���[��root�m�[�h
	BehaviorTreeNode*											debugActionNode;				//�f�o�b�N�p�̃A�N�V�����m�[�h(TODO:�}�X�^�łł͏���)
	BehaviorTreeNode*											currentBattleAction;			//���݂̃o�g���A�N�V����
	BehaviorTreeNode::NodeState									prevNodeState;					//�O�̃m�[�h����A���Ă������
	float														toTargetDistance;				//�ڕW�܂ł̋���
	float														innerProductOfDirectionToTarget;//�����x�N�g���ƖڕW�x�N�g���̓���
	int															currentAction;					//���݂̃A�N�V����
	int															attackCount;					//�U����
	bool														isSelectedBattleAction;			//���A�N�V�������I������Ă��邩
	bool														isCancelAction;					//�A�N�V�����L�����Z���t���O
};