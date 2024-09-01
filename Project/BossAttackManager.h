#pragma once

class Physics;
class Collidable;
class BossAttack;
class BossAttackManager
{
public:
	BossAttackManager();//コンストラクタ
	~BossAttackManager();//デストラクタ

	void		Initialize(GoriLib::Physics* _physics);		//初期化
	void		Finalize(GoriLib::Physics* _physics);		//後処理
	void		Update(GoriLib::Physics* _physics);		//更新
	void		OnCollide(const GoriLib::Collidable& _colider);//衝突したとき
	const void	Draw()const;	//描画
	void		OnIsStart(const int _index);
	const VECTOR GetPosition(const int _index);
	const VECTOR GetThrowPosition();
private:
	static constexpr int ATTACK_NUM = 3;
	enum class AttackType
	{
		NONE = -1,
		SLASH = 0,//詠唱
		ROTATE_PUNCH = 1,//詠唱
		JUMP_ATTACK = 2,//コンボ
	};
	std::vector<BossAttack*> attack;
	int hitNumber;
};

