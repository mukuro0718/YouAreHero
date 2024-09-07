//===========================================
// @brief プレイヤー通常攻撃１
//===========================================
#pragma once

class Physics;
class Collidable;
class PlayerAttack : public GoriLib::Collidable
{
public:
	PlayerAttack();//コンストラクタ
	~PlayerAttack();//デストラクタ

	void		Initialize	(GoriLib::Physics* _physics);							//初期化
	void		Finalize	(GoriLib::Physics* _physics);							//後処理
	void		Update		(GoriLib::Physics* _physics, const VECTOR _position, const VECTOR _direction);	//更新
	void		OnCollide	(const Collidable& _colider)override;					//衝突したとき
	void		OnIsStart	() { this->isStartHitCheck = true; }
	const bool	GetIsStart	()const { return this->isStartHitCheck; }
	const void	Draw		()const;												//描画
private:
	bool isStartHitCheck;
	bool isDontStartPrevFrame;
	int	 frameCount;
	int	 damage;
	float radius;
	VECTOR basePosition;
	VECTOR direction;
	//当たり判定のスフィアと座標はCollidableが持つため、発生タイミングを管理する
};

