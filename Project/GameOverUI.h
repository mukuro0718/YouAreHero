//===============================================
// @brief クエスト失敗時のUI
//===============================================
#pragma once

class GameOverUI
{
public:
	 GameOverUI();//コンストラクタ
	~GameOverUI();//デストラクタ

	void		Initialize();	//初期化
	void		Update();		//更新
	const void	Draw() const;	//描画

	const bool GetIsProductFinished()const { return this->isProductFinished; }
private:
	/*列挙体*/
	//演出の段階
	enum class ProductStage
	{
		WAIT_FOR_FADE_IN,
		LOGO_FADE_IN,
		WAIT_FOR_FADE_OUT,
		LOGO_FADE_OUT,
	};

	/*定数*/
	static constexpr int	MAX_ALPHA	= 255;						//最大アルファ値
	static constexpr double LOGO_ANGLE	= 0.0;						//ロゴの最初の回転率
	const float				FIRST_LOGO_RATE;						//ロゴの最初の大きさ
	const float				TARGET_LOGO_RATE_FOR_FADE_IN;			//フェードイン時のロゴの目標の大きさ
	const float				LERP_VALUE_FOR_REDUCTION;				//縮小用ラープ値
	const int				WAIT_TIME_BEFORE_START_FADE_IN;			//フェードイン開始前の待機時間
	const int				WAIT_TIME_BEFORE_START_FADE_OUT;		//フェードアウト開始前の待機時間
	const int				INCREASED_ALPHA_FOR_LOGO_FADE_IN;		//ロゴフェードイン用アルファ値増加量
	const int				REDUCTION_ALPHA_FOR_LOGO_FADE_OUT;		//ロゴフェードアウト用アルファ値減少量
	const int				POSITION_X;								//描画座標
	const int				FIRST_POSITION_Y;						//描画座標
	const int				POSITION_Y_FOR_FADE_OUT_MAGNIFICATION;	//フェードアウト時のY座標増加量

	/*メンバ変数*/
	ProductStage currentProductStage;	//現在の演出の段階
	int			 logoAlpha;				//ロゴのアルファ値
	float		 logoRate;				//ロゴの大きさ
	int			 waitTimer;				//待機時間(フレームごとに-1される)
	bool		 isProductFinished;		//演出が終了したか
	int			 questFailedLogo;		//クエスト失敗時のロゴ
	int			 positionY;				//描画座標
};

