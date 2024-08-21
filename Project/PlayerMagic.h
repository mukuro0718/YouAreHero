//===========================================================
// @brief プレイヤー魔法クラス
//===========================================================
#pragma once

class PlayerMagic abstract
{
public:
	PlayerMagic();//コンストラクタ
	virtual ~PlayerMagic(){}//デストラクタ

	virtual		  void Initialize()		 abstract;
	virtual		  void Update	 ()		 abstract;
	virtual const void Draw		 ()const abstract;//描画

	/*setter*/
	void OnIsStart() { this->isStart = true; }
protected:
	/*メンバ変数*/
	VECTOR position;	//座標
	VECTOR moveVector;	//移動ベクトル
	float  radius;		//半径
	bool   isInitialize;//初期化フラグ
	bool isStart;
};

