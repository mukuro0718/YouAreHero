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
	static constexpr int COUNT_NUM = 2;//フレームカウントの数
	//プレイヤーの状態
	static constexpr unsigned int IDLE			 = (1 << 0);  //待機
	static constexpr unsigned int ROLL			 = (1 << 1);  //回避
	static constexpr unsigned int DEATH			 = (1 << 2);  //デス
	static constexpr unsigned int BLOCK			 = (1 << 3);  //ブロック
	static constexpr unsigned int REACTION		 = (1 << 4);  //リアクション
	static constexpr unsigned int BLOCK_REACTION = (1 << 5);  //ブロックリアクション
	static constexpr unsigned int STUNNED		 = (1 << 6);  //スタン（大ダウン）
	static constexpr unsigned int KIP_UP		 = (1 << 7);  //起き上がり
	static constexpr unsigned int RUNNING		 = (1 << 8);  //走り
	static constexpr unsigned int WALK_FRONT	 = (1 << 9);  //歩き
	static constexpr unsigned int SLASH			 = (1 << 10); //攻撃
	//マスク
	static constexpr unsigned int MASK_REACTION				 = BLOCK_REACTION | REACTION | STUNNED | KIP_UP;//リアクション
	static constexpr unsigned int MASK_CANT_RECOVERY_STAMINA = ROLL | MASK_REACTION | RUNNING | BLOCK;		
	static constexpr unsigned int MASK_ATTACK				 = SLASH;
	static constexpr unsigned int MASK_MOVE					 = WALK_FRONT | RUNNING; //移動マスク
	static constexpr unsigned int MASK_ALWAYS_TURN_OFF		 = MASK_MOVE | IDLE;
	static constexpr unsigned int MASK_ALL					 = MASK_MOVE | IDLE | MASK_ATTACK | MASK_REACTION | BLOCK | ROLL | DEATH;
	static constexpr unsigned int MASK_CAN_VELOCITY			 = MASK_MOVE | ROLL;
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
		HEAL = 1,//回復
	};
	//アニメーションの種類
	enum class AnimationType
	{
		IDLE			= 0,
		ROLL			= 1,
		DEATH			= 2,
		BLOCK			= 3,
		REACTION		= 4,
		BLOCK_REACTION  = 5,
		STUNNED			= 6,
		KIP_UP			= 7,
		RUNNING			= 8,
		WALK_FRONT		= 9,
		SLASH			= 10,
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
	std::map<unsigned int, int> animationMap;
	int					nowAnimation;				//アニメーション
	float				animationPlayTime;			//アニメーション再生時間
	int					attackComboCount;
	int					healOrbNum;					//回復オーブの数
	bool				isHeal;						//回復しているか
};

