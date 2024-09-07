//===========================================
// @brief ジャンプ攻撃
//===========================================
#pragma once

class BossAttack;
class BossJumpAttack : public GoriLib::BossAttack
{
public:
	BossJumpAttack(const int _attackIndex);//コンストラクタ
	~BossJumpAttack();//デストラクタ

	void Initialize(GoriLib::Physics* _physics)override;//初期化
	void Update(GoriLib::Physics* _physics)override;//更新
	const void Draw()const;							  //描画
protected:
	/*内部処理関数*/
	void		SetRadius(const float _radius) override;//半径のセット
	const float GetRadius() const			   override;//半径の
};

