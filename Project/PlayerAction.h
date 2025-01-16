//===================================================
// @プレイヤーアクションベースクラス
//===================================================
#pragma once

class Player;
class PlayerAction abstract
{
public:
			 PlayerAction(); //コンストラクタ
	virtual ~PlayerAction(){}//デストラクタ

	virtual void Initialize	() abstract;				//初期化
	virtual void Finalize	() abstract;				//後処理
	virtual void Update		(Player& _player) abstract;	//更新

	/*getter/setter*/
	const bool GetIsChangeAction()const { return this->isChangeAction; }//アクション変更フラグの取得
	const bool GetIsEndAction	()const { return this->isEndAction; }	//アクション変更フラグの取得
protected:
	/*構造体*/
	struct MoveData
	{
		VECTOR nextRotation = { 0.0f,0.0f,0.0f };
		float maxSpeed		= 0.0f;
		bool isSkip			= false;
		bool isLerp			= false;
		void Set(const VECTOR _nextRotation, const float _speed, const bool _isSkip, const bool _isLerp)
		{
			this->nextRotation = _nextRotation;
			this->maxSpeed = _speed;
			this->isSkip = _isSkip;
			this->isLerp = _isLerp;
		}
	};

	/*内部処理関数*/
	void	Move			(Player& _player, MoveData& _set);																//移動
	void	UpdateRotation	(const bool isSkip, VECTOR& _nextRotation, VECTOR& _nowRotation);								//回転率の更新
	void	UpdateSpeed		(float& _nowSpeed, const float _maxSpeed, const VECTOR nowRotation, const VECTOR _nextRotation);//移動速度の更新
	VECTOR	UpdateVelocity	(const VECTOR _rotation, const VECTOR _prevVelocity, const float _speed, const bool _isLerp);	//移動ベクトルの更新

	/*メンバ変数*/
	bool  isChangeAction;//アクションの変更ができる状態になればこのフラグを立てる
	bool  isEndAction;//アクションがすべて終了したらこのフラグを立てる
	short frameCount;
	int	  frameTime;//処理までにかかった時間
	float staminaRecoveryValue;
	float maxStamina;
	float playTime;
	int   nextAnimation;
	VECTOR rotateLerpValue;
	VECTOR velocityLerpValue;
	float accel;
	float decel;
	float totalPlayTime;
	float nowTotalPlayTime;
};

