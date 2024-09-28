//=============================================================================================
// @brief シーン系のUIクラス
// タイトルロゴ、ゲームオーバー/クリア
// リザルトの描画を行う
//=============================================================================================
#pragma once

class SceneUI abstract
{
public:
			 SceneUI(){}//コンストラクタ
	virtual ~SceneUI(){}//デストラクタ

	virtual void		Initialize	() abstract;	  //初期化
	virtual	void		Update		() abstract;	  //更新
	virtual const void	Draw		() const abstract;//描画
	virtual const bool	IsEnd		() const abstract;//終了するか
};

