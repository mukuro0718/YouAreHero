//===============================================
// @brief プレイヤークラス
//===============================================
#pragma once

class Model;
class BitFlag;
class Collider;
class Player
{
public:
	Player();//コンストラクタ
	~Player();//デストラクタ

	void		Initialize();		//初期化
	void		Action	  ();		//アクション
	void		Update	  ();		//更新
	const void	Draw	  ()const;	//描画

	/*getter*/
	const int GetAttackNumber()const { return this->attackNumber; }
	const int GetHitNumber()const { return this->hitNumber; }
	void SetHitNumber(const int _attackNumber) { this->hitNumber = _attackNumber; }
	void CalcDamage(const int _damage);
	const int		GetDamage	  ()const;									  //ダメージの取得
	const VECTOR	GetPosition		  ()const;									  //座標の取得
	const VECTOR	GetDirection	  ()const { return this->direction; }		  //移動ベクトルの取得
	const bool		IsMove			  ()const;									  //移動フラグの取得
	const bool		IsAttack		  ()const;									  //ショットフラグの取得
	void		FixMoveVector	  (const VECTOR _fixVector);				  //移動ベクトルの修正
	const Collider	GetCharacterCollider		  ();										  //コライダーの取得
	const Collider	GetAttackCollider		  ();										  //コライダーの取得
	const int		GetHP			  ()const { return this->hp; }				  //HPの取得
private:
	

	/*静的定数*/
	static constexpr int COUNT_NUM = 4;//フレームカウントの数
	static constexpr int COLLIDER_NUM = 2;//コライダーの数
	//プレイヤーの状態
	static constexpr unsigned int IDLE	   = (1 << 0); //待機
	static constexpr unsigned int REACTION = (1 << 1); //リアクション（攻撃を受けた時）
	static constexpr unsigned int DEATH    = (1 << 2); //死亡
	static constexpr unsigned int LOCK_ON  = (1 << 3); //ロックオン
	//移動
	static constexpr unsigned int RUN	  = (1 << 6); //走り
	static constexpr unsigned int WALK	  = (1 << 7); //歩き
	//アクション
	static constexpr unsigned int AVOID	= (1 << 8); //回避
	static constexpr unsigned int BLOCK = (1 << 9); //ブロック
	//リアクション
	static constexpr unsigned int BIG_IMPACT   = (1 << 10); //大衝撃
	static constexpr unsigned int SMALL_IMPACT = (1 << 11); //小衝撃
	//攻撃
	static constexpr unsigned int MAIN_ATTACK_1				 = (1 << 12); //基本攻撃１
	static constexpr unsigned int MAIN_ATTACK_2				 = (1 << 13); //基本攻撃１
	static constexpr unsigned int MAIN_ATTACK_3				 = (1 << 14); //基本攻撃１
	static constexpr unsigned int SPECIAL_ATTACK			 = (1 << 15); //コンボ
	//マスク
	static constexpr unsigned int MASK_MOVE = RUN | WALK; //移動マスク
	static constexpr unsigned int MASK_REACTION = BIG_IMPACT | SMALL_IMPACT;//リアクションマスク
	static constexpr unsigned int MASK_ATTACK = MAIN_ATTACK_1 | MAIN_ATTACK_2 | MAIN_ATTACK_3 | SPECIAL_ATTACK;//攻撃マスク
	static constexpr unsigned int MASK_ALWAYS_INITIALIZE = BLOCK | IDLE;
	static constexpr unsigned int MASK_CANT_AVOID = MASK_ATTACK | MASK_REACTION | BLOCK | LOCK_ON | AVOID;
	static constexpr unsigned int MASK_CANT_MAIN_ATTACK = MASK_ATTACK | MASK_REACTION | BLOCK | AVOID ;
	static constexpr unsigned int MASK_CANT_IDLE = MASK_ATTACK | MASK_REACTION | AVOID | BLOCK | DEATH | RUN | WALK;
	static constexpr unsigned int MASK_CANT_JUMP = MASK_ATTACK | MASK_REACTION | AVOID | BLOCK | DEATH;

