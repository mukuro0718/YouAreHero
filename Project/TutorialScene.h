//==================================================
// @brief チュートリアルシーン
//==================================================
#pragma once

class ResultUI;
class SceneBase;
class TutorialScene : public SceneBase
{
public:
	TutorialScene(); //コンストラクタ
	~TutorialScene();//デストラクタ

	void		Initialize() override;		//初期化
	void		Finalize() override;		//最終処理
	void		Update() override;		//更新
	const void	Draw() const override;//描画

private:
	/*静的定数*/
	static constexpr int TARGET_PRESS_BUTTON_COUNT = 2;

	/*内部処理関数*/
	bool IsEnd();

	/*メンバ変数*/
	bool isPrevPressAnyButton;//何かボタンが押されたか
	int pressButtonCount;
};

