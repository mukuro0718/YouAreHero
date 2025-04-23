#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "ColliderData.h"
#include "CharacterColliderData.h"
#include "Character.h"
#include "Boid.h"

/// <summary>
/// コンストラクタ
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
/// デストラクタ
/// </summary>
Boid::~Boid() 
{

}

/// <summary>
/// 初期化
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
/// ランダム関数の初期化
/// </summary>
void Boid::InitializeRandom()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

/// <summary>
/// ランダムなfloatの値を取得
/// </summary>
float Boid::RandomFloat(const float _min,const float _max)
{
	float rand = static_cast<float>(std::rand());
	float out = _min + rand / RAND_MAX * (_max - _min);
	return out;
}
/// <summary>
/// ランダムなintの値を取得
/// </summary>
int Boid::RandomInt(const int _min, const int _max)
{
	int rand = std::rand();
	int out = _min + rand / RAND_MAX * (_max - _min);
	return out;
}
/// <summary>
/// 更新
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

	/*新しい移動ベクトルを出す*/
	newVelocity = VGet(newVelocity.x, nowVelocity.y, newVelocity.z);
	_boid.SetVelocity(newVelocity);
}

/// <summary>
/// 求心性
/// </summary>
VECTOR Boid::RuleCohesion(const Character& _self)
{
	//切り替えるタイマーの計測(毎フレーム更新)
	++this->switchStateTimer;
	if (this->switchStateTimer >= this->nextSwitchTime)
	{
		this->boidState = (this->boidState == State::IDLE) ? State::MOVE : State::IDLE;
		this->switchStateTimer = 0;
		this->nextSwitchTime = RandomInt(this->RANDOM_MIN_TIME, this->RANDOM_MAX_TIME);
	}
	if (this->boidState == State::IDLE) return VGet(0, 0, 0);

	VECTOR direction = VSub(this->stageCenter, _self.GetRigidbody().GetPosition());//中心へのベクトル
	if (VSize(direction) >= this->neighborRadius)
	{
		direction = VNorm(direction);
		return direction;
	}
	return VGet(0, 0, 0);
}

/// <summary>
/// 整列性
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
/// 分離性
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
//		//範囲外に出たら目標に向かって戻ろうとする
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
		//外に出たら内側に戻す
		return VScale(VSub(this->stageCenter, selfNextPosition), 0.05f);
	}
	return VGet(0, 0, 0);
}
VECTOR Boid::RuleSurroundPlayer(const Character& _self, const VECTOR& _target)
{
	//切り替えるタイマーの計測(毎フレーム更新)
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
		//近すぎたら後退
		return VScale(VNorm(VSub(selfNextPosition, _target)), 0.05f);
	}
	else if (dist > this->targetRadius * 1.2f)
	{
		//遠すぎたら近づく
		return VScale(VNorm(toTarget), 0.03f);
	}
	else
	{
		if (this->boidState == State::IDLE) return VGet(0, 0, 0);
		//ちょうどよい距離ならプレイヤーの周囲を旋回（横に動く）するような方向を与える
		VECTOR right = VCross(toTarget, VGet(0, 1, 0));//プレイヤーの周囲を回るベクトル
		if (this->circleDirection == CircleDirection::LEFT)
		{
			return VScale(right, -1.0f);
		}
		return VScale(VNorm(right), 0.02f);
	}
}

/// <summary>
/// ターゲットを向く
/// </summary>
VECTOR Boid::RuleTargetAttraction(const Character& _self, const VECTOR& _target)
{
	VECTOR selfNextPosition = _self.GetRigidbody().GetPosition();
	VECTOR toTarget = VSub(_target, selfNextPosition);
	VECTOR direction = VNorm(toTarget);
	return direction;
}
