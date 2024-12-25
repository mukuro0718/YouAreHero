//===========================================================================
// @brief ゲームシーンクラス
// ゲーム本編に必要な、各クラスのインスタンスを保持し、
// シーンベースクラスに基づいた実装を行う
//===========================================================================
#pragma once

class SceneBase;
class GameScene : public SceneBase
{
public:
	GameScene(); //コンストラクタ
	~GameScene();//デストラクタ

	void		Initialize() override;		//初期化
	void		Finalize  () override;		//最終処理
	void		Update	  () override;		//更新
	const void	Draw	  () const override;//描画

private:
	bool IsEnd();
};

