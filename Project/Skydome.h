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
		  void Update	 ();		//更新
	const void Draw		 ()const;	//描画
private:
	int model;//モデル
	VECTOR position;
	VECTOR rotation;
	VECTOR scale;
};