	static constexpr unsigned int MASK_ALL = MASK_MOVE | MASK_ATTACK | AVOID | IDLE | LOCK_ON;
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
		SWITCH_MOVE_STATE = 0,
		ATTACK_INTERVAL = 1,//攻撃待機
		SWITCH_LOCK_ON = 2,//攻撃待機
		SWITCH_AVOID = 3,//回避
	};
	enum class AttackType
	{
		NONE			 = -1,
		MAIN_1			 = 0,//詠唱
		MAIN_2			 = 1,//詠唱
		MAIN_3			 = 2,//詠唱
		SPECIAL			 = 3,//コンボ
	};
	//アニメーションの種類
	enum class AnimationType
	{
		IDLE				 = 0,//立ち待機
		IDLE_PLAY			 = 1,//立ち待機遊び
		WALK				 = 2,//歩き
		RUN					 = 3,//走り
		LOCK_ON_RUN_BACK	 = 4,//ロックオン後ろ走り
		LOCK_ON_RUN_LEFT	 = 5,//ロックオン左走り
		LOCK_ON_RUN_RIGHT	 = 6,//ロックオン右走り
		LOCK_ON_WALK_BACK	 = 7,//ロックオン後ろ歩き
		LOCK_ON_WALK_LEFT	 = 8,//ロックオン左歩き
		LOCK_ON_WALK_RIGHT	 = 9,//ロックオン右歩き
		BLOCK				 = 10,//ブロック
		AVOID				 = 11,//回避
		DEATH				 = 12,//デス
		BIG_IMPACT			 = 13,//大衝撃
		SMALL_IMPACT		 = 14,//小衝撃
		MAIN_1				 = 15,//詠唱
		MAIN_2				 = 16,//詠唱
		SPECIAL				 = 17,//コンボ
	};

	/*内部処理関数*/
		  void UpdateVelocity	 ();		//速度の更新
		  void UpdateMoveVector	 ();		//移動ベクトルの更新
		  void UpdateRotation	 ();		//回転率の更新
		  void Move				 ();		//移動
		  void LockOn			 ();		//ロックオン
		  void Block			 ();		//ブロック
		  void Avoid			 ();		//回避
		  void Attack			 ();		//攻撃
		  void Reaction();
		  void UpdateAnimation	 ();		//現在のアニメーションの更新
		  bool FrameCount(const int _index,const int _maxFrame);
	const bool CanRotation()const;
	const bool CanBlock()const;
	const bool CanAvoid()const;
	const bool CanAttack()const;
	const bool DontAnyAction()const;
	/*メンバ変数*/

	Model*				model;						//モデル
	BitFlag*			state;						//状態
	Collider*			collider[COLLIDER_NUM];		//コライダークラス
	VECTOR				moveVector;					//移動ベクトル
	VECTOR				direction;					//向いている方向
	VECTOR				fixVector;					//補正ベクトル
	VECTOR				moveVectorRotation;			//移動ベクトル用回転値
	VECTOR				wasd;						//wasd入力
	VECTOR				lStick;						//lStick入力(上:Z+ 下:Z- 左:x- 右:x+)
	float				velocity;					//速度
	std::vector<int>	frameCount;					//フレームカウント
	std::vector<bool>	isCount;					//カウントをするか
	std::map<unsigned int, int> attackAnimationMap;//攻撃アニメーションマップ
	std::map<int, unsigned int> attackComboStateMap;//コンボに応じて攻撃のビットフラグを返す
	float				jumpPower;					//ジャンプ力
	int					nowAnimation;				//アニメーション
	float				animationPlayTime;			//アニメーション再生時間
	int					hp;
	int damage;
	int attackComboCount;
	int hitNumber;
	int attackNumber;
	bool isDraw;
};

