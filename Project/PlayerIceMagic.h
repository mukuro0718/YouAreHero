//============================================================
// @brief プレイヤー氷魔法クラス
//============================================================
#pragma once
#include "PlayerMagic.h"

class PlayerIceMagic :public PlayerMagic
{
public:
	PlayerIceMagic();//コンストラクタ
	~PlayerIceMagic();//デストラクタ

	void Initialize()override;//初期化
	void Update()override;//更新
	const void Draw()const override;//更新
private:
	VECTOR firstPosition;//最初の座標
	float velocity;
	int frameCount;
	bool isBooting;
};

