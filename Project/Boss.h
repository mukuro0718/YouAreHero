//===============================================
// @brief ボスクラス
//===============================================
#pragma once

class Character;
class BossAction;
class Boss : public Character
{
public:
	Boss();//コンストラクタ
	~Boss();//デストラクタ

	void		Initialize		 () override;		//初期化
	void		Finalize		 () override;		//後処理
	void		Update			 () override;		//更新
	const void	DrawCharacterInfo() const override;	//描画
	void		PlayAnimation();

	/*getter/setter*/
	const int    GetAngryState			()const { return this->angryState; }		//怒り状態の取得
	const VECTOR GetHeadPosition		()const;									//頭の座標を取得
	const float	 GetAnimationPlayTime	()const;									//アニメーション再生時間の取得
	const bool	 GetIsAttack			()const override;							//コウゲキしたか
	const float  GetSpeed				()const { return this->speed; }				//スピードの取得
	const VECTOR GetNowMoveTarget		()const { return this->moveTarget; }		//現在の移動目標の取得
	const int	 GetNowPhase			()const { return this->nowPhase; }			//今のフェー	ズの取得
	const int	 GetPrevPhase			()const { return this->prevPhase; }			//前のフェーズの取得
	const float  GetNowAnimationPlayTime()const { return this->animationPlayTime; }	//現在のアニメーション再生時間の取得
		  void	 SetRotation			(const VECTOR _rotation);											//回転率の設定
		  void	 SetVelocity			(const VECTOR _velocity);											//移動ベクトルの設定
		  void	 SetNowMoveTarget		(const VECTOR _moveTarget)	{ this->moveTarget = _moveTarget; }		//現在の移動目標を設定
		  void	 UnifyPhases			()							{ this->prevPhase = this->nowPhase; }	//フェーズを統一する
		  void	 SetSpeed				(const float _speed)		{ this->speed = _speed; }				//速さの設定
		  void	 SetAnimationPlayTime	(const float _playTime)		{ this->animationPlayTime = _playTime; }//アニメーション再生時間の設定
		  void	 SetNowAnimation		(const int _animation)		{ this->nowAnimation = _animation; }	//現在のアニメーションの設定
		  void	 OffIsDraw				()							{ this->isDraw = false; }				//描画フラグを下す

	/*列挙体*/
	//アニメーション
	enum class AnimationType
	{
		 DYING			= 0, //デス
		 IDLE			= 1, //待機
		 ROAR			= 2, //咆哮
		 WALK			= 3, //歩き
		 WALK_LEFT		= 4, //歩き
		 WALK_RIGHT		= 5, //歩き
		 SLASH			= 6, //スラッシュ
		 STAB			= 7, //突き刺し攻撃
		 ROTATE_PUNCH	= 8,//回転パンチ
	};
	//フェーズ
	enum class Phase
	{
		PHASE_1,
		PHASE_2,
		PHASE_3,
	};
	//攻撃の種類
	enum class AttackType
	{
		NONE		 = -1,
		SLASH		 = 0,//パンチ
		STAB		 = 1,//突き刺し攻撃
		ROTATE_PUNCH = 2,//回転パンチ
	};
	enum class AngryStateType
	{
		NORMAL,
		TIRED,
		ANGRY,
	};
private:
	/*静的定数*/
	static constexpr int COUNT_NUM = 6;
	//基本状態
	static constexpr unsigned int DYING	= (1 << 0);//デス
	static constexpr unsigned int IDLE	= (1 << 1);//待機
	static constexpr unsigned int ROAR	= (1 << 2);//咆哮
	static constexpr unsigned int WALK	= (1 << 3);//歩き
	static constexpr unsigned int REST	= (1 << 4);//休憩
	//攻撃
	static constexpr unsigned int SLASH			 = (1 << 5);//パンチ
	static constexpr unsigned int STAB			 = (1 << 6);//投石
	static constexpr unsigned int ROTATE_PUNCH	 = (1 << 7);//スラッシュ

	static constexpr unsigned int MASK_ATTACK = SLASH | ROTATE_PUNCH | STAB ;
	static constexpr unsigned int MASK_ALL	  = MASK_ATTACK | WALK | DYING | REST | REST | IDLE | ROAR;
	
	/*列挙体*/
	//コライダーの種類
	enum class ColliderType
	{
		CHARACTER = 0,
		ATTACK = 1,
	};
	//フレームカウントの種類
	enum class FrameCountType
	{
		SLASH			= 0,
		STAB			= 1,
		ROTATE_PUNCH	= 2,
		REST			= 3,
	};
	enum class ActionType
	{
		NONE = -1,
		DYING			= 0,//デス
		IDLE			= 1,//待機
		ROAR			= 2,//咆哮
		WALK			= 3,//歩き
		WALK_LEFT		= 4,//歩き
		WALK_RIGHT		= 5,//歩き
		REST			= 6,//歩き
		SLASH			= 7,//スラッシュ
		STAB			= 8,//突き刺し攻撃
		ROTATE_PUNCH	= 9,//回転パンチ
	};

	/*内部処理関数*/
	void ChangeState();
	void SetPhase	();
	void SetAngryState();

	/*メンバ変数*/
	std::map<int, unsigned int>	actionTypeMap;			//アクションタイプ
	std::vector<BossAction*>	parameters;				//アクションパラメーター
	VECTOR						moveTarget;				//移動目標
	float						animationPlayTime;		//アニメーション再生時間
	int							nowAnimation;			//現在のアニメーション
	int							nowPhase;				//現在のフェーズ
	int							prevPhase;				//前のフェーズ
	int							actionType;
	float angryValue;
	int angryState;
	int tiredInterval;
};

