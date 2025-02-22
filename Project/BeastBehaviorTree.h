//====================================================
// @brief ���N�X���I�p�r�w�C�r�A�c���[
//====================================================
#pragma once
#include "Singleton.h"

class BehaviorTreeNode;
class BeastBehaviorTree : public Singleton<BeastBehaviorTree>
{
public:
	friend class Singleton<BeastBehaviorTree>;

	/*�񋓑�*/

	enum class ActionType
	{
		DYING				= 0, //�f�X
		IDLE				= 1, //�ҋ@
		ROAR				= 2, //���K(�ʏ큨�{���Ԃ̕ω�)
		WALK				= 3, //����
		RUN					= 4, //�_�b�V��
		TURN_LEFT			= 5, //��������
		TURN_RIGHT			= 6, //�E������
		DOWN				= 7, //�_�E��
		LONG_FRIGHT			= 8, //��������
		SHORT_FRIGHT		= 9, //�Z������
		REST				= 10,//�x�e
		STEP_BACK			= 11,//���ɃW�����v���ĉ�����
		RAISE_LEVEL			= 12,//���̃`���[�W�����㏸������i���K�j
		BACKING_BREATH		= 13,//���ɉ�����Ȃ���u���X
		RUSH				= 14,//�ːi
		CHARGE_RIGHT_ATTACK	= 15,//�E�O�����ߍU��
		CHARGE_BOTH_ATTACK	= 16,//���O�����ߍU��
		BREATH				= 17,//�u���X
		SUPER_NOVA			= 18,//�X�[�p�[�m���@
		WEAK_BREATH			= 19,//��u���X
		SMALL_EXPLOSION		= 20,//������
		RIGHT_FOOT_ATTACK	= 21,//�E���U��
		EXPLOSION			= 22,//�����U���i�X�[�p�[�m���@����������)
		COMBO_ATTACK		= 23,//3�A���U��
	};
	//���x���̒i�K
	enum class LevelStage
	{
		Lv1 = 0,
		Lv2 = 1,
		Lv3 = 2,
		Lv4 = 3,
	};
		  void Initialize	();		//������
		  void Update		();		//�X�V
	const void Draw			()const;//�`��

	/*getter/setter*/
	const int		 GetDamage					()const				{ return this->damage; }						 //�_���[�W�̎擾
	const int		 GetPrevHp					()const				{ return this->prevHp; }						 //�O�t���[����HP�̎擾
	const int		 GetInterval				(const int _index)	{ return this->intervalSet[_index]; }			 //�C���^�[�o���̎擾
	const float		 GetToTargetDistance		()const				{ return this->toTargetDistance; }				 //�ڕW�܂ł̋������擾
	const int		 GetLevel					()const				{ return this->level; }							 //���x���̎擾
	const bool		 GetIsDestroyedPart			()const				{ return this->isDestroyedPart; }				 //���ʔj�󂳂�Ă��邩
	const bool		 GetIsSelectedBattleAction	()const				{ return this->isSelectedBattleAction; }		 //���ʔj�󂳂�Ă��邩
	const bool		 GetIsSelectedReaction		()const				{ return this->isSelectedReaction; }			 //���ʔj�󂳂�Ă��邩
	const short		 GetNowSelectAction			()const				{ return this->selectAction; }					 //�I������Ă���A�N�V�������擾
	const float		 GetDotOfDirAndToTarget		()const				{ return this->innerProductOfDirectionToTarget; }//���ς̎擾
	void SetDamage			(const int _set)						{ this->damage = _set; }						//�_���[�W��ݒ�
	void SetInterval		(const int _index,const int _set = 600)	{ this->intervalSet[_index] = _set; }			//�C���^�[�o���̐ݒ�
	void SetToTargetDistance(const float _set)						{ this->toTargetDistance = _set; }				//�ڕW�܂ł̋�����ݒ�
	void RaiseLevel			()										{ this->level++; }								//���x���㏸
	void ResetLevel			()										{ this->level = 0; }							//���x���̃��Z�b�g
	void SetIsDestroyedPart	(const bool _isDestroyedPart)			{ this->isDestroyedPart = _isDestroyedPart; }	//���ʔj�󂳂ꂽ���ǂ�����ݒ�
	void SetSelectAction	(const short _type)						{ this->selectAction = _type; }					//�I�����ꂽ�A�N�V������ݒ�
	void EntryCurrentBattleAction(BehaviorTreeNode& _action);	//�o�g���A�N�V�����̓o�^
	void EntryCurrentReaction	 (BehaviorTreeNode& _action);	//���A�N�V�����A�N�V�����̓o�^
	void ExitCurrentReaction	 ();							//���A�N�V�����A�N�V�����̉���
	void ExitCurrentBattleAction ();							//�o�g���A�N�V�����̉���
	BehaviorTreeNode& GetBattleAction() { return *this->currentBattleAction; }
	BehaviorTreeNode& GetReaction()  { return *this->currentReaction; }
private:
	/*���������֐�*/
		  BeastBehaviorTree	  ();//�R���X�g���N�^
		 ~BeastBehaviorTree	  ();//�f�X�g���N�^
	void UpdateMemberVariables();//�����o�ϐ��̍X�V
	void CreateBehaviorTree();//�r�w�C�r�A�c���[���쐬

	/*�����o�ϐ�*/
	std::array<int, static_cast<int>(ActionType::COMBO_ATTACK) + 1>	intervalSet;							//�C���^�[�o��
	BehaviorTreeNode*												Selector_DeathOrReactionOrBattleOrBreak;//�r�w�C�r�A�c���[��root�m�[�h
	int																prevHp;									//�O�t���[����HP
	int																damage;									//���܂łɎ󂯂��_���[�W�ʁi�{���Ԃ��I������ƂO�Ƀ��Z�b�g����j
	int																level;									//���x��
	int																selectAction;							//�I�����ꂽ���
	bool															isDestroyedPart;						//���ʔj�󂳂ꂽ��
	bool															isSelectedBattleAction;					//�o�g���A�N�V�������I������Ă��邩
	bool															isSelectedReaction;						//���A�N�V�������I������Ă��邩
	float															toTargetDistance;						//�ڕW�܂ł̋���
	float															innerProductOfDirectionToTarget;		//�����x�N�g���ƖڕW�x�N�g���̓���
	BehaviorTreeNode*												currentBattleAction;					//���݂̃o�g���A�N�V����
	BehaviorTreeNode*												currentReaction;						//���݂̃��A�N�V�����A�N�V����
	BehaviorTreeNode*												debugActionNode;						//�f�o�b�N�p�A�N�V�����m�[�h
};