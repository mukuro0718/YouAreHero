
#pragma once

class Character;
class Boid
{
public:
	 Boid();
	~Boid();

	void Initialize(const float _neighborRadius, const float _cohesionWeight, const float _alignmentWeight,
					const float _separationDistance, const float _separationWeight, const float _targetRadius,
					const float _stageRadius, const VECTOR _stageCenter);
	void Update	(Character& _boid, const std::vector<Character*> _boids, const VECTOR& _target);
	/*���������֐�*/
	VECTOR RuleCohesion			(const Character& _self);												//�X�e�[�W�̒��S�Ɍ������Ĉړ�
	VECTOR RuleSeparation		(const Character& _self, const std::vector<Character*> _boids);			//�߂����钇�Ԃ��狗�������
	VECTOR RuleAlignment		(const Character& _self, const std::vector<Character*> _boids);			//�߂��̒��ԂƓ��������ɐi�����Ƃ���
	VECTOR RuleTargetAttraction	(const Character& _self, const VECTOR& _target);						//
	VECTOR RuleBounding			(const Character& _self);												//���͈͊O�ɏo�Ȃ��悤�ɂ���
	VECTOR RuleSurroundPlayer	(const Character& _self, const VECTOR& _target);						//�v���C���[�����͂ނ悤�ȓ���
	const bool GetIsCanAttack() const { return this->isCanAttack; }
	void SetIsCanAttak(const bool _isCanAttack) { this->isCanAttack = _isCanAttack; }

private:
	/*�񋓑�*/
	enum class State
	{
		IDLE,
		MOVE,
	};
	enum class CircleDirection
	{
		RIGHT,
		LEFT
	};

	/*�ÓI�萔*/
	static constexpr float RANDOM_MAX_RADIUS = 20.0f;
	static constexpr float RANDOM_MAX_DISTANCE = 10.0f;
	static constexpr float RANDOM_MIN_DISTANCE = 3.0f;
	static constexpr float RANDOM_MAX_WEIGHT = 0.05f;
	static constexpr float RANDOM_MAX_DIRECTION = 1.0f;
	static constexpr int RANDOM_MAX_TIME = 60;
	static constexpr int RANDOM_MIN_TIME = 120;


	float  RandomFloat			(const float _min, const float _max);
	int	   RandomInt			(const int _min, const int _max);
	void   InitializeRandom		();

	/*�����o�ϐ�*/
	float	neighborRadius;		//boid���ق���boid�ɉe�����󂯂鋗���̍ő�l
	float	cohesionWeight;		//boid���Q��̒��S�Ɍ��������Ƃ���͂̋���
	float	alignmentWeight;	//�ق���boid���ړ����Ă���������܂˂��悤�Ƃ���͂̋���
	float	separationDistance;	//�ق���boid�Ɨ���悤�Ƃ��鋗���̍ő�l
	float	separationWeight;	//�ق���boid�Ɨ���悤�Ƃ���͂̋���
	float	targetRadius;		//�����boid�v���C���[�����͂ނ��߁A�ǂꂾ���������J���邩
	float	stageRadius;		//boid���X�e�[�W���痣��Ă悢�����i����ȏ㗣��Ȃ��悤�ɂ���j
	VECTOR	stageCenter;		//boid���X�|�[�������X�e�[�W�̒������W
	bool	isCanAttack;		//�U����

	CircleDirection circleDirection;//��]����Ƃ��Ɉړ��������
	State boidState;//boid�̏��
	int switchStateTimer;//��Ԃ�؂�ւ���^�C�}�[
	int nextSwitchTime;
};

