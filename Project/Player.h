//===============================================
// @brief �v���C���[�N���X
//===============================================
#pragma once

class PlayerController;
class Character;
class HitStop;
class PlayerAction;
class Player : public Character
{
public:
	Player();//�R���X�g���N�^
	~Player();//�f�X�g���N�^

	void		Initialize		 () override;		//������
	void		Finalize		 () override;		//�㏈��
	void		Update			 () override;		//�X�V
	const void	DrawCharacterInfo()const override;	//�`��
	void		PlayAnimation	 (const int _nextAnimation, const float _playTime);
	void		DeathProcess	 ();
	const bool	CanAction		 (const float _staminaConsumed)const;//�A�N�V�������ł��邩
	void		CalcStamina		 (const float _staminaConsumed, const float _maxStamina);			//�X�^�~�i�̉񕜏���

	/*getter*/
	const int		GetNowState			()const;
	const bool		GetIsAttack			()const override { return false; }												//�U���t���O�̎擾
	const float		GetStamina			()const;																		//�X�^�~�i�̎擾
	const int		GetHealCount		()const { return this->healCount; }												//�񕜃I�[�u�̐�(���͉񕜉񐔂ɂȂ��Ă���)
	CharacterData&	GetPlayerData		();																				//�v���C���[�f�[�^�̎擾
	Rigidbody&		GetPlayerRigidbody	();																				//���W�b�h�{�f�B�̎擾
	const bool		GetIsDrawSword		()const { return this->isDrawSword; }											//������o���Ă��邩
		  void		SetHealCount		(const int _count) { this->healCount = _count; }								//�񕜉񐔂̐ݒ�
		  void		SetHitStop			(const int _time, const int _type, const int _delay, const float _factor);		//�q�b�g�X�g�b�v�̐ݒ�
		  void		SetIsDrawSword		(const bool _set) { this->isDrawSword = _set; }									//�����t���O�̐ݒ�
	const bool		GetIsLock			() { return this->isLock; }//���b�N�I���t���O
	//�A�j���[�V�����̎��
	enum class AnimationType
	{
		IDLE			= 0,
		AVOID			= 1,
		DEATH			= 2,
		BLOCK			= 3,
		REACTION		= 4,
		BLOCK_REACTION	= 5,
		WALK_FRONT		= 6,
		WALK_BACK		= 7,
		WALK_LEFT		= 8,
		WALK_RIGHT		= 9,
		RUN_FRONT		= 10,
		RUN_BACK		= 11,
		RUN_LEFT		= 12,
		RUN_RIGHT		= 13,
		RUN_180_TURN	= 14,
		COMBO_1			= 15,
		HEAL			= 16,
		COMBO_2			= 17,
		COMBO_3			= 18,
		SKILL			= 19,
		DOWN_REACTION	= 20,
		DOWN_UP			= 21,
		DRAW_SWORD_1	= 22,
		DRAW_SWORD_2	= 23,
	};

private:
	/*�ÓI�萔*/
	static constexpr float	SHADOW_HEIGHT	= 10.0f;//�e�𓊉e���鍂��
	static constexpr float	SHADOW_SIZE		= 6.0f;	//�����̃T�C�Y
	
	/*�����o�ϐ�*/
	PlayerController*		controller;		//�R���g���[���[
	vector<PlayerAction*>	action;			//�A�N�V����
	HitStop*				hitStop;		//�q�b�g�X�g�b�v
	int						healCount;		//�񕜉\��
	bool					isDrawSword;	//���𔲂��Ă��邩
	int						frameTime;		//�����܂łɂ�����������
	bool isLock;
	bool isPrevPushLS;
};

