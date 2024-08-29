//===========================================================================
// @brief ゲームシーンクラス
// ゲーム本編に必要な、各クラスのインスタンスを保持し、
// シーンベースクラスに基づいた実装を行う
//===========================================================================
#pragma once
#include "SceneBase.h"

class GoriLib::Physics;
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
	/*静的定数*/
	static constexpr unsigned int TITLE		= (1 << 0);//タイトル
	static constexpr unsigned int TUTORIAL	= (1 << 1);//チュートリアル
	static constexpr unsigned int GAME		= (1 << 2);//ゲーム
	static constexpr unsigned int RESULT	= (1 << 3);//リザルト

	/*内部処理関数*/
	void ChangeState();//ゲームの状態の変更

	unsigned int gameState;//ゲームの状態（本当はScene~にしたかったが、ほかの命名と被るため変更）

	class GoriLib::Physics* physics;
};

