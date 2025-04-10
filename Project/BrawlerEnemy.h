//====================================================
// @brief 素手での近接攻撃をする敵
//====================================================
#pragma once

class Enemy;
class BrawlerEnemy : public Enemy
{
public:
	 BrawlerEnemy(const int _indentNum, const int _bossType);//コンストラクタ
	~BrawlerEnemy();					//デストラクタ

	void		Initialize		 () override;		//初期化
	void		Finalize		 () override;		//後処理
	void		Update			 () override;		//更新
	const void	DrawCharacterInfo() const override;	//描画
	void		PlayAnimation	 ();				//アニメーションの再生

	/*列挙体*/
	//アニメーション
	enum class AnimationType
	{
		DYING,			//ダイ
		IDLE,			//待機
		RUN,			//走り
		WALK_FRONT,		//歩き
		WALK_BACK,		//歩き
		WALK_LEFT,		//歩き
		WALK_RIGHT,		//歩き
		ATTACK,			//攻撃
		REACTION,		//攻撃を受けた時のリアクション
		SWAY,			//回避
	};

	/*getter/setter*/
	const bool GetIsAttack	 ()const override;	//コウゲキしたか
		  void SetAttackCount() override {}		//攻撃コンボの設定
private:
	/*静的定数*/
	static constexpr float SHADOW_HEIGHT = 10.0f;//影を投影する高さ
	static constexpr float SHADOW_SIZE	 = 4.0f; //影のサイズ
	static constexpr float LOCKON_OFFSET = 5.0f;//ロックオンオフセット

	/*メンバ変数*/
	int indentNum;
	int bossType;
};

