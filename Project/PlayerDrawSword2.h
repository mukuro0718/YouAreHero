//=============================================================
// @brief プレイヤー納刀アクションクラス
//=============================================================
#pragma once

class PlayerAction;
class PlayerDrawSword2 : public PlayerAction
{
public:
	 PlayerDrawSword2();	//コンストラクタ
	~PlayerDrawSword2();	//デストラクタ
	void Initialize	() override;				//初期化
	void Finalize	() override;				//後処理
	void Update		(Player& _player) override;	//更新
private:
	short cancelableFrame;
	short frameIndex;
};

