
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
	/*内部処理関数*/
	VECTOR RuleCohesion			(const Character& _self);												//ステージの中心に向かって移動
	VECTOR RuleSeparation		(const Character& _self, const std::vector<Character*> _boids);			//近すぎる仲間から距離を取る
	VECTOR RuleAlignment		(const Character& _self, const std::vector<Character*> _boids);			//近くの仲間と同じ方向に進もうとする
	VECTOR RuleTargetAttraction	(const Character& _self, const VECTOR& _target);						//
	VECTOR RuleBounding			(const Character& _self);												//一定範囲外に出ないようにする
	VECTOR RuleSurroundPlayer	(const Character& _self, const VECTOR& _target);						//プレイヤーを取り囲むような動き
	const bool GetIsCanAttack() const { return this->isCanAttack; }
	void SetIsCanAttak(const bool _isCanAttack) { this->isCanAttack = _isCanAttack; }

private:
	/*列挙体*/
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

	/*静的定数*/
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

	/*メンバ変数*/
	float	neighborRadius;		//boidがほかのboidに影響を受ける距離の最大値
	float	cohesionWeight;		//boidが群れの中心に向かおうとする力の強さ
	float	alignmentWeight;	//ほかのboidが移動している方向をまねしようとする力の強さ
	float	separationDistance;	//ほかのboidと離れようとする距離の最大値
	float	separationWeight;	//ほかのboidと離れようとする力の強さ
	float	targetRadius;		//今回のboidプレイヤーを取り囲むため、どれだけ距離を開けるか
	float	stageRadius;		//boidがステージから離れてよい距離（これ以上離れないようにする）
	VECTOR	stageCenter;		//boidがスポーンしたステージの中央座標
	bool	isCanAttack;		//攻撃権

	CircleDirection circleDirection;//回転するときに移動する方向
	State boidState;//boidの状態
	int switchStateTimer;//状態を切り替えるタイマー
	int nextSwitchTime;
};

