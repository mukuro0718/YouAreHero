//===============================================
// @brief プレイヤークラス
//===============================================
#pragma once

class Model;
class BitFlag;
class Satellite;
class Shield;
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
	const int		GetCurrentElement ()const { return this->element; }			  //現在の属性の取得
	const bool		IsShowElementWheel()const { return this->isShowElementWheel; }//属性ホイールを表示するか
	const float GetElementAngle		  ()const { return this->elementAngle; }
private:
	/*静的定数*/
	static constexpr int SATELLITE_NUM	 = 2;//衛星の
	static constexpr unsigned int IDLE			 = (1 << 0); //待機
	static constexpr unsigned int RUN			 = (1 << 1); //歩き
	static constexpr unsigned int WALK			 = (1 << 2); //歩き
	static constexpr unsigned int AVOID			 = (1 << 3); //回避
	static constexpr unsigned int JUMP			 = (1 << 4); //ガード
	static constexpr unsigned int FLAME_ATTACK	 = (1 << 5); //重攻撃
	static constexpr unsigned int BOLT_ATTACK	 = (1 << 6); //重攻撃
	static constexpr unsigned int ICE_ATTACK	 = (1 << 7); //重攻撃
	static constexpr unsigned int EARTH_ATTACK	 = (1 << 8); //重攻撃
	static constexpr unsigned int STORM_ATTACK	 = (1 << 9); //重攻撃
	static constexpr unsigned int MASK_ATTACK	 = FLAME_ATTACK | BOLT_ATTACK | ICE_ATTACK | EARTH_ATTACK | STORM_ATTACK;
	static constexpr unsigned int MASK_MOVE		 = RUN | WALK;
	static constexpr unsigned int MASK_ALL		 = IDLE | MASK_MOVE | MASK_ATTACK | AVOID | JUMP;
	/*列挙体*/
	//フレームカウントの種類
	enum class FrameCountType
	{
		ATTACK_INTERVAL = 0,//攻撃待機
	};
	//アニメーションの種類
	enum class AnimationType
	{
		IDLE		 = 0,
		WALK		 = 1,
		RUN			 = 2,
		AVOID		 = 3,
		JUMP		 = 4,
		FLAME_ATTACK = 5,
		BOLT_ATTACK	 = 6,
		ICE_ATTACK	 = 7,
		EARTH_ATTACK = 8,
		STORM_ATTACK = 9,
	};
	enum class Element
	{
		FRAME,
		BOLT,
		ICE,
		EARTH,
		STORM,
	};
	/*内部処理関数*/
		  void UpdateVelocity	 ();		//速度の更新
		  void UpdateMoveVector	 ();		//移動ベクトルの更新
		  void UpdateRotation	 ();		//回転率の更新
		  void Attack			 ();		//攻撃
		  void Move				 ();		//移動
		  void Jump				 ();		//ジャンプ
		  void UpdateAnimation	 ();		//現在のアニメーションの更新
		  void UpdateElement	 ();
	const bool CanMove			 ()const;//移動できるか
	const bool CanAttack		 ()const;//攻撃できるか
	const bool CanJump			 ()const;//ジャンプできるか


	/*メンバ変数*/

	Model*				model;						//モデル
	BitFlag*			state;						//アクションの状態
	Satellite*			satellite[SATELLITE_NUM];	//衛星
	Shield*				shield;						//シールド
	Collider*			collider;					//コライダークラス
	VECTOR				moveVector;					//移動ベクトル
	VECTOR				direction;					//向いている方向
	VECTOR				fixVector;					//補正ベクトル
	VECTOR				moveVectorRotation;			//移動ベクトル用回転値
	VECTOR				wasd;						//wasd入力
	VECTOR				lStick;						//lStick入力
	float				velocity;					//速度
	std::vector<int>	frameCount;					//フレームカウント
	std::vector<bool>	isCount;					//カウントをするか
	std::map<unsigned int, int> animationMap;		//アニメーション
	std::map<int, unsigned int> attackTypeMap;
	bool				isShot;						//魔法を撃ったか
	float				jumpPower;					//ジャンプ力
	int					nowAnimation;				//アニメーション
	float				animationPlayTime;			//アニメーション再生時間
	int					hp;
	int					element;
	bool				isShowElementWheel;//属性ホイールの表示
	std::vector<int> rStick;
	float elementAngle;
};

