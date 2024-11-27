//===============================================
// @brief セレクトシーンUIクラス
//===============================================
#pragma once

class Image;
class SceneUI;
class SelectUI : public SceneUI
{
public:
	 SelectUI();//コンストラクタ
	~SelectUI();//デストラクタ

		  void Initialize()		 override;
		  void Update	 ()		 override;
	const void Draw		 ()const override;
	const bool IsEnd	 ()const override;
private:
	/*メンバ変数*/
	int background;
	int logo;
	int iconFrame;
	int provDecide;
	vector<int>	icon;//画像ハンドル
	vector<int>	summary;//画像ハンドル
	vector<int> image;

	int nowSelectEnemy;//現在選択されている敵
	bool		isEnd;				//終了するか
};

