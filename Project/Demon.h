//===============================================
// @brief �{�X�N���X
//===============================================
#pragma once

class Character;
class Enemy;
class DemonBehaviorTree;
class CharacterData;
class CharacterColliderData;
class Demon : public Enemy
{
public:
	 Demon();//�R���X�g���N�^
	~Demon();//�f�X�g���N�^

	void		Initialize			() override;		//������
	void		Finalize			() override;		//�㏈��
	void		Update				() override;		//�X�V
	const void	DrawCharacterInfo	() const override;	//�`��
	void		PlayAnimation		();					//�A�j���[�V�����̍Đ�

	/*�񋓑�*/
	//�A�j���[�V����
	enum class AnimationType
	{
		DYING	,//�_�C
		IDLE	,//�ҋ@
	};

	/*getter/setter*/
	const bool	 GetIsAttack			()const override;													//�R�E�Q�L������
	const float	 GetAnimationPlayTime	()const;															//�A�j���[�V�����Đ����Ԃ̎擾
	const float	 GetNowAnimationPlayTime()const { return this->animationPlayTime; }							//���݂̃A�j���[�V�����Đ����Ԃ̎擾
		  void	 SetRotation			(const VECTOR _rotation);											//��]���̐ݒ�
		  void	 SetVelocity			(const VECTOR _velocity);											//�ړ��x�N�g���̐ݒ�
		  void	 SetNowMoveTarget		(const VECTOR _moveTarget)	{ this->moveTarget = _moveTarget; }		//���݂̈ړ��ڕW��ݒ�
		  void	 SetSpeed				(const float _speed)		{ this->speed = _speed; }				//�����̐ݒ�
		  void	 SetAnimationPlayTime	(const float _playTime)		{ this->animationPlayTime = _playTime; }//�A�j���[�V�����Đ����Ԃ̐ݒ�
		  void	 SetNowAnimation		(const int _animation)		{ this->nowAnimation = _animation; }	//���݂̃A�j���[�V�����̐ݒ�
		  void	 SetAttackCount			() override {}				//�U���R���{�̐ݒ�
private:
	/*�ÓI�萔*/
	static constexpr float SHADOW_HEIGHT = 10.0f;//�e�𓊉e���鍂��
	static constexpr float SHADOW_SIZE	 = 8.0f; //�e�̃T�C�Y
	static constexpr float LOCKON_OFFSET = 10.0f;//���b�N�I���I�t�Z�b�g

	/*�����o�ϐ�*/
	float	animationPlayTime;	//�A�j���[�V�����Đ�����
	int		nowAnimation;		//���݂̃A�j���[�V����
};

