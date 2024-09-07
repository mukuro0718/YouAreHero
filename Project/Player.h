//===============================================
// @brief プレイヤークラス
//===============================================
#pragma once

class BitFlag;
class Physics;
class Collidable;
class Animation;
class Player : public GoriLib::Collidable
{
public:
	Player();//コンストラクタ
	~Player();//デストラクタ

	void		Initialize(GoriLib::Physics* _physics);			//初期化
	void		Finalize  (GoriLib::Physics* _physics);			//後処理
	void		Update	  (GoriLib::Physics* _physics);			//更新
	void		OnCollide (const Collidable& _colider)override;	//衝突したとき
	const void	Draw	  ()const;								//描画

	/*getter*/
	const int		GetHitNumber()const { return this->attackNumber; }//攻撃番号の取得
	const int		GetDamage	()const;							  //ダメージの取得
	const VECTOR	GetPosition	()const;							  //座標の取得
	const VECTOR	GetDirection()const { return this->direction; }	  //移動ベクトルの取得
	const bool		IsMove		()const;							  //移動フラグの取得
	const bool		GetIsSlash ()const;							  //ショットフラグの取得
	const int		GetHP		()const;							  //HPの取得
	const float		GetStamina	()const;
private:
	

	/*静的定数*/
	static constexpr int COUNT_NUM = 4;//フレームカウントの数
	static constexpr int COLLIDER_NUM = 2;//コライダーの数
	//プレイヤーの状態
	static constexpr unsigned int IDLE			 = (1 << 0); //待機
	static constexpr unsigned int ROLL			 = (1 << 1); //回避
	static constexpr unsigned int DEATH			 = (1 << 2); //ブロック
	static constexpr unsigned int BLOCK			 = (1 << 3); //ブロック
	static constexpr unsigned int JUMP			 = (1 << 4); //ブロック
	static constexpr unsigned int REACTION		 = (1 << 5); //ブロック
	static constexpr unsigned int BLOCK_REACTION = (1 << 6); //ブロック
	static constexpr unsigned int RUNNING		 = (1 << 7); //歩き
	static constexpr unsigned int WALK_BACK		 = (1 << 8); //走り
	static constexpr unsigned int WALK_FRONT	 = (1 << 9); //歩き
	static constexpr unsigned int WALK_LEFT		 = (1 << 10); //歩き
	static constexpr unsigned int WALK_RIGHT	 = (1 << 11); //歩き
	static constexpr unsigned int SLASH			 = (1 << 12); //歩き
	//マスク
	static constexpr unsigned int MASK_CANT_RECOVERY_STAMINA = ROLL | REACTION;
	static constexpr unsigned int MASK_ATTACK = SLASH;
	static constexpr unsigned int MASK_REACTION = BLOCK_REACTION | REACTION;
	static constexpr unsigned int MASK_MOVE = WALK_BACK | WALK_FRONT | WALK_LEFT | WALK_RIGHT | RUNNING; //移動マスク
	static constexpr unsigned int MASK_ALL = MASK_MOVE | IDLE ;
	static constexpr unsigned int MASK_CAN_VELOCITY = MASK_MOVE | ROLL | JUMP;
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
		AVOID = 0,//回避
	};
	//アニメーションの種類
	enum class AnimationType
	{
		IDLE			= 0,
		ROLL			= 1,
		DEATH			= 2,
		BLOCK			= 3,
		JUMP			= 4,
		REACTION		= 5,
		BLOCK_REACTION  = 6,
		RUNNING			= 7,
		WALK_BACK		= 8,
		WALK_FRONT		= 9,
		WALK_LEFT		= 10,
		WALK_RIGHT		= 11,
		SLASH			= 12,
	};

	/*内部処理関数*/
	void UpdateSpeed	 ();//速度の更新
	void UpdateMoveVector();//移動ベクトルの更新
	void UpdateRotation	 ();//回転率の更新
	void Move			 ();//移動
	void LockOn			 ();//ロックオン
	void Rolling		 ();//回避
	void Reaction		 ();//リアクションｎ
	void Attack			 ();//攻撃
	void Death			 ();//デス
	void Block			 ();//ブロック
	void Jump			 ();//ジャンプ
	//許可フラグ
	const bool CanRotation()const;
	const bool CanRolling()const;
	const bool CanAttack()const;
	const bool CanBlock()const;
	const bool CanJump()const;
	const bool DontAnyAction()const;

		  void UpdateAnimation	 ();		//現在のアニメーションの更新
		  bool FrameCount(const int _index,const int _maxFrame);
	const bool CanAction(const float _staminaConsumed)const;
	void CalcStamina(const float _staminaConsumed);//スタミナの回復処理
	/*メンバ変数*/

	BitFlag*			state;						//状態
	Animation*			animation;					//アニメーション
	VECTOR				direction;					//向いている方向
	VECTOR				moveVectorRotation;			//移動ベクトル用回転値
	std::vector<int>	frameCount;					//フレームカウント
	std::vector<bool>	isCount;					//カウントをするか
	int attackType;
	std::map<unsigned int, int> animationMap;
	float				jumpPower;					//ジャンプ力
	int					nowAnimation;				//アニメーション
	float				animationPlayTime;			//アニメーション再生時間
	int damage;
	int attackComboCount;
	int attackNumber;
	bool isDraw;


	int modelHandle;
	int prevHitNum;
	float speed;
	bool isGround;
	float stamina;
	bool isLockOn;
};

