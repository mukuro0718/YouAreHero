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
	/*メンバ変数*/
	int		model;		//モデル
	float	addRotate;	//回転率の増加
	VECTOR	position;	//座標
	VECTOR	rotation;	//回転値
	VECTOR	scale;		//拡大率
};

