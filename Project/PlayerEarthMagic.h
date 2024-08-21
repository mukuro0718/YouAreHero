//============================================================
// @brief プレイヤー土魔法クラス
//============================================================
#pragma once
#include "PlayerMagic.h"

class PlayerEarthMagic :public PlayerMagic
{
public:
	PlayerEarthMagic();//コンストラクタ
	~PlayerEarthMagic();//デストラクタ

	void Initialize()override;//初期化
	void Update()override;//更新
	const void Draw()const override;//更新
private:
	int frameCount;
};

