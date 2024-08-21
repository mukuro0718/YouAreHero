//============================================================
// @brief プレイヤー風魔法クラス
//============================================================
#pragma once
#include "PlayerMagic.h"

class PlayerStormMagic :public PlayerMagic
{
public:
	PlayerStormMagic();//コンストラクタ
	~PlayerStormMagic();//デストラクタ

	void Initialize()override;//初期化
	void Update()override;//更新
	const void Draw()const override;//更新
private:
	VECTOR endPosition;//最初の座標
	float velocity;
	int frameCount;
};

