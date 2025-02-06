//===============================================
// @brief �{�X�N���X
//===============================================
#pragma once

class Character;
class Enemy;
class DragonBehaviorTree;
class CharacterData;
class CharacterColliderData;
class AttackCapsuleColliderData;
class Dragon : public Enemy
{
public:
	 Dragon();//�R���X�g���N�^
	~Dragon();//�f�X�g���N�^

	void		Initialize			() override;										//������
	void		Finalize			() override;										//�㏈��
	void		Update				() override;										//�X�V
	const void	DrawCharacterInfo	() const override;									//�`��
	void		PlayAnimation		(const int _playAnimation, const float _playTime);	//�A�j���[�V�����̍Đ�
	void		OnAttackCollider	(const short _index);								//�w��̍U���R���C�_�[�̃t���O�𗧂Ă�
	void		OffAttackCollider	(const short _index);								//�w��̍U���R���C�_�[�̃t���O������
	void		UpdateAttackCollider(const short _colliderIndex, const float _nowTotalPlayTime);//�w��̍U���R���C�_�[���X�V

	/*�񋓑�*/
	//�A�j���[�V����
	enum class AnimationType
	{
		 DYING			= 0,//�f�X3
		 IDLE			= 1,//�ҋ@19
		 ROAR			= 2,//���K9
		 WALK			= 3,//����26
		 TURN_LEFT		= 4,//��������21
		 TURN_RIGHT		= 5,//�E������22
		 SMASH			= 6,//�@������1
		 SWEEP			= 7,//�Ȃ�����0
		 ROTATE_ATTACK	= 8,//��]�U��18
		 BREATH			= 9,//�u���X12
	};
	//���ʂ��Ƃ̃R���C�_�[
	enum class PartsCollider
	{
		HEAD		= 0,
		BODY		= 1,
		LEFT_HAND	= 2,
		RIGHT_HAND	= 3,
		LEFT_FOOT	= 4,
		RIGHT_FOOT	= 5,
		TAIL1		= 6,
		TAIL2		= 7,
		TAIL3		= 8,
	};
	//�U���R���C�_�[
	enum class AttackCollider
	{
		SMASH = 0,
		SWEEP = 1,
		ROTATE= 2,
		BREATH= 3,
	};
	/*getter/setter*/
	const bool GetIsAttack	()const override;			//�R�E�Q�L������
		  void OffIsDraw	(){ this->isDraw = false; }	//�`��t���O������
private:
	/*�ÓI�萔*/
	static constexpr float SHADOW_HEIGHT = 10.0f;//�e�𓊉e���鍂��
	static constexpr float SHADOW_SIZE	 = 30.0f; //�e�̃T�C�Y

	/*�����o�ϐ�*/
	map<short,vector<AttackCapsuleColliderData*>>				attackCollider;				//�U���R���C�_�[
	map<short, vector<vector<short>>>							frameIndexUseAttackColider;	//�U���R���C�_�[�Ɏg�p����t���[���ԍ�
	vector<CharacterColliderData*>								partsCollider;				//���ʂ��Ƃ̃R���C�_�[
	vector<vector<short>>										frameIndexUsePartsColider;	//���ʂ��Ƃ̃R���C�_�[�̎w��Ɏg�p����t���[���ԍ�
	array<float,static_cast<int>(AttackCollider::BREATH) + 1>	startHitCheckPlayTime;		//�����蔻��J�n�t���[��
	array<float,static_cast<int>(AttackCollider::BREATH) + 1>	endHitCheckPlayTime;		//�����蔻��I���t���[��
	array<bool, static_cast<int>(AttackCollider::BREATH) + 1>	isStartHitCheck;			//�����蔻��I���t���[��
	short														maxPartsColliderNum;		//�R���C�_�[�̐�
	vector<int>													prevPartsHp;				//1�t���[���O�̕��ʂ��Ƃ�HP
	int															maxHp;						//���ʂ̍ő�HP
	float														breathLength;				//�u���X�̒���

};

