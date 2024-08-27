//===============================================
// @brief ボスクラス
//===============================================
#pragma once

class Model;
class BitFlag;
class Collider;
class Boss
{
public:
	Boss();//コンストラクタ
	~Boss();//デストラクタ

	void Initialize();		//初期化
	void		Action();
	void		Update();	//更新
	const void	Draw()const;//描画
	void CalcDamage(const int _damage) { this->hp -= _damage; }
	const int		GetDamage()const;									  //ダメージの取得
	const int		GetHP()const { return this->hp; }				  //HPの取得

	const VECTOR GetPosition()const;
	const Collider GetCharacterCollider();
	const Collider GetAttackCollider();
	const int GetAttackNumber()const { return this->attackNumber; }
	const int GetHitNumber()const { return this->hitNumber; }
	void SetHitNumber(const int _attackNumber) { this->hitNumber = _attackNumber; }
	const bool IsAttack()const;
	void OnIsHitAttack() { this->isHitAttack = true; }
private:
	/*ファンクション*/
	typedef std::function<void(void)> FlagsState;//フラグごとの実行したい関数（引数、返り値無し）

	/*静的定数*/
	static constexpr int COUNT_NUM = 1;
	static constexpr int COLLIDER_NUM = 2;//コライダーの数

	static constexpr unsigned int IDLE						 = (1 << 0);//待機
	static constexpr unsigned int TAUNT						 = (1 << 1);//咆哮
	static constexpr unsigned int WALK_FRONT			 	 = (1 << 2);//前歩き
	static constexpr unsigned int WALK_LEFT					 = (1 << 3);//左歩き
	static constexpr unsigned int WALK_RIGHT				 = (1 << 4);//右歩き

	static constexpr unsigned int VERTICAL_SLASH			 = (1 << 5);//縦切り
	static constexpr unsigned int HORIZONTAL_SLASH			 = (1 << 6);//横切り
	static constexpr unsigned int ROTATION_SLASH			 = (1 << 7);//回転切り
	static constexpr unsigned int KNOCK_UP_SLASH			 = (1 << 8);//下から上切り
	static constexpr unsigned int STRONG_HORIZONTAL_SLASH	 = (1 << 9);//強い横切り
	static constexpr unsigned int TWO_COMBO					 = (1 << 10);//１２コンボ
	static constexpr unsigned int THREE_COMBO				 = (1 << 11);//１２３コンボ
	static constexpr unsigned int REACTION					 = (1 << 12);//被ダメージ

	static constexpr unsigned int DEATH						 = (1 << 13);//デス
	static constexpr unsigned int REST						 = (1 << 14);//休憩

	static constexpr unsigned int MASK_MOVE = WALK_FRONT | WALK_LEFT | WALK_RIGHT;
	static constexpr unsigned int MASK_ATTACK = VERTICAL_SLASH | HORIZONTAL_SLASH | ROTATION_SLASH |
												KNOCK_UP_SLASH | STRONG_HORIZONTAL_SLASH |
												TWO_COMBO | THREE_COMBO;
	static constexpr unsigned int MASK_ALL = MASK_MOVE | MASK_ATTACK | REACTION | DEATH | REST | TAUNT | IDLE;
	
	/*クラス*/
	class FlagsStateSet
	{
	public:
		FlagsState update;
	};
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
		REST = 0,
	};
	enum class AttackType
	{
		NONE					= -1,
		VERTICAL_SLASH			= 0,//縦切り
		HORIZONTAL_SLASH		= 1,//横切り
		ROTATION_SLASH			= 2,//回転切り
		KNOCK_UP_SLASH			= 3,//下から上切り
		STRONG_HORIZONTAL_SLASH = 4,//強い横切り
		TWO_COMBO				= 5,//１２コンボ
		THREE_COMBO				= 6,//１２３コンボ
	};
	enum class AnimationType
	{
		IDLE					= 0,//待機
		TAUNT					= 1,//咆哮
		WALK_FRONT				= 2,//前歩き
		WALK_LEFT				= 3,//左歩き
		WALK_RIGHT				= 4,//右歩き
		VERTICAL_SLASH			= 5,//縦切り
		HORIZONTAL_SLASH		= 6,//横切り
		ROTATION_SLASH			= 7,//回転切り
		KNOCK_UP_SLASH			= 8,//下から上切り
		STRONG_HORIZONTAL_SLASH = 9,//強い横切り
		TWO_COMBO				= 10,//１２コンボ
		THREE_COMBO				= 11,//１２３コンボ
		REACTION = 12,
		DEATH = 13,
	};

	/*内部処理関数*/
		  void Idle				(){}
		  void Taunt			();//咆哮
		  void UpdateRotation	();//回転率の更新
		  void UpdateMoveVector	();//移動ベクトルの更新
		  void UpdateVelocity	();//移動ベクトルの更新
		  void Move				();//移動
		  void Attack			();//攻撃
		  void Rest				();
		  void Death			();
		  void Reaction			();
		  void DecideOfAttack	();//攻撃を決める
		  void RushAttack		();//突進攻撃
		  void JumpAttack		();//ジャンプ攻撃
		  void ChangeState		();
		  bool FrameCount		(const int _index, const int _maxFrame);
	unsigned int GetState		();
	const bool CanAttack		()const;//攻撃できるか
	const bool CanRotation		()const;//回転
	const bool CanMove			()const;//移動
	const bool CanRest			()const;//休憩できるか
	void AddItemFunction(const unsigned int _item, const FlagsState _update);//項目ごとの関数の追加
	
	/*メンバ変数*/
	Model*						model;					//モデル
	BitFlag*					state;					//状態
	Collider*					collider[COLLIDER_NUM];	//コライダー
	float						velocity;				//速度
	VECTOR						moveVector;				//移動ベクトル
	VECTOR						direction;
	VECTOR						moveTarget;				//移動目標
	int							stamina;				//スタミナ
	std::map<unsigned int, FlagsStateSet>stateFunctionMap;//項目ごとの関数
	std::map<unsigned int, int> stateAnimationMap;//項目ごとのアニメーション
	std::map<int, unsigned int> phase1AttackMap;
	std::map<int, unsigned int> phase2AttackMap;
	std::vector<int>	frameCount;					//フレームカウント
	std::vector<bool>	isCount;					//カウントをするか

	int nowAnimation;
	float animationPlayTime;
	float dot;

	float targetRange;
	int attackComboCount;//攻撃コンボ回数
	float jumpPower;
	bool isHitAttack;
	int hp;
	int damage;
	int hitNumber;
	int attackNumber;
	bool isDraw;
};

