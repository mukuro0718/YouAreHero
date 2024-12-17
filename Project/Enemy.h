//===============================================
// @brief ボスクラス
//===============================================
#pragma once

class Character;
class Enemy: public Character
{
public:
	 Enemy();//コンストラクタ
	~Enemy();//デストラクタ

	/*内部処理関数*/
	void Move			(const float _maxSpeed, const float _accel, const float _decel, const bool _isLerp);//移動
	void UpdateRotation	(VECTOR _toTarget);																	//回転率の更新
	void UpdateSpeed	(const float _maxSpeed, const float _accel, const float _decel);					//移動速度の更新
	void UpdateVelocity	(const bool _isLerp);																//移動ベクトルの更新

	/*getter/setter*/
	const VECTOR GetMoveTarget			()const { return this->moveTarget; }
	const float  GetAnimationPlayTime	()const;															//アニメーション再生時間の取得
	const float  GetNowAnimationPlayTime()const { return this->animationPlayTime; }							//現在のアニメーション再生時間の取得
		  void   SetRotation			(const VECTOR _rotation);											//回転率の設定
		  void   SetVelocity			(const VECTOR _velocity);											//移動ベクトルの設定
		  void   SetNowMoveTarget		(const VECTOR _moveTarget)	{ this->moveTarget = _moveTarget; }		//現在の移動目標を設定
		  void   SetSpeed				(const float _speed)		{ this->speed = _speed; }				//速さの設定
		  void   SetAnimationPlayTime	(const float _playTime)		{ this->animationPlayTime = _playTime; }//アニメーション再生時間の設定
		  void   SetNowAnimation		(const int _animation)		{ this->nowAnimation = _animation; }	//現在のアニメーションの設定
		  void   OffIsDraw				()							{ this->isDraw = false; }				//描画フラグを下す
protected:
	/*静的定数*/
	static constexpr VECTOR ROTATE_LERP_VALUE = { 0.1f,0.1f,0.1f };
	static constexpr VECTOR VELOCITY_LERP_VALUE = { 0.1f,0.1f,0.1f };

	/*メンバ変数*/
	VECTOR						moveTarget;				//移動目標
	float						animationPlayTime;		//アニメーション再生時間
	int							nowAnimation;			//現在のアニメーション
};

