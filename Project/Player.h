//===============================================
// @brief プレイヤークラス
//===============================================
#pragma once

class Character;
class HitStop;
class Player : public Character
{
public:
	Player();//コンストラクタ
	~Player();//デストラクタ

	void		Initialize		 () override;		//初期化
	void		Finalize		 () override;		//後処理
	void		Update			 () override;		//更新
	const void	DrawCharacterInfo()const override;	//描画

	/*getter*/
	const bool	IsMove		 ()const;//移動フラグの取得
	const bool	GetIsAttack	 ()const override;//ショットフラグの取得
	const int	GetStamina	 ()const;
	const int	GetHealOrbNum()const { return this->healOrbNum; }
private:
	/*静的定数*/
		/*静的定数*/
	const float SHADOW_HEIGHT = 10.0f;
	const float SHADOW_SIZE = 6.0f;
	static constexpr int COUNT_NUM = 5;//フレームカウントの数
	//プレイヤーの状態
	static constexpr unsigned int IDLE			  = (1 << 0);  //待機
	static constexpr unsigned int AVOID			  = (1 << 1);  //回避
	static constexpr unsigned int DEATH			  = (1 << 2);  //デス
	static constexpr unsigned int BLOCK			  = (1 << 3);  //ブロック
	static constexpr unsigned int REACTION		  = (1 << 4);  //リアクション
	static constexpr unsigned int BLOCK_REACTION  = (1 << 5);  //ブロックリアクション
	static constexpr unsigned int WALK_FRONT	  = (1 << 6);  //歩き
	static constexpr unsigned int WALK_BACK		  = (1 << 7);  //後ろ歩き
	static constexpr unsigned int WALK_LEFT		  = (1 << 8);  //左歩き
	static constexpr unsigned int WALK_RIGHT	  = (1 << 9);  //右歩き
	static constexpr unsigned int RUN_FRONT		  = (1 << 10); //前ダッシュ
	static constexpr unsigned int RUN_BACK		  = (1 << 11); //後ろダッシュ
	static constexpr unsigned int RUN_LEFT		  = (1 << 12); //左ダッシュ
	static constexpr unsigned int RUN_RIGHT		  = (1 << 13); //右ダッシュ
	static constexpr unsigned int RUN_180_TURN	  = (1 << 14); //180向き変更
	static constexpr unsigned int SLASH			  = (1 << 15); //攻撃
	static constexpr unsigned int HEAL			  = (1 << 16); //回復
	//マスク
	static constexpr unsigned int MASK_REACTION				 = BLOCK_REACTION | REACTION;//リアクション
	static constexpr unsigned int MASK_WALK					 = WALK_FRONT | WALK_BACK | WALK_LEFT | WALK_RIGHT; //移動マスク
	static constexpr unsigned int MASK_RUN					 = RUN_FRONT | RUN_BACK | RUN_LEFT | RUN_RIGHT; //移動マスク
	static constexpr unsigned int MASK_MOVE					 = MASK_WALK | MASK_RUN; //移動マスク
	static constexpr unsigned int MASK_AVOID				 = AVOID; //移動マスク
	static constexpr unsigned int MASK_ATTACK				 = SLASH;
	static constexpr unsigned int MASK_CANT_RECOVERY_STAMINA = MASK_AVOID | BLOCK | MASK_ATTACK;
	static constexpr unsigned int MASK_ALWAYS_TURN_OFF		 = MASK_MOVE | IDLE;
	static constexpr unsigned int MASK_ALL					 = MASK_MOVE | IDLE | MASK_ATTACK | MASK_REACTION | BLOCK | MASK_AVOID | HEAL;
	static constexpr unsigned int MASK_CAN_VELOCITY			 = MASK_MOVE | MASK_AVOID;
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
		JUST_AVOID	= 0,//ジャスト回避
		HEAL		= 1,//回復
		AVOID		= 2,//回避
		JUST_BLOCK	= 3,//ジャストガード
		LOCK_ON		= 4,//ロックオン
	};
	//アニメーションの種類
	enum class AnimationType
	{
		IDLE			= 0,
		AVOID			= 1,
		DEATH			= 2,
		BLOCK			= 3,
		REACTION		= 4,
		BLOCK_REACTION  = 5,
		WALK_FRONT		= 6,
		WALK_BACK		= 7,
		WALK_LEFT		= 8,
		WALK_RIGHT		= 9,
		RUN_FRONT		= 10,
		RUN_BACK		= 11,
		RUN_LEFT		= 12,
		RUN_RIGHT		= 13,
		RUN_180_TURN	= 14,
		SLASH			= 15,
		HEAL			= 16,
	};

	/*内部処理関数*/
	void UpdateSpeed	 ();//速度の更新
	void UpdateMoveVector();//移動ベクトルの更新
	void UpdateRotation	 ();//回転率の更新
	void Move			 ();//移動
	void Rolling		 ();//回避
	void Reaction		 ();//リアクション
	void Attack			 ();//攻撃
	void Death			 ();//デス
	void Block			 ();//ブロック
	void Heal			 ();//回復
	void LockOn			 ();//ロックオン
	//許可フラグ
	const bool CanRotation		()const;							//回転できるか
	const bool CanRolling		()const;							//回避できるか
	const bool CanAttack		()const;							//攻撃できるか
	const bool CanBlock			()const;							//ガードできるか
	const bool CanHeal			()const;							//回復できるか
	const bool DontAnyAction	()const;							//ほかにアクションを行っているか
	const bool CanAction		(const float _staminaConsumed)const;//アクションができるか

		  void UpdateAnimation	();										//現在のアニメーションの更新
		  void CalcStamina		(const float _staminaConsumed);			//スタミナの回復処理
		  bool FrameCount		(const int _index,const int _maxFrame);	//フレームカウント
	/*メンバ変数*/
	HitStop*							 hitStop;			//ヒットストップ
	VECTOR								 moveVectorRotation;//移動ベクトル用回転値
	std::vector<int>					 frameCount;		//フレームカウント
	std::vector<bool>					 isCount;			//カウントをするか
	std::map<unsigned int, int>			 animationMap;		//アニメーションマップ
	std::map<int, unsigned int>			 reactionMap;		//リアクションマップ
	std::map<unsigned int, unsigned int> whenRunMoveState;	//ダッシュ時の状態
	int									 nowAnimation;		//アニメーション
	float								 animationPlayTime;	//アニメーション再生時間
	int									 attackComboCount;	//攻撃コンボカウント
	int									 healOrbNum;		//回復オーブの数
	float								 dot;				//内積
	bool								 isLockOn;			//ロックオン
};

