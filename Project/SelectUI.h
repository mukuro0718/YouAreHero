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
	/*静的定数*/

	/*メンバ変数*/
	vector<int> enemyImage;		//敵画像
	int			iconTable;		//アイコンテーブル
	int			background;		//背景
	int			frame;			//アイコンのフレーム
	int			provDecide;		//決定アイコン
	int			nowSelectEnemy;	//現在選択されている敵
	bool		isEnd;			//終了するか
	int			headerFont;		//ヘッダーフォント
	int			questFont;		//クエストフォント
	int			actionFont;		//アクションフォント
	int			enemyImageTable;//敵画像テーブル
	int			drawRectBackGround;//描画範囲背景
	int			aButton;
	int			bButton;
	int			lStick;
};

