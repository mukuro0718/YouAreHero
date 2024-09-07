//===========================================
// @brief 回転蹴り攻撃
//===========================================
#pragma once

class BossAttack;
class BossHurricaneKick : public GoriLib::BossAttack
{
public:
	BossHurricaneKick(const int _attackIndex);//コンストラクタ
	~BossHurricaneKick();//デストラクタ

	void Initialize(GoriLib::Physics* _physics)override;//初期化
	void Update(GoriLib::Physics* _physics)override;//更新
	const void Draw()const;							  //描画
protected:
	/*内部処理関数*/
	void		SetRadius(const float _radius) override;//半径のセット
	const float GetRadius() const			   override;//半径の取得

};

