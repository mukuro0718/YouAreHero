//===============================================
// @brief ボスクラス
//===============================================
#pragma once

class Character;
class Enemy;
class DemonBehaviorTree;
class CharacterData;
class CharacterColliderData;
class Demon : public Enemy
{
public:
	 Demon();//コンストラクタ
	~Demon();//デストラクタ

	void		Initialize			() override;		//初期化
	void		Finalize			() override;		//後処理
	void		Update				() override;		//更新
	const void	DrawCharacterInfo	() const override;	//描画
	void		PlayAnimation		();					//アニメーションの再生

	/*列挙体*/
	//アニメーション
	enum class AnimationType
	{
		DYING	,//ダイ
		IDLE	,//待機
	};

	/*getter/setter*/
	const bool	 GetIsAttack			()const override;													//コウゲキしたか
	const float	 GetAnimationPlayTime	()const;															//アニメーション再生時間の取得
	const float	 GetNowAnimationPlayTime()const { return this->animationPlayTime; }							//現在のアニメーション再生時間の取得
		  void	 SetRotation			(const VECTOR _rotation);											//回転率の設定
		  void	 SetVelocity			(const VECTOR _velocity);											//移動ベクトルの設定
		  void	 SetNowMoveTarget		(const VECTOR _moveTarget)	{ this->moveTarget = _moveTarget; }		//現在の移動目標を設定
		  void	 SetSpeed				(const float _speed)		{ this->speed = _speed; }				//速さの設定
		  void	 SetAnimationPlayTime	(const float _playTime)		{ this->animationPlayTime = _playTime; }//アニメーション再生時間の設定
		  void	 SetNowAnimation		(const int _animation)		{ this->nowAnimation = _animation; }	//現在のアニメーションの設定
		  void	 SetAttackCount			() override {}				//攻撃コンボの設定
private:
	/*静的定数*/
	static constexpr float SHADOW_HEIGHT = 10.0f;//影を投影する高さ
	static constexpr float SHADOW_SIZE	 = 8.0f; //影のサイズ
	static constexpr float LOCKON_OFFSET = 10.0f;//ロックオンオフセット

	/*メンバ変数*/
	float	animationPlayTime;	//アニメーション再生時間
	int		nowAnimation;		//現在のアニメーション
};

