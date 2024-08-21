//============================================================
// @brief プレイヤー雷魔法クラス
//============================================================
#pragma once
#include "PlayerMagic.h"

class PlayerBoltMagic:public PlayerMagic
{
public:
	PlayerBoltMagic();//コンストラクタ
	~PlayerBoltMagic();//デストラクタ

	void Initialize	()override;//初期化
	void Update		()override;//更新
	const void Draw	()const override;//更新
private:
	VECTOR endPosition;//最初の座標
	float velocity;
	int frameCount;
	bool isBooting;//魔法が起動したか
};

