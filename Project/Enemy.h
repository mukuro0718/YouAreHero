//===============================================
// @brief �{�X�N���X
//===============================================
#pragma once

class Character;
class Enemy: public Character
{
public:
	 Enemy();//�R���X�g���N�^
	~Enemy();//�f�X�g���N�^

	/*���������֐�*/
	void Move			(const float _maxSpeed, const float _accel, const float _decel, const bool _isLerp);//�ړ�
	void UpdateRotation	(VECTOR _toTarget);																	//��]���̍X�V
	void UpdateSpeed	(const float _maxSpeed, const float _accel, const float _decel);					//�ړ����x�̍X�V
	void UpdateVelocity	(const bool _isLerp);																//�ړ��x�N�g���̍X�V

	/*getter/setter*/
	const VECTOR GetMoveTarget			()const { return this->moveTarget; }
	const float  GetAnimationPlayTime	()const;															//�A�j���[�V�����Đ����Ԃ̎擾
	const float  GetNowAnimationPlayTime()const { return this->animationPlayTime; }							//���݂̃A�j���[�V�����Đ����Ԃ̎擾
		  void   SetRotation			(const VECTOR _rotation);											//��]���̐ݒ�
		  void   SetVelocity			(const VECTOR _velocity);											//�ړ��x�N�g���̐ݒ�
		  void   SetNowMoveTarget		(const VECTOR _moveTarget)	{ this->moveTarget = _moveTarget; }		//���݂̈ړ��ڕW��ݒ�
		  void   SetSpeed				(const float _speed)		{ this->speed = _speed; }				//�����̐ݒ�
		  void   SetAnimationPlayTime	(const float _playTime)		{ this->animationPlayTime = _playTime; }//�A�j���[�V�����Đ����Ԃ̐ݒ�
		  void   SetNowAnimation		(const int _animation)		{ this->nowAnimation = _animation; }	//���݂̃A�j���[�V�����̐ݒ�
		  void   OffIsDraw				()							{ this->isDraw = false; }				//�`��t���O������
protected:
	/*�ÓI�萔*/
	static constexpr VECTOR ROTATE_LERP_VALUE = { 0.1f,0.1f,0.1f };
	static constexpr VECTOR VELOCITY_LERP_VALUE = { 0.1f,0.1f,0.1f };

	/*�����o�ϐ�*/
	VECTOR						moveTarget;				//�ړ��ڕW
	float						animationPlayTime;		//�A�j���[�V�����Đ�����
	int							nowAnimation;			//���݂̃A�j���[�V����
};
