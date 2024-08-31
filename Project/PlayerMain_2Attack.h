//===========================================
// @brief プレイヤー通常攻撃２
//===========================================
#pragma once

class Physics;
class Collidable;
class PlayerMain_2Attack : public GoriLib::Collidable
{
public:
	PlayerMain_2Attack();//コンストラクタ
	~PlayerMain_2Attack();//デストラクタ

	void		Initialize(GoriLib::Physics* _physics);							//初期化
	void		Finalize(GoriLib::Physics* _physics);							//後処理
	void		Update(GoriLib::Physics* _physics, const VECTOR _position, const VECTOR _direction);	//更新
	void		OnCollide(const Collidable& _colider)override;					//衝突したとき
	void		OnIsStart() { this->isStartHitCheck = true; }
	const void	Draw()const;												//描画
private:
	static constexpr int ATTACK_NUM = 1;
	bool isStartHitCheck;
	int	 frameCount;
	int	 damage;
	bool isDontStartPrevFrame;
	float radius;
	VECTOR basePosition;
	VECTOR direction;
	//当たり判定のスフィアと座標はCollidableが持つため、発生タイミングを管理する
};

