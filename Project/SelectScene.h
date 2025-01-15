//==================================================
// @brief チュートリアルシーン
//==================================================
#pragma once

class SceneBase;
class SelectScene : public SceneBase
{
public:
	 SelectScene(); //コンストラクタ
	~SelectScene();//デストラクタ

	void		Initialize	() override;		//初期化
	void		Finalize	() override;		//最終処理
	void		Update		() override;		//更新
	const void	Draw		() const override;	//描画

private:
	/*内部処理関数*/
	bool IsEnd();
};

