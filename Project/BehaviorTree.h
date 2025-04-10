//============================================================
// @brief �r�w�C�r�A�c���[�x�[�X�N���X
//============================================================
#pragma once

class BehaviorTreeNode;
class Character;
class BehaviorTree abstract
{
public:
			 BehaviorTree();//�R���X�g���N�^
	virtual ~BehaviorTree();//�f�X�g���N�^

	virtual void		Initialize	()					abstract;		//������
	virtual void		Update		(Character& _chara) abstract;		//�X�V
	virtual const void	Draw		()					const abstract;	//�`��

	/*getter/setter*/
	const int		  GetNowSelectAction		()const				{ return this->selectAction; }					 //�I������Ă���A�N�V�������擾
	const bool		  GetIsCancelAction			()const				{ return this->isCancelAction; }				 //�L�����Z���t���O�̎擾
	BehaviorTreeNode& GetBattleAction			()					{ return *this->currentBattleNode; }			 //���ݎ��s���̃o�g���A�N�V����
	BehaviorTreeNode& GetReaction				()					{ return *this->currentReactionNode; }			 //���ݎ��s���̃o�g���A�N�V����
	const int		  GetInterval				(const int _index)	{ return this->intervalSet[_index]; }			 //�C���^�[�o���̎擾
	const float		  GetDotOfDirAndToTarget	()const				{ return this->innerProductOfDirectionToTarget; }//���ς̎擾
	const float		  GetToTargetDistance		()const				{ return this->toTargetDistance; }				 //�ڕW�܂ł̋������擾
	const bool		  GetIsSelectedBattleAction	()const				{ return this->isSelectedBattleAction; }		 //���ʔj�󂳂�Ă��邩
	const bool		  GetIsSelectedReaction		()const				{ return this->isSelectedReaction; }			 //���ʔj�󂳂�Ă��邩
	const int		  GetPrevHp					()const				{ return this->prevHp; }						 //�O�t���[����HP�̎擾
	const bool		  GetIsDestroyedPart		()const				{ return false; }								 //���ʔj�󂳂�Ă��邩
	const int		  GetLevel					()const				{ return 0; }									 //���x���̎擾
	const int		  GetDamage					()const				{ return this->damage; }						 //�_���[�W�̎擾

	void ResetLevel			()										 { this->level = 0; }					//���x�����グ��
	void RaiseLevel			()										 { this->level++; }						//���x�����グ��
	void SetNowSelectAction	(const int _actionType)					 { this->selectAction = _actionType; }	//�I�����ꂽ�A�N�V������ݒ�
	void SetInterval		(const int _index, const int _set = 600) { this->intervalSet[_index] = _set; }	//�C���^�[�o���̐ݒ�
	void SetDamage			(const int _set)						 { this->damage = _set; }				//�_���[�W��ݒ�

	void EntryCurrentBattleAction(BehaviorTreeNode& _action);	//�o�g���A�N�V�����̓o�^
	void ExitCurrentBattleAction ();							//�o�g���A�N�V�����̉���
	void EntryCurrentReaction	 (BehaviorTreeNode& _action);	//���A�N�V�����A�N�V�����̓o�^
	void ExitCurrentReaction	 ();							//���A�N�V�����A�N�V�����̉���
protected:
	BehaviorTreeNode*			mainNode;						//���C���m�[�h
	BehaviorTreeNode*			debugNode;						//�f�o�b�N�p�A�N�V�����m�[�h
	BehaviorTreeNode*			currentBattleNode;				//�o�g���A�N�V�������I������Ă���Ƃ��Ɏ��s�����m�[�h
	BehaviorTreeNode*			currentReactionNode;			//���A�N�V�������I������Ă���Ƃ��Ɏ��s�����m�[�h
	BehaviorTreeNode::NodeState nodeState;						//�m�[�h�̏�Ԃ�������ϐ�
	vector<int>					intervalSet;					//�C���^�[�o��
	float						toTargetDistance;				//�ڕW�܂ł̋���
	float						innerProductOfDirectionToTarget;//�L�����N�^�[�̌����x�N�g���ƖڕW�ւ̃x�N�g���̓���
	int							prevHp;							//�O�t���[����HP
	int							damage;							//���܂łɎ󂯂��_���[�W�ʁi�{���Ԃ��I������ƂO�Ƀ��Z�b�g����j
	int							selectAction;					//���ݑI�𒆂̃A�N�V����
	int							attackCount;					//�U���\��
	int							level;							//�{�X�̃��x��
	bool						isSelectedBattleAction;			//�U���A�N�V�������I������Ă��邩
	bool						isSelectedReaction;				//���A�N�V�������I������Ă��邩
	bool						isCancelAction;					//�A�N�V�������L�����Z�����ꂽ��
};

/*
memo
current~�̕ϐ��́A�e�A�N�V�������I�����ꂽ�Ƃ��ēx�c���[�����s���邱�ƂȂ�
�A�N�V���������s���邽�߂̃T�u�m�[�h
����ɂ��A�U�����⃊�A�N�V�������ɂق��̃A�N�V���������s����邱�Ƃ�h��
*/