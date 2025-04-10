//==========================================
// �J�����N���X
//==========================================
#pragma once

class ColliderData;
class Rigidbody;
class Camera
{
public:
	 Camera();//�R���X�g���N�^
	~Camera();//�f�X�g���N�^

		  void Initialize();		//������
		  void Update	 ();		//�X�V
	const void Draw		 ()const;	//�`��
	static void InitializeStaticConst();

	/*getter*/
	const VECTOR GetNowPosition	()const;						//���݂̍��W�i�g�p���Ă��Ȃ��j
	const VECTOR GetNowTarget	() { return this->nowTarget; }	//���݂̒����_�i�g�p���Ă��Ȃ��j
	const VECTOR GetDirection	() { return this->direction; }	//���݂̒����_�i�v���C���[��TPS�ő��삷��Ƃ��̂ݕK�v�j
private:
	/*�萔*/
	const int BOSS_HEAD_FRAME_INDEX;
	const int ENTRY_TIME;
	static constexpr float ENTRY_MULT = 1.5f;

	/*���������֐�*/
	void   UpdateDirection	();																//�����̍X�V
	void   UpdateAngle		();																//���W�̍X�V
	void   UpdateVelocity	();																//�ړ��x�N�g��
	void   UpdateTarget		();																//�����_�̍X�V
	void   UpdateLength		();																//���W�̏C��
	void   UpdateFoV		();																//Fov�̍X�V
	float  Lerp				(const float _start, const float _end, const float _percent);	//��[�Պ֐�
	VECTOR Lerp				(const VECTOR _start, const VECTOR _end, const VECTOR _percent);//��[�Պ֐�
	float  DegToRad			(const float _deg) { return _deg * (DX_PI_F / 180.0f); }		//�x�������W�A���ɕϊ�

	/*�����o�ϐ�*/
	ColliderData*	collider;				//�R���C�_�[
	VECTOR			nextTarget;				//���̒����_
	VECTOR			nowTarget;				//���̒����_
	VECTOR			direction;				//�J�������璍���_�̕���
	float			fov;					//field of view
	float			length;					//����
	float			yow;					//���[�i����]�j
	float			pitch;					//�s�b�`�i�c��]�j
	int				entryTime;				//���掞��
	bool			isFightingBoss;			//�{�X�Ɛ���Ă��邩

	//TODO:�ȉ��̕ϐ���Json�̃f�[�^���i�[����ϐ��ł���A�o����Β萔�ɂ��ׂ����̂ł���
	float			nearClip;				//��O�N���b�v����
	float			farClip;				//���N���b�v����
	float			firstLength;			//�����i�������p�j
	float			firstAngle;				//�A���O���i�������p�j
	VECTOR			firstDirection;			//�����i�������p�j
	VECTOR			targetOffset;			//
	VECTOR			firstPosition;			//
	VECTOR			titleTarget;			//
	VECTOR			lerpValueForTarget;		//
	VECTOR			lerpValueForVelocity;	//
	float			addAngle;				//
	float			maxYow;					//
	float			minYow;					//
	float			maxPitch;				//
	float			minPitch;				//
	float			titleMaxLength;			//
	float			titleMinLength;			//
	float			deathMaxLength;			//	
	float			deathMinLength;			//
	float			maxLength;				//
	float			minLength;				//
	float			lerpValueForLength;		//
	float			titlePositionOffset;	//
	VECTOR			positionOffset;			//
	//HACK:�����_����length�������������ʒu�ɃJ�����̍��W��ݒ肷�邽�߁A����direction�𔽓]�������l��ێ�����
};
