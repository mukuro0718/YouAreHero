//===========================================
// @brief ボス攻撃
//===========================================
#pragma once

class Physics;
class Collidable;
class BossAttack : public GoriLib::Collidable
{
public:
	BossAttack(const int _attackNum);//コンストラクタ
	~BossAttack();//デストラクタ

	void		Initialize(GoriLib::Physics* _physics, const float _radius);							//初期化
	void		Finalize(GoriLib::Physics* _physics);							//後処理
	void		Update(GoriLib::Physics* _physics, const VECTOR _position, const VECTOR _direction, const bool _isMove, const float _speed);	//更新
	void		OnCollide(const Collidable& _colider)override;					//衝突したとき
	void		OnIsStart() { this->isStartHitCheck = true; }
	const void	Draw()const;												//描画
	const VECTOR GetPosition()const;
private:
	bool isStartHitCheck;
	bool isDontStartPrevFrame;
	int	 frameCount;
	int	 damage;
	float radius;
	VECTOR basePosition;
	VECTOR direction;
	int attackNum;
	float stackSpeed;
	//当たり判定のスフィアと座標はCollidableが持つため、発生タイミングを管理する
};

