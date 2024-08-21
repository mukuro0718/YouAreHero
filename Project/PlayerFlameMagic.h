//============================================================
// @brief プレイヤー炎魔法クラス
//============================================================
#pragma once
#include "PlayerMagic.h"

class PlayerFlameMagic :public PlayerMagic
{
public:
	PlayerFlameMagic();//コンストラクタ
	~PlayerFlameMagic();//デストラクタ

	void Initialize()override;//初期化
	void Update()override;//更新
	const void Draw()const override;//更新
private:
	VECTOR endPosition;//魔法の範囲終点
	int frameCount;
	float range;
};

