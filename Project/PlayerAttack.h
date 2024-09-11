//===========================================
// @brief プレイヤー通常攻撃１
//===========================================
#pragma once

class ColliderData;
class PlayerAttack
{
public:
	PlayerAttack();//コンストラクタ
	~PlayerAttack();//デストラクタ

	void		Initialize	();							//初期化
	void		Finalize	();							//後処理
	void		Update		(const VECTOR _position, const VECTOR _direction);	//更新
	const void	Draw		()const;												//描画
	void		OnIsStart	() { this->isStartHitCheck = true; this->isNotOnHit = false;}
	const bool	GetIsStart	()const { return this->isStartHitCheck; }
private:
	ColliderData* collider;
	bool isStartHitCheck;
	bool isNotOnHit;
	int	 frameCount;
	//当たり判定のスフィアと座標はCollidableが持つため、発生タイミングを管理する
};

