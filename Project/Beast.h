//===============================================
// @brief �{�X�N���X
//===============================================
#pragma once

class Character;
class Enemy;
class BeastBehaviorTree;
class CharacterData;
class CharacterColliderData;
class Beast : public Enemy
{
public:
	 Beast();//�R���X�g���N�^
	~Beast();//�f�X�g���N�^

	void		Initialize			() override;		//������
	void		Finalize			() override;		//�㏈��
	void		Update				() override;		//�X�V
	const void	DrawCharacterInfo	() const override;	//�`��
	void		PlayAnimation		();					//�A�j���[�V�����̍Đ�

	/*�񋓑�*/
	//�A�j���[�V����
	enum class AnimationType
	{
		DYING					,//�f�X(97)
		IDLE					,//�ҋ@(111)
		ROAR					,//���K(�ʏ큨�{���Ԃ̕ω�110)
		WALK					,//����(121)
		RUN						,//�_�b�V��(117)
		TURN_LEFT				,//��������(119)
		TURN_RIGHT				,//�E������(120)
		DOWN_START				,//�_�E��(5)
		DOWN_LOOP				,//�_�E��(4)
		DOWN_END				,//�_�E��(3)
		LONG_FRIGHT_START		,//��������(�J�n104)
		LONG_FRIGHT_LOOP		,//��������(�r��102)
		LONG_FRIGHT_END			,//��������(�I��100)
		SHORT_FRIGHT_START		,//�Z������(�J�n107)
		SHORT_FRIGHT_LOOP		,//�Z������(�J�n106)
		SHORT_FRIGHT_END		,//�Z������(�I��105)
		REST_START				,//�x�e(�J�n89)
		REST_LOOP				,//�x�e(�r��90)
		REST_END				,//�x�e(�I��91)
		STEP_BACK				,//��щ�����(8)
		RAISE_LEVEL				,//�`���[�W���x���̏㏸(14)
		BACKING_BREATH			,//������Ȃ���u���X(10)
		RUSH_START				,//�ːi(�J�n75)
		RUSH_LOOP				,//�ːi(�r��76)
		RUSH_ROTATE_END			,//�ːi(��]���Ď~�܂�77)
		RUSH_NORMAL_END			,//�ːi(���ʂɎ~�܂�78)
		CHARGE_RIGHT_ATTACK		,//�E�O�����ߍU��(52)
		CHARGE_BOTH_ATTACK_START,//���O�����ߍU��(�J�n50)
		CHARGE_BOTH_ATTACK_END	,//���O�����ߍU��(�I��51)
		BREATH_START			,//�u���X(�J�n45)
		BREATH_LOOP				,//�u���X(�r��46)
		BREATH_END				,//�u���X(�I��47)
		SUPER_NOVA_START		,//�X�[�p�[�m���@(�J�n27)
		SUPER_NOVA_LOOP			,//�X�[�p�[�m���@(�r��28)
		SUPER_NOVA_END			,//�X�[�p�[�m���@(�I��29)
		WEAK_BREATH				,//��u���X(80)
		SMALL_EXPLOSION			,//������(9)
		RIGHT_FOOT_ATTACK		,//�E���U��(40)
		EXPLOSION				,//�����U���i�X�[�p�[�m���@�����������j(65)
		COMBO_ATTACK			,//�E���ōU���������]�U��(22)
	};
	enum class Parts
	{
		HEAD,
		BODY,
		LEFT_HAND,
		RIGHT_HAND,
		LEFT_FOOT,
		RIGHT_FOOT,
	};

	/*getter/setter*/
	const bool		 GetIsAttack			()const override;													//�R�E�Q�L������
	const VECTOR GetNowMoveTarget()const { return this->moveTarget; }//�ړ��ڕW�̎擾
		  void		 SetRotation			(const VECTOR _rotation);											//��]���̐ݒ�
		  void		 SetVelocity			(const VECTOR _velocity);											//�ړ��x�N�g���̐ݒ�
		  void		 SetNowMoveTarget		(const VECTOR _moveTarget)	{ this->moveTarget = _moveTarget; }		//���݂̈ړ��ڕW��ݒ�
		  void		 SetSpeed				(const float _speed)		{ this->speed = _speed; }				//�����̐ݒ�
	const float		 GetAnimationPlayTime	()const;															//�A�j���[�V�����Đ����Ԃ̎擾
	const float		 GetNowAnimationPlayTime()const { return this->animationPlayTime; }							//���݂̃A�j���[�V�����Đ����Ԃ̎擾
		  void		 SetAnimationPlayTime	(const float _playTime)		{ this->animationPlayTime = _playTime; }//�A�j���[�V�����Đ����Ԃ̐ݒ�
		  void		 SetNowAnimation		(const int _animation)		{ this->nowAnimation = _animation; }	//���݂̃A�j���[�V�����̐ݒ�
private:
	/*�ÓI�萔*/
	static constexpr float SHADOW_HEIGHT = 10.0f;//�e�𓊉e���鍂��
	static constexpr float SHADOW_SIZE	 = 8.0f; //�e�̃T�C�Y
	
	/*�����o�ϐ�*/
	vector<CharacterColliderData*>  partsCollider;				//���ʂ��Ƃ̃R���C�_�[
	short							maxPartsColliderNum;		//�R���C�_�[�̐�
	VECTOR							moveTarget;					//�ړ��ڕW
	float							animationPlayTime;			//�A�j���[�V�����Đ�����
	int								nowAnimation;				//���݂̃A�j���[�V����
	vector<vector<short>>			frameIndexUsePartsColider;	//���ʂ��Ƃ̃R���C�_�[�̎w��Ɏg�p����t���[���ԍ�
	vector<VECTOR>					pos1;						//���ʂ��Ƃ̃R���C�_�[�̍��W�����Ă���
	vector<VECTOR>					pos2;						//HACK:�Ȃ����������������Ȃ��Ƃ��܂�����Ȃ��A�Ȃ�
	vector<int>						prevPartsHp;				//�O�̕��ʂ��Ƃ�HP
	int								maxHp;						//���ʂ̍ő�HP
};

