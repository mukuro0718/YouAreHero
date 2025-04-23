//===============================================
// @brief ボスクラス
//===============================================
#pragma once

class Character;
class BehaviorTree;
class Boid;
class Enemy: public Character
{
public:
	 Enemy();//コンストラクタ
	~Enemy();//デストラクタ

	void Move				(const float _maxSpeed, const float _accel, const float _decel, const bool _isLerp);//移動
	void UpdateRotation		(VECTOR _toTarget);																	//回転率の更新
	void UpdateSpeed		(const float _maxSpeed, const float _accel, const float _decel);					//移動速度の更新
	void UpdateVelocity		(const bool _isLerp);																//移動ベクトルの更新
	void ChangeAngryColor	();																					//怒り状態時の色に変更
	void ChangeTiredColor	();																					//疲れ状態時の色に変更
	void ChangeNormalColor	();																					//通常時の色に変更
	void DecAttackComboCount() { this->attackCount--; }															//攻撃回数を減らす

	/*boid関数*/
	Boid& GetBoid() { return *this->boid; }

	/*getter/setter*/
	const	VECTOR  GetSpownPosition		()const { return this->spownPosition; }		//スポーン位置を取得
	const	VECTOR	GetPositionForLockon	()const { return this->positionForLockon; }	//ロックオン用座標の取得
	const	int		GetAttackCount			()const { return this->attackCount; }		//残り攻撃コンボ回数の取得
	const	float	GetAnimationPlayTime	()const;									//アニメーション再生時間の取得
	const	VECTOR	GetMoveTarget			()const { return this->moveTarget; }		//移動目標
	const	float	GetNowAnimationPlayTime	()const { return this->animationPlayTime; }	//現在のアニメーション再生時間の取得
	const	int		GetBossState			()const { return this->bossState; }			//怒り状態の取得
			void	SetRotation				(const VECTOR _rotation);											//回転率の設定
			void	SetVelocity				(const VECTOR _velocity);											//移動ベクトルの設定
			void	SetNowMoveTarget		(const VECTOR _moveTarget)	{ this->moveTarget = _moveTarget; }		//現在の移動目標を設定
			void	SetSpeed				(const float _speed)		{ this->speed = _speed; }				//速さの設定
			void	SetAnimationPlayTime	(const float _playTime)		{ this->animationPlayTime = _playTime; }//アニメーション再生時間の設定
			void	SetNowAnimation			(const int _animation)		{ this->nowAnimation = _animation; }	//現在のアニメーションの設定
			void	OffIsDraw				()							{ this->isDraw = false; }				//描画フラグを下す
			void	OffIsHit				();//攻撃を受けた時に立つフラグを下す
	virtual void	SetAttackCount			() abstract;
	const	bool	GetIsCanAttack			() const;
			void	SetIsCanAttak			(const bool _isCanAttack);
		  /*列挙体*/
	enum class BossState
	{
		TIRED,
		NORMAL,
		ANGRY,
	};
protected:
	/*静的定数*/
	static constexpr VECTOR ROTATE_LERP_VALUE = { 0.1f,0.1f,0.1f };
	static constexpr VECTOR VELOCITY_LERP_VALUE = { 0.1f,0.1f,0.1f };

	/*内部処理関数*/


	/*メンバ変数*/
	BehaviorTree*	tree;				//ツリー
	Boid*			boid;
	VECTOR			positionForLockon;	//プレイヤーのロックオン用の座標
	VECTOR			moveTarget;			//移動目標
	float			animationPlayTime;	//アニメーション再生時間
	int				nowAnimation;		//現在のアニメーション
	COLOR_F			tiredColor;			//疲れ状態時カラー
	COLOR_F			angryColor;			//怒り状態時カラー
	COLOR_F			normalColor;		//通常状態時カラー
	int				bossState;			//ボスの状態
	int				attackCount;		//攻撃回数
	float			angryValue;			//怒りゲージ
	float			tiredValue;			//疲労ゲージ
	int				tiredDuration;		//疲れ状態を継続する時間
};

