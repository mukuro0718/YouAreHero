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
	const VECTOR	GetPosition		  ()const;									  //座標の取得
	const VECTOR	GetDirection	  ()const { return this->direction; }		  //移動ベクトルの取得
	const bool		IsMove			  ()const;									  //移動フラグの取得
	const bool		IsAttack		  ()const;									  //ショットフラグの取得
	const bool		IsLightAttack	  ()const;									  //軽攻撃
	const bool		IsHeavyAttack	  ()const;									  //重攻撃
	const void		FixMoveVector	  (const VECTOR _fixVector);				  //移動ベクトルの修正
	const Collider	GetCollider		  ();										  //コライダーの取得
	const int		GetHP			  ()const { return this->hp; }				  //HPの取得
private:
	/*静的定数*/
	static constexpr int COUNT_NUM = 4;
	//プレイヤーの状態
	static constexpr unsigned int IDLE	   = (1 << 0); //待機
	static constexpr unsigned int REACTION = (1 << 1); //リアクション（攻撃を受けた時）
	static constexpr unsigned int DEATH    = (1 << 2); //死亡
	static constexpr unsigned int LOCK_ON  = (1 << 3); //ロックオン
	//立ち、座り
	static constexpr unsigned int STAND  = (1 << 4); //立ち
	static constexpr unsigned int CROUCH = (1 << 5); //座り
	//移動
	static constexpr unsigned int RUN	  = (1 << 6); //走り
	static constexpr unsigned int WALK	  = (1 << 7); //歩き
	//アクション
	static constexpr unsigned int AVOID	= (1 << 8); //回避
	static constexpr unsigned int JUMP  = (1 << 9); //ジャンプ
	static constexpr unsigned int BLOCK = (1 << 10); //ブロック
	//リアクション
	static constexpr unsigned int BIG_IMPACT   = (1 << 11); //大衝撃
	static constexpr unsigned int SMALL_IMPACT = (1 << 12); //小衝撃
	//攻撃
	static constexpr unsigned int CASTING				 = (1 << 13); //詠唱
	static constexpr unsigned int COMBO_ATTACK			 = (1 << 14); //コンボ
	static constexpr unsigned int CROUCH_SLASH			 = (1 << 15); //しゃがみ切り
	static constexpr unsigned int KICK					 = (1 << 16); //キック
	static constexpr unsigned int PUNCH					 = (1 << 17); //パンチ
	static constexpr unsigned int ROTATION_ATTACK		 = (1 << 18); //回転攻撃
	static constexpr unsigned int SLASH_1				 = (1 << 19); //切り１
	static constexpr unsigned int SLASH_2				 = (1 << 20); //切り２
	static constexpr unsigned int ROAR					 = (1 << 21);//咆哮
	//マスク
	static constexpr unsigned int MASK_MOVE = RUN | WALK; //移動マスク
	static constexpr unsigned int MASK_REACTION = BIG_IMPACT | SMALL_IMPACT;//リアクションマスク
	static constexpr unsigned int MASK_ATTACK = CASTING | COMBO_ATTACK | CROUCH_SLASH |
												KICK | PUNCH | ROTATION_ATTACK | SLASH_1 | SLASH_2;//攻撃マスク
	static constexpr unsigned int MASK_ALWAYS_INITIALIZE = BLOCK | STAND | CROUCH | IDLE;
	static constexpr unsigned int MASK_CANT_AVOID = MASK_ATTACK | MASK_REACTION | BLOCK | LOCK_ON | JUMP | AVOID;
	static constexpr unsigned int MASK_CANT_ROAR = MASK_ATTACK | MASK_REACTION | JUMP | ROAR;
	static constexpr unsigned int MASK_CANT_MAIN_ATTACK = MASK_ATTACK | MASK_REACTION | ROAR | BLOCK | AVOID | JUMP;
	static constexpr unsigned int MASK_CANT_SUB_ATTACK = MASK_ATTACK | MASK_REACTION | ROAR | AVOID | JUMP;
	static constexpr unsigned int MASK_CANT_IDLE = MASK_ATTACK | MASK_REACTION | ROAR | AVOID | JUMP | BLOCK | DEATH | RUN | WALK;
	static constexpr unsigned int MASK_CANT_JUMP = MASK_ATTACK | MASK_REACTION | ROAR | AVOID | JUMP | BLOCK | DEATH;

	/*列挙体*/
	//フレームカウントの種類
	enum class FrameCountType
	{
		SWITCH_MOVE_STATE = 0,
		ATTACK_INTERVAL = 1,//攻撃待機
		SWITCH_LOCK_ON = 2,//攻撃待機
		SWITCH_AVOID = 3,//回避
	};
	//アニメーションの種類
	enum class AnimationType
	{
		CROUCH_IDLE			 = 0,//座り待機
		STANDING_IDLE		 = 1,//立ち待機
		STANDING_IDLE_PLAY	 = 2,//立ち待機遊び
		WALK				 = 3,//歩き
		WALK_180_TURN		 = 4,//歩き180ターン
		RUN					 = 5,//走り
		RUN_180_TURN		 = 6,//走り180ターン
		LOCK_ON_RUN_BACK	 = 7,//ロックオン後ろ走り
		LOCK_ON_RUN_LEFT	 = 8,//ロックオン左走り
		LOCK_ON_RUN_RIGHT	 = 9,//ロックオン右走り
		LOCK_ON_WALK_BACK	 = 10,//ロックオン後ろ歩き
		LOCK_ON_WALK_LEFT	 = 11,//ロックオン左歩き
		LOCK_ON_WALK_RIGHT	 = 12,//ロックオン右歩き
		BLOCK				 = 13,//ブロック
		IDLE_JUMP			 = 14,//待機ジャンプ
		MOVE_JUMP			 = 15,//移動ジャンプ
		ROAR				 = 16,//咆哮
		BIG_IMPACT			 = 17,//大衝撃
		SMALL_IMPACT		 = 18,//小衝撃
		DEATH				 = 19,//デス

		CASTING				 = 20,//詠唱
		COMBO				 = 21,//コンボ
		CROUCH_SLASH		 = 22,//しゃがみ切り
		
		KICK				 = 23,//蹴り
		PUNCH				 = 24,//殴り
		ROTATION_ATTACK		 = 25,//回転攻撃
		SLASH_1				 = 26,//切り１
		SLASH_2				 = 27,//切り２

		AVOID				 = 28,//回避
	};

	/*内部処理関数*/
		  void StandOrSit		 ();		//立ち座りの切り替え
		  void UpdateVelocity	 ();		//速度の更新
		  void UpdateMoveVector	 ();		//移動ベクトルの更新
		  void UpdateRotation	 ();		//回転率の更新
		  void Move				 ();		//移動
		  void LockOn			 ();		//ロックオン
		  void Jump				 ();		//ジャンプ
		  void Block			 ();		//ブロック
		  void Avoid			 ();		//回避
		  void Roar				 ();		//咆哮
		  void Attack			 ();		//攻撃
		  void UpdateAnimation	 ();		//現在のアニメーションの更新
		  bool FrameCount(const int _index,const int _maxFrame);
	const bool CanRotation()const;
	const bool CanBlock()const;
	const bool CanAvoid()const;
	const bool CanRoar()const;
	const bool CanAttack()const;
	const bool CanJump()const;
	const bool DontAnyAction()const;
	/*メンバ変数*/

	Model*				model;						//モデル
	BitFlag*			state;				//状態
	Collider*			collider;					//コライダークラス
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
	int attackComboCount;
};

