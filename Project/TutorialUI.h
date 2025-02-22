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

		  void Initialize()		;//初期化
		  void Update	 ()		;//更新
	const void Draw		 ()const;//描画
private:
	/*静的定数*/
	static constexpr int	TEXT_COLOR = 0xffffff;
	static constexpr short	MAX_ALPHA = 255;
	static constexpr short	ADD_ALPHA = 5;

	/*メンバ変数*/
	int			  alpha;			 //アルファ値
	int			  backGround;		 //背景
	int			  headerFontHandle;	 //中断用フォントハンドル
	int			  bodyFontHandle;	 //中断用フォントハンドル
	int			  frameCount;		 //フレームカウント
	bool		  isFadeIn;			 //フェードインするか
	string		  headerText;		 //テキスト
	vector<string>bodyText;			 //テキスト
	vector<int>   backGroundDrawRect;//背景描画範囲
	vector<int>   headerTextPosition;//文字座標
	vector<int>   bodyTextPosition;//文字座標
};

