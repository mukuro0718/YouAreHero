//==================================
// @brief スカイドームクラス
//==================================
#pragma once

class Model;
class Skydome
{
public:
	Skydome();
	~Skydome();

	void Initialize();		//初期化
	const void Draw()const;	//描画
private:
	int model;//モデル
	VECTOR position;
	VECTOR rotation;
	VECTOR scale;
};

