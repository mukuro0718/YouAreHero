//===============================================
// @brief ゲームUIクラス
//===============================================
#pragma once

class Image;
class HPUI;
class ButtonUI;
class BossNameUI;
class GameClearUI;
class GameOverUI;
class TutorialUI
{
public:
	TutorialUI();
	~TutorialUI();

		  void Initialize()		;
		  void Update	 ()		;
	const void Draw		 ()const;
private:
	/*静的定数*/
	static constexpr int	TEXT_COLOR = 0xffffff;
	static constexpr short	MAX_ALPHA = 255;
	static constexpr short	ADD_ALPHA = 5;

	/*メンバ変数*/
	int			 alpha;				//アルファ値
	int			 backGround;		//背景
	int			 fontHandle;		//中断用フォントハンドル
	int			 frameCount;		//フレームカウント
	short		 textDrawTime;		//ロゴ描画時間
	bool		 isFadeIn;			//フェードインするか
	vector<int>  backGroundDrawRect;//背景描画範囲
	vector<int>  textPosition;		//文字座標
};

