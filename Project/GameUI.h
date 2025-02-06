//===============================================
// @brief ゲームUIクラス
//===============================================
#pragma once

class SceneUI;
class Image;
class HPUI;
class ButtonUI;
class BossNameUI;
class GameClearUI;
class GameOverUI;
class TutorialUI;
class GameUI : public SceneUI
{
public:
	GameUI();
	~GameUI();

		  void Initialize()		 override;
		  void Update	 ()		 override;
	const void Draw		 ()const override;
	const bool IsEnd	 ()const override;
private:
	/*列挙体*/
	enum class ResultType
	{
		NONE,
		WIN,
		LOSE,
	};

	/*静的定数*/
	static constexpr int TEXT_COLOR = 0xffffff;
	static constexpr short MAX_ALPHA = 255;
	const float LOCK_ON_UI_OFFSET;
	const float LOCK_ON_UI_SIZE;

	/*内部処理関数*/
	void SetType();

	/*メンバ変数*/
	HPUI*		 hp;				//バーUI
	ButtonUI*	 button;			//ボタンUI
	BossNameUI*  bossName;			//ボスの名前UI
	GameClearUI* clearUI;			//クエストクリアUI
	GameOverUI*	 overUI;			//クエスト失敗UI
	TutorialUI*  tutorialUI;		//チュートリアルUI
	ResultType	 type;				//ロゴの種類
	bool		 isEnd;				//終了フラグ
	int			 alpha;				//アルファ値
	int			 backGround;		//背景
	int			 lockOnImage;		//ロックオンUI画像
	int			 pauseFontHandle;	//中断用フォントハンドル
	int			 pauseActionHandle; //中断用フォントハンドル
	int			 frameCount;		//フレームカウント
	bool		 isPause;			//中断フラグ
	short		 maxAlpha;			//最大アルファ
	short		 addAlpha;			//増加アルファ
	short		 logoDrawTime;		//ロゴ描画時間
	int			 decideButton;		//決定ボタン
	int			 backButton;		//戻るボタン
	vector<int> pauseTableDrawRect;			//中断テーブル描画範囲
	vector<int> pauseHeaderTextPosition;	//中断ヘッダー文字座標
	vector<int> pauseActionTextPosition;	//中断アクション文字座標
	vector<int> decideButtonDrawRect;		//決定ボタン描画範囲
	vector<int> backButtonDrawRect;			//戻る描画範囲
};

