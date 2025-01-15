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
	vector<int>	icon;			//画像ハンドル
	vector<int>	summary;		//画像ハンドル
	vector<int> image;			//画像ハンドル
	int			background;		//背景
	int			logo;			//ロゴ
	int			iconFrame;		//アイコンのフレーム
	int			provDecide;		//決定アイコン
	int			nowSelectEnemy;	//現在選択されている敵
	bool		isEnd;			//終了するか
};

