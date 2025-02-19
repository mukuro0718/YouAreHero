//===========================================
// @brief スラッシュコンボ攻撃Ver.2
//===========================================
#pragma once

class BossAttack;
class BossSlashCombo2Attack : public BossAttack
{
public:
	BossSlashCombo2Attack(const int _attackIndex);//コンストラクタ
	~BossSlashCombo2Attack();//デストラクタ

	void Initialize() override;		//初期化
	void Update(const float _playTime) override;		//更新
	const void Draw() const override;	//描画
private:
	vector<int> onHitCheckFrame;
	vector<int> offHitChekcFrame;
};

