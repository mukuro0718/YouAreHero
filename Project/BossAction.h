//================================================
// @brief �{�X�A�N�V�����̊��N���X
//================================================
#pragma once

class Boss;
class ActionParameter;
class BossAttack;
class HitStop;
class BossAction abstract
{
public:
			 BossAction();	//�R���X�g���N�^
	virtual ~BossAction();	//�f�X�g���N�^
	virtual void Initialize	  ()					abstract;//������
	virtual void Update		  (Boss& _boss)			abstract;//�X�V
	virtual void CalcParameter(const Boss& _boss)	abstract;//�p�����[�^�[�̌v�Z
	const void Draw()const;

	/*getter/setter*/
	const int  GetWeight	 (const int _sum)const;
	const int  GetDesireValue()	const;
	const int  GetBaseWeight ()	const;
	const bool GetIsSelect	 () const { return this->isSelect; }
	const bool GetIsPriority () const { return this->isPriority; }
		  void OnIsSelect	 (){ this->isSelect = true; }
protected:
	/*�񋓑�*/
	/*���������֐�*/
		  bool		FrameCount		(const int _maxFrame);																						//�t���[���J�E���g
		  void		OffIsSelect		(const int _maxInterval);																					//�I���t���O�̏�����
		  VECTOR	GetLerpRotation (Boss& _boss, const VECTOR _positionToTargetVector, const VECTOR _nowRotation, const VECTOR _lerpValue);	//�⊮������]���̎擾
		  VECTOR	CalcVelocity	(const VECTOR _prevVelocity, const VECTOR _nowRotation, const float _speed);								//�ړ��x�N�g���̌v�Z
		  float		Lerp			(const float _start, const float _end, const float _percent);												//��[�Պ֐�
		  COLOR_F	LerpColor		(const COLOR_F _start, const COLOR_F _end, const COLOR_F _percent);											//��[�Պ֐�
	const COLOR_F	ColorConvert	(const std::vector<float> _value);																			//vector<float>��COLOR_F�ɕϊ�

	/*�����o�ϐ�*/
	ActionParameter*		parameter;			 //�p�����[�^
	BossAttack*				attack;				 //�{�X�U���N���X
	VECTOR					moveTarget;			 //�ړ��ڕW
	bool					isSelect;			 //�A�N�V�������I�����ꂽ��
	short					frameCount;			 //�t���[���J�E���g
	bool					isInitialize;		 //���������ꂽ��
	bool					isPriority;			 //�D��t���O
	bool					isAllowAction;		 //�A�N�V������������
	int						frameTime;			 //�t���[������
	short					nextAnimation;		 //���̃A�j���[�V����
	float					animationPlayTime;	 //�A�j���[�V�����Đ�����
	short					maxDesireValue;		 //�ő�~���l
	float					nowTotalAnimPlayTime;//���݂̑��Đ�����
	short					interval;			 //�C���^�[�o��
};

