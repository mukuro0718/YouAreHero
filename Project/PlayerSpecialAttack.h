//===========================================
// @brief プレイヤースキル攻撃
//===========================================

#pragma once

class Physics;
class Collidable;
class PlayerSpecialAttack : public GoriLib::Collidable
{
public:
	PlayerSpecialAttack();//コンストラクタ
	~PlayerSpecialAttack();//デストラクタ

	void		Initialize(GoriLib::Physics* _physics);							//初期化
	void		Finalize(GoriLib::Physics* _physics);							//後処理
	void		Update(GoriLib::Physics* _physics, const VECTOR _position, const VECTOR _direction);	//更新
	void		OnCollide(const Collidable& _colider)override;					//衝突したとき
	void		OnIsStart() { this->isStartHitCheck = true; }
	const void	Draw()const;												//描画
private:
	static constexpr int ATTACK_NUM = 2;
	bool isStartHitCheck;
	int	 frameCount;
	bool isDontStartPrevFrame;
	int	 damage;
	float radius;
	VECTOR basePosition;
	VECTOR direction;
	//当たり判定のスフィアと座標はCollidableが持つため、発生タイミングを管理する
};

