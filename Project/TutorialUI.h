//==============================================================================
// @brief チュートリアルUIクラス
//	画像・テキストの描画と更新処理を行う
//==============================================================================
#pragma once

class SceneUI;
class TutorialUI : public SceneUI
{
public:
	TutorialUI();//コンストラクタ
	~TutorialUI();//デストラクタ

	void Initialize() override;
	void Update() override;
	const void Draw()const override;
private:

	/*メンバ変数*/
	//画像変数
	Image logo;
	Image pressA;
};

/*
タイトルロゴ

alpha = 0 から MAX_ALPHA まで毎フレーム+1する
alpha が増加中にプレイヤーが X を押したら alpha を一気に MAX_ALPHA にする
*/

/*
pressA

タイトルロゴの alpha が MAX_ALPHA に到達したら
alpha = 0 から MAX_ALPHA まで毎フレーム+1する
MAX_ALPHA に到達したら次は alpha が 0 になるまで毎フレーム -1 ずつ減少させる

Aボタンが押されたら一気に alpha を MAX_ALPHA にした後、
alpha が 0 になるまで毎フレーム -1 ずつ減少させる
*/