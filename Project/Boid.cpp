#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "ColliderData.h"
#include "CharacterColliderData.h"
#include "Character.h"
#include "Boid.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Boid::Boid()
	: neighborRadius	(0.0f)
	, cohesionWeight	(0.0f)
	, alignmentWeight	(0.0f)
	, separationDistance(0.0f)
	, separationWeight	(0.0f)
	, targetRadius		(0.0f)
	, stageRadius		(0.0f)
	, stageCenter		{ 0.0f,0.0f,0.0f }
	, isCanAttack		(false)
	, boidState			(State::IDLE)
	, switchStateTimer	(0)
	, nextSwitchTime	(0)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Boid::~Boid() 
{

}

/// <summary>
/// ������
/// </summary>
void Boid::Initialize(const float _neighborRadius, const float _cohesionWeight, const float _alignmentWeight,
					  const float _separationDistance, const float _separationWeight, const float _targetRadius,
					  const float _stageRadius, const VECTOR _stageCenter)
{
	InitializeRandom();
	this->neighborRadius	 = _neighborRadius + RandomFloat(0, this->RANDOM_MAX_RADIUS);
	this->cohesionWeight	 = _cohesionWeight + RandomFloat(0, this->RANDOM_MAX_WEIGHT);
	this->alignmentWeight	 = _alignmentWeight + RandomFloat(0, this->RANDOM_MAX_WEIGHT);
	this->separationDistance = _separationDistance + RandomFloat(this->RANDOM_MIN_DISTANCE, this->RANDOM_MAX_DISTANCE);
	this->separationWeight	 = _separationWeight + RandomFloat(0, this->RANDOM_MAX_WEIGHT);
	this->targetRadius		 = _targetRadius + RandomFloat(0, this->RANDOM_MAX_RADIUS);
	this->stageRadius		 = _stageRadius;
	this->stageCenter		 = VAdd(_stageCenter, VGet(RandomFloat(0, this->RANDOM_MAX_DISTANCE / 2.0f), 0.0f, RandomFloat(0, this->RANDOM_MAX_DISTANCE / 2.0f)));
}

/// <summary>
/// �����_���֐��̏�����
/// </summary>
void Boid::InitializeRandom()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

/// <summary>
/// �����_����float�̒l���擾
/// </summary>
float Boid::RandomFloat(const float _min,const float _max)
{
	float rand = static_cast<float>(std::rand());
	float out = _min + rand / RAND_MAX * (_max - _min);
	return out;
}
/// <summary>
/// �����_����int�̒l���擾
/// </summary>
int Boid::RandomInt(const int _min, const int _max)
{
	int rand = std::rand();
	int out = _min + rand / RAND_MAX * (_max - _min);
	return out;
}
/// <summary>
/// �X�V
/// </summary>
void Boid::Update(Character& _boid, const std::vector<Character*> _boids, const VECTOR& _playerPos)
{
	VECTOR nowVelocity = _boid.GetRigidbody().GetVelocity();
	VECTOR v1 = RuleCohesion(_boid);
	VECTOR v2 = RuleAlignment(_boid, _boids);
	VECTOR v3 = RuleSeparation(_boid, _boids);
	VECTOR v4 = RuleSurroundPlayer(_boid, _playerPos);
	VECTOR v5 = RuleBounding(_boid);
	//VECTOR v1 = VGet(0,0,0);
	//VECTOR v2 = VGet(0,0,0);
	//VECTOR v3 = VGet(0,0,0);
	//VECTOR v4 = VGet(0,0,0);
	//VECTOR v5 = VGet(0,0,0);

	VECTOR newVelocity = VAdd(nowVelocity, VAdd(VAdd(VAdd(v1, v2), v3),VAdd(v4,v5)));
	float MAX_SPEED = _boid.GetSpeed();
	if (VSize(newVelocity) > MAX_SPEED)
	{
		newVelocity = VScale(VNorm(newVelocity), MAX_SPEED);
	}

	/*�V�����ړ��x�N�g�����o��*/
	newVelocity = VGet(newVelocity.x, nowVelocity.y, newVelocity.z);
	_boid.SetVelocity(newVelocity);
}

/// <summary>
/// ���S��
/// </summary>
VECTOR Boid::RuleCohesion(const Character& _self)
{
	//�؂�ւ���^�C�}�[�̌v��(���t���[���X�V)
	++this->switchStateTimer;
	if (this->switchStateTimer >= this->nextSwitchTime)
	{
		this->boidState = (this->boidState == State::IDLE) ? State::MOVE : State::IDLE;
		this->switchStateTimer = 0;
		this->nextSwitchTime = RandomInt(this->RANDOM_MIN_TIME, this->RANDOM_MAX_TIME);
	}
	if (this->boidState == State::IDLE) return VGet(0, 0, 0);

	VECTOR direction = VSub(this->stageCenter, _self.GetRigidbody().GetPosition());//���S�ւ̃x�N�g��
	if (VSize(direction) >= this->neighborRadius)
	{
		direction = VNorm(direction);
		return direction;
	}
	return VGet(0, 0, 0);
}

