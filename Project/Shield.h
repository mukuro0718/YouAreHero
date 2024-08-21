//===========================================
// @brief シールドクラス
//===========================================
#pragma once

class Model;
class Shield
{
public:
	Shield();//コンストラクタ
	~Shield();//デストラクタ

	void		Initialize();							 //初期化
	void		Update	  (const VECTOR _targetPosition);//更新
	const void	Draw	  ()const;						 //描画
private:
	/*メンバ変数*/
	Model* model;//モデル
};

