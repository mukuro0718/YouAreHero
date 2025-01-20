//===============================================
// @brief クエストクリア時のUI
//===============================================
#pragma once

class GameClearUI
{
public:
	 GameClearUI();//コンストラクタ
	~GameClearUI();//デストラクタ

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
		EFFECT_FADE_OUT,
		WAIT_FOR_FADE_OUT,
		LOGO_FADE_OUT,
	};

	/*定数*/
	static constexpr int DRAW_POSITION_X = 960;							//描画座標
	static constexpr int DRAW_POSITION_Y = 540;							//描画座標
	static constexpr int MAX_ALPHA		 = 255;								//最大アルファ値
	const float			 FIRST_LOGO_ANGLE;								//ロゴの最初の回転率
	const float			 TARGET_LOGO_ANGLE;								//ロゴの回転目標
	const float			 FIRST_LOGO_RATE;								//ロゴの最初の大きさ
	const float			 TARGET_LOGO_RATE_FOR_FADE_IN;					//フェードイン時のロゴの目標の大きさ
	const float			 TARGET_LOGO_RATE_FOR_FADE_OUT;					//フェードアウト時のロゴの目標の大きさ
	const float			 TARGET_EFFECT_LOGO_RATE;						//エフェクト用ロゴの目標の大きさ
	const float			 LERP_VALUE_FOR_ANGLE;							//アングル用ラープ値
	const float			 LERP_VALUE_FOR_REDUCTION;						//縮小用ラープ値
	const float			 LERP_VALUE_FOR_FADE_OUT_MAGNIFICATION;			//フェードイン時拡大用ラープ値
	const float			 LERP_VALUE_FOR_EFFECT_FADE_OUT_MAGNIFICATION;	//フェードイン時エフェクト拡大用ラープ値
	const int			 WAIT_TIME_BEFORE_START_FADE_IN;				//フェードイン開始前の待機時間
	const int			 WAIT_TIME_BEFORE_START_FADE_OUT;				//フェードアウト開始前の待機時間
	const int			 INCREASED_ALPHA_FOR_LOGO_FADE_IN;				//ロゴフェードイン用アルファ値増加量
	const int			 REDUCTION_ALPHA_FOR_LOGO_FADE_OUT;				//ロゴフェードアウト用アルファ値減少量
	const int			 REDUCTION_ALPHA_FOR_EFFECT_FADE_OUT;			//エフェクトフェードアウト用アルファ値減少量

	/*メンバ変数*/
	ProductStage currentProductStage;	//現在の演出の段階
	float		 logoAngle;				//ロゴのアングル
	float		 logoRate;				//ロゴの大きさ
	int			 logoAlpha;				//ロゴのアルファ値
	float		 effectLogoRate;		//エフェクト用のロゴの大きさ
	int			 effectLogoAlpha;		//エフェクトのアルファ値
	int			 waitTimer;				//待機時間(フレームごとに-1される)
	bool		 isProductFinished;		//演出が終了したか
	int			 questClearLogo;		//クエストクリア時のロゴ
};