/// <summary>
/// ����
/// </summary>
VECTOR Boid::RuleAlignment(const Character& _self, const std::vector<Character*> _boids)
{
	VECTOR avgVelocity = VGet(0, 0, 0);
	int count = 0;
	VECTOR selfNextPosition = _self.GetRigidbody().GetPosition();
	VECTOR selfVelocity = _self.GetRigidbody().GetVelocity();
	for (const auto& other : _boids)
	{
		if (other == &_self) continue;
		VECTOR otherNextPosition = other->GetRigidbody().GetPosition();
		VECTOR otherVelocity = other->GetRigidbody().GetVelocity();
		if (VSize(VSub(otherNextPosition,selfNextPosition)) < this->neighborRadius)
		{
			avgVelocity = VAdd(avgVelocity, otherVelocity);
			count++;
		}
	}
	if (count == 0) return VGet(0, 0, 0);
	avgVelocity = VScale(avgVelocity, 1.0f / count);
	return VScale(VSub(avgVelocity, selfVelocity), this->alignmentWeight);
}

/// <summary>
/// ������
/// </summary>
VECTOR Boid::RuleSeparation(const Character& _self, const std::vector<Character*> _boids)
{
	VECTOR repulsion = VGet(0, 0, 0);
	VECTOR selfNextPosition = _self.GetRigidbody().GetPosition();
	for (const auto& other : _boids) 
	{
		if (other == &_self) continue;
		VECTOR otherNextPosition = other->GetRigidbody().GetPosition();
		float dist = VSize(VSub(otherNextPosition, selfNextPosition));
		if (dist < this->separationDistance && dist > 0) {
			repulsion = VAdd(repulsion, VNorm(VSub(selfNextPosition, otherNextPosition)));
		}
	}
	return VScale(repulsion, this->separationWeight);
}

//VECTOR Boid::RuleTargetAttraction(const Character& _self, const VECTOR& _target)
//{
//	VECTOR selfNextPosition = _self.GetRigidbody().GetPosition();
//	float dist = VSize(VSub(_target, selfNextPosition));
//	if (dist > this->targetRadius)
//	{
//		//�͈͊O�ɏo����ڕW�Ɍ������Ė߂낤�Ƃ���
//		return VScale(VSub(_target, selfNextPosition), 0.02f);
//	}
//	return VGet(0, 0, 0);
//}

VECTOR Boid::RuleBounding(const Character& _self)
{
	VECTOR selfNextPosition = _self.GetRigidbody().GetPosition();
	float dist = VSize(VSub(selfNextPosition, this->stageCenter));
	if (dist > this->stageRadius)
	{
		//�O�ɏo��������ɖ߂�
		return VScale(VSub(this->stageCenter, selfNextPosition), 0.05f);
	}
	return VGet(0, 0, 0);
}
VECTOR Boid::RuleSurroundPlayer(const Character& _self, const VECTOR& _target)
{
	//�؂�ւ���^�C�}�[�̌v��(���t���[���X�V)
	++this->switchStateTimer;
	if (this->switchStateTimer >= this->nextSwitchTime)
	{
		this->boidState = (this->boidState == State::IDLE) ? State::MOVE : State::IDLE;
		this->switchStateTimer = 0;
		this->nextSwitchTime = RandomInt(this->RANDOM_MIN_TIME, this->RANDOM_MAX_TIME);
		this->circleDirection = (RandomFloat(0.0f, this->RANDOM_MAX_DIRECTION) > 0.5f) ? CircleDirection::LEFT : CircleDirection::RIGHT;
	}

	VECTOR selfNextPosition = _self.GetRigidbody().GetPosition();
	VECTOR toTarget = VSub(_target, selfNextPosition);
	float dist = VSize(toTarget);

	if (dist < this->targetRadius * 0.8f)
	{
		//�߂���������
		return VScale(VNorm(VSub(selfNextPosition, _target)), 0.05f);
	}
	else if (dist > this->targetRadius * 1.2f)
	{
		//����������߂Â�
		return VScale(VNorm(toTarget), 0.03f);
	}
	else
	{
		if (this->boidState == State::IDLE) return VGet(0, 0, 0);
		//���傤�ǂ悢�����Ȃ�v���C���[�̎��͂����i���ɓ����j����悤�ȕ�����^����
		VECTOR right = VCross(toTarget, VGet(0, 1, 0));//�v���C���[�̎��͂����x�N�g��
		if (this->circleDirection == CircleDirection::LEFT)
		{
			return VScale(right, -1.0f);
		}
		return VScale(VNorm(right), 0.02f);
	}
}

/// <summary>
/// �^�[�Q�b�g������
/// </summary>
VECTOR Boid::RuleTargetAttraction(const Character& _self, const VECTOR& _target)
{
	VECTOR selfNextPosition = _self.GetRigidbody().GetPosition();
	VECTOR toTarget = VSub(_target, selfNextPosition);
	VECTOR direction = VNorm(toTarget);
	return direction;
}
