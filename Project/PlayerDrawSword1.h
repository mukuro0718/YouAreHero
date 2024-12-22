//=============================================================
// @brief プレイヤー抜刀アクションクラス
//=============================================================
#pragma once

class PlayerAction;
class PlayerDrawSword1 : public PlayerAction
{
public:
	 PlayerDrawSword1();	//コンストラクタ
	~PlayerDrawSword1();	//デストラクタ
	void Initialize	() override;				//初期化
	void Finalize	() override;				//後処理
	void Update		(Player& _player) override;	//更新
private:
	int frameCount;
};

