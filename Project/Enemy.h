//===============================================
// @brief �{�X�N���X
//===============================================
#pragma once

class Character;
class BehaviorTree;
class Boid;
class Enemy: public Character
{
public:
	 Enemy();//�R���X�g���N�^
	~Enemy();//�f�X�g���N�^

	void Move				(const float _maxSpeed, const float _accel, const float _decel, const bool _isLerp);//�ړ�
	void UpdateRotation		(VECTOR _toTarget);																	//��]���̍X�V
	void UpdateSpeed		(const float _maxSpeed, const float _accel, const float _decel);					//�ړ����x�̍X�V
	void UpdateVelocity		(const bool _isLerp);																//�ړ��x�N�g���̍X�V
	void ChangeAngryColor	();																					//�{���Ԏ��̐F�ɕύX
	void ChangeTiredColor	();																					//����Ԏ��̐F�ɕύX
	void ChangeNormalColor	();																					//�ʏ펞�̐F�ɕύX
	void DecAttackComboCount() { this->attackCount--; }															//�U���񐔂����炷

	/*boid�֐�*/
	Boid& GetBoid() { return *this->boid; }

	/*getter/setter*/
	const	VECTOR  GetSpownPosition		()const { return this->spownPosition; }		//�X�|�[���ʒu���擾
	const	VECTOR	GetPositionForLockon	()const { return this->positionForLockon; }	//���b�N�I���p���W�̎擾
	const	int		GetAttackCount			()const { return this->attackCount; }		//�c��U���R���{�񐔂̎擾
	const	float	GetAnimationPlayTime	()const;									//�A�j���[�V�����Đ����Ԃ̎擾
	const	VECTOR	GetMoveTarget			()const { return this->moveTarget; }		//�ړ��ڕW
	const	float	GetNowAnimationPlayTime	()const { return this->animationPlayTime; }	//���݂̃A�j���[�V�����Đ����Ԃ̎擾
	const	int		GetBossState			()const { return this->bossState; }			//�{���Ԃ̎擾
			void	SetRotation				(const VECTOR _rotation);											//��]���̐ݒ�
			void	SetVelocity				(const VECTOR _velocity);											//�ړ��x�N�g���̐ݒ�
			void	SetNowMoveTarget		(const VECTOR _moveTarget)	{ this->moveTarget = _moveTarget; }		//���݂̈ړ��ڕW��ݒ�
			void	SetSpeed				(const float _speed)		{ this->speed = _speed; }				//�����̐ݒ�
			void	SetAnimationPlayTime	(const float _playTime)		{ this->animationPlayTime = _playTime; }//�A�j���[�V�����Đ����Ԃ̐ݒ�
			void	SetNowAnimation			(const int _animation)		{ this->nowAnimation = _animation; }	//���݂̃A�j���[�V�����̐ݒ�
			void	OffIsDraw				()							{ this->isDraw = false; }				//�`��t���O������
			void	OffIsHit				();//�U�����󂯂����ɗ��t���O������
	virtual void	SetAttackCount			() abstract;
	const	bool	GetIsCanAttack			() const;
			void	SetIsCanAttak			(const bool _isCanAttack);
		  /*�񋓑�*/
	enum class BossState
	{
		TIRED,
		NORMAL,
		ANGRY,
	};
protected:
	/*�ÓI�萔*/
	static constexpr VECTOR ROTATE_LERP_VALUE = { 0.1f,0.1f,0.1f };
	static constexpr VECTOR VELOCITY_LERP_VALUE = { 0.1f,0.1f,0.1f };

	/*���������֐�*/


	/*�����o�ϐ�*/
	BehaviorTree*	tree;				//�c���[
	Boid*			boid;
	VECTOR			positionForLockon;	//�v���C���[�̃��b�N�I���p�̍��W
	VECTOR			moveTarget;			//�ړ��ڕW
	float			animationPlayTime;	//�A�j���[�V�����Đ�����
	int				nowAnimation;		//���݂̃A�j���[�V����
	COLOR_F			tiredColor;			//����Ԏ��J���[
	COLOR_F			angryColor;			//�{���Ԏ��J���[
	COLOR_F			normalColor;		//�ʏ��Ԏ��J���[
	int				bossState;			//�{�X�̏��
	int				attackCount;		//�U����
	float			angryValue;			//�{��Q�[�W
	float			tiredValue;			//��J�Q�[�W
	int				tiredDuration;		//����Ԃ��p�����鎞��
};

