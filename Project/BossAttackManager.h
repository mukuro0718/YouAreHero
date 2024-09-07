#pragma once

class Physics;
class Collidable;
namespace GoriLib
{
	class BossAttack;
}
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
	enum class AttackType
	{
		NONE		   = -1,
		SLASH		   = 0,//スラッシュ
		FLY_ATTACK	   = 1,//飛び攻撃
		HURRICANE_KICK = 2,//回転蹴り
		JUMP_ATTACK	   = 3,//ジャンプアタック
		ROTATE_PUNCH   = 4,//回転パンチ
	};
	std::vector<GoriLib::BossAttack*> attack;
	int hitNumber;
};

