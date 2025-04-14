//===============================================================
// @brief �����A�Փ˔��肷��I�u�W�F�N�g��o�^���A
// �����ړ��A�Փ˂�ʒm����
//===============================================================
#pragma once
#include "Singleton.h"

class ColliderData;
class CollisionManager : public Singleton<CollisionManager>
{
public:
	/*Singleton�N���X��ColliderManager�ɃA�N�Z�X�ł���悤�ɂ���*/
	friend class Singleton<CollisionManager>;

	virtual ~CollisionManager() {}//�f�X�g���N�^


	void Entry(ColliderData& _data);	//�Փ˕��̓o�^
	void Exit(ColliderData& _data);		//�o�^����
	void Update();						//�X�V(�o�^�I�u�W�F�N�g�̕����ړ��A�Փ˒ʒm)
private:
	/*�ÓI�萔*/
	static constexpr float	GRAVITY					= -0.098f;	//�d��
	static constexpr float	MAX_GRAVITY_ACCEL		= -1.5f;	//�ő�d�͉����x
	static constexpr int	BEFORE_FIX_INFO_COLOR	= 0x0000ff;	//�␳�O���F
	static constexpr int	AIM_INFO_COLOR			= 0x0000aa;	//�␳�O�\����F
	static constexpr int	AFTER_FIX_INFO_COLOR	= 0xff00ff;	//�␳����F
	static constexpr int	DIV_NUM					= 16;		//�f�o�b�N�\���}�`������
	static constexpr int	MAX_CHECK_COUNT			= 1000;		//�����蔻����m�F����ő��
	static constexpr float  HIT_SLIDE_LENGTH		= 0.1f;
	static constexpr int	HIT_TRY_NUM				= 4;
	static constexpr int	MAX_HIT_POLY			= 1000;
	static constexpr VECTOR FLOOR_NORM				= { 0.0f,1.0f,0.0f };
	static constexpr VECTOR X_PLUS_NORM				= { 1.0f,0.0f,0.0f };
	static constexpr VECTOR X_MINUS_NORM			= { -1.0f,0.0f,0.0f };
	static constexpr VECTOR Z_PLUS_NORM				= { 0.0f,0.0f,1.0f };
	static constexpr VECTOR Z_MINUS_NORM			= { 0.0f,0.0f,-1.0f };
	static constexpr float  MAX_DEGREE				= 60.0f;

	/*���������֐�*/
			CollisionManager() {}												//�R���X�g���N�^
	void	CheckColide		();													//�����蔻��`�F�b�N
	bool	IsCollide		(ColliderData& _objectA, ColliderData& _objectB);	//����������
	void	FixNextPosition	(ColliderData& _primary, ColliderData& _secondary);	//���̍��W��␳
	void	FixPosition		();													//���W�m��
	float	GetDegree		(const VECTOR _norm1, const VECTOR _norm2);
	void	JudgeNorm		(const MV1_COLL_RESULT_POLY_DIM _hitPolyDim, vector<MV1_COLL_RESULT_POLY>& _floor, vector<MV1_COLL_RESULT_POLY>& _xpWall, vector<MV1_COLL_RESULT_POLY>& _xmWall, vector<MV1_COLL_RESULT_POLY>& _zpWall, vector<MV1_COLL_RESULT_POLY>& _zmWall, const VECTOR _position);
	bool	JudgeDegree		(const VECTOR _norm1, const VECTOR _norm2);
	float	JudgeMax		(const float _nowMax, const float _judgeValue);
	bool	HitCheckWall	(float& _max, vector<MV1_COLL_RESULT_POLY> _wall, const VECTOR _pos, const float _height, const float _radius, const bool _sign, const bool isX, const bool isZ);
	void CheckHitXWall(vector<MV1_COLL_RESULT_POLY> _xpWall, vector<MV1_COLL_RESULT_POLY> _xmWall, bool& _isHit, VECTOR& _maxFix, float _hitHeight, float _radius, VECTOR _nowNextPosition, VECTOR& _newNextPosition, VECTOR _moveVector);
	void CheckHitZWall(vector<MV1_COLL_RESULT_POLY> _xpWall, vector<MV1_COLL_RESULT_POLY> _xmWall, bool& _isHit, VECTOR& _maxFix, float _hitHeight, float _radius, VECTOR _nowNextPosition, VECTOR& _newNextPosition, VECTOR _moveVector);

	/*�����o�ϐ�*/
	std::list<ColliderData*> collidables;//�o�^���ꂽCollidable�̃��X�g
	int frameTime;//�����܂łɂ�����������
	enum class HitCheckType
	{
		SAME_CHARA,
		CHARA_ATTACK_S,
		CHARA_ATTACK_C,
		CHARA_PLANE,
		CHARA_MODEL,
	};
	std::list<HitCheckType> hitCheckLog;
	std::vector<std::vector<bool>> alreadyProcessed;
};

