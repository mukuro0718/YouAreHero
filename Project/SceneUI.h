//=============================================================================================
// @brief シーン系のUIクラス
// タイトルロゴ、ゲームオーバー/クリア
// リザルトの描画を行う
//=============================================================================================
#pragma once

class SceneUI abstract
{
public:
			 SceneUI();//コンストラクタ
	virtual ~SceneUI(){}//デストラクタ

	virtual void		Initialize	() abstract;	  //初期化
	virtual	void		Update		() abstract;	  //更新
	virtual const void	Draw		() const abstract;//描画
	virtual const bool	IsEnd		() const abstract;//終了するか
protected:
	/*静的定数*/
	static constexpr short MAX_ALPHA = 255;//最大アルファ
	static constexpr short ALPHA_INCREASE = 2;//最大アルファ
	static constexpr short ALPHA_REDUCTION = 5;//最大アルファ
	static constexpr short MAX_X = 1920;//最大X
	static constexpr short MAX_Y = 1080;//最大Y
	
	/*メンバ変数*/
	short alphaForTransition;//遷移用アルファ
	bool isTransition;//遷移するか
	bool isEndFadeInForTransition;//遷移するか
};

