//===============================================
// @brief プレイヤークラス
//===============================================
#pragma once

class Character;
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
	static constexpr int COUNT_NUM = 3;//フレームカウントの数
	//プレイヤーの状態
	static constexpr unsigned int IDLE			  = (1 << 0);  //待機
	static constexpr unsigned int AVOID_BACK	  = (1 << 1);  //回避
	static constexpr unsigned int AVOID_FRONT	  = (1 << 2);  //回避
	static constexpr unsigned int AVOID_LEFT	  = (1 << 3);  //回避
	static constexpr unsigned int AVOID_RIGHT	  = (1 << 4);  //回避
	static constexpr unsigned int DEATH			  = (1 << 5);  //デス
	static constexpr unsigned int BLOCK			  = (1 << 6);  //ブロック
	static constexpr unsigned int REACTION		  = (1 << 7);  //リアクション
	static constexpr unsigned int BLOCK_REACTION  = (1 << 8);  //ブロックリアクション
	static constexpr unsigned int STUNNED		  = (1 << 9);  //スタン（大ダウン）
	static constexpr unsigned int KIP_UP		  = (1 << 10);  //起き上がり
	static constexpr unsigned int STAB_REACT	  = (1 << 11);  //突き刺し攻撃に対するリアクション
	static constexpr unsigned int THRUST_UP_REACT = (1 << 12);  //アッパー攻撃に対するリアクション
	static constexpr unsigned int WALK_BACK		  = (1 << 13); //歩き
	static constexpr unsigned int WALK_FRONT	  = (1 << 14); //歩き
	static constexpr unsigned int WALK_LEFT		  = (1 << 15); //歩き
	static constexpr unsigned int WALK_RIGHT	  = (1 << 16); //歩き
	static constexpr unsigned int SLASH			  = (1 << 17); //攻撃
	//マスク
	static constexpr unsigned int MASK_REACTION				 = BLOCK_REACTION | REACTION | STUNNED | KIP_UP;//リアクション
	static constexpr unsigned int MASK_MOVE					 = WALK_FRONT | WALK_BACK | WALK_LEFT | WALK_RIGHT; //移動マスク
	static constexpr unsigned int MASK_AVOID				 = AVOID_BACK | AVOID_FRONT | AVOID_LEFT | AVOID_RIGHT; //移動マスク
	static constexpr unsigned int MASK_CANT_RECOVERY_STAMINA = MASK_AVOID | BLOCK;
	static constexpr unsigned int MASK_ATTACK				 = SLASH;
	static constexpr unsigned int MASK_ALWAYS_TURN_OFF		 = MASK_MOVE | IDLE;
	static constexpr unsigned int MASK_ALL					 = MASK_MOVE | IDLE | MASK_ATTACK | MASK_REACTION | BLOCK | MASK_AVOID | DEATH;
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
		INVINCIBLE = 0,//無敵時間
		HEAL = 1,//回復
		AVOID = 2,//回避
	};
	//アニメーションの種類
	enum class AnimationType
	{
		IDLE			= 0,
		AVOID_BACK		= 1,
		AVOID_FRONT		= 2,
		AVOID_LEFT		= 3,
		AVOID_RIGHT		= 4,
		DEATH			= 5,
		BLOCK			= 6,
		REACTION		= 7,
		BLOCK_REACTION  = 8,
		STUNNED			= 9,
		KIP_UP			= 10,
		STAB_REACT		= 11,
		THRUST_UP_REACT = 12,
		WALK_BACK		= 13,
		WALK_FRONT		= 14,
		WALK_LEFT		= 15,
		WALK_RIGHT		= 16,
		SLASH			= 17,
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
	void Heal();
	//許可フラグ
	const bool CanRotation()const;
	const bool CanRolling()const;
	const bool CanAttack()const;
	const bool CanBlock()const;
	const bool DontAnyAction()const;

		  void UpdateAnimation	 ();		//現在のアニメーションの更新
		  bool FrameCount(const int _index,const int _maxFrame);
	const bool CanAction(const float _staminaConsumed)const;
	void CalcStamina(const float _staminaConsumed);//スタミナの回復処理
	/*メンバ変数*/

	VECTOR				moveVectorRotation;			//移動ベクトル用回転値
	std::vector<int>	frameCount;					//フレームカウント
	std::vector<bool>	isCount;					//カウントをするか
	std::map<unsigned int, int> animationMap;//アニメーションマップ
	std::map<int, unsigned int> reactionMap;//リアクションマップ
	int					nowAnimation;				//アニメーション
	float				animationPlayTime;			//アニメーション再生時間
	int					attackComboCount;
	int					healOrbNum;					//回復オーブの数
	bool				isHeal;						//回復しているか
};

