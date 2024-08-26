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
	void		Move();		//移動
	void		Update();	//更新
	const void	Draw()const;//描画

	const VECTOR GetPosition()const;
	const Collider GetCollider();
private:
	/*静的定数*/
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
	static constexpr unsigned int STRONG_TWO_COMBO			 = (1 << 11);//強い１２コンボ横切り
	static constexpr unsigned int THREE_COMBO				 = (1 << 12);//１２３コンボ
	static constexpr unsigned int REACTION					 = (1 << 13);
	static constexpr unsigned int DEATH						 = (1 << 14);

	static constexpr unsigned int MASK_ATTACK = VERTICAL_SLASH | HORIZONTAL_SLASH | ROTATION_SLASH |
												KNOCK_UP_SLASH | STRONG_HORIZONTAL_SLASH |
												TWO_COMBO | STRONG_TWO_COMBO | THREE_COMBO;
	/*列挙体*/
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
		STRONG_TWO_COMBO		= 11,//強い１２コンボ横切り
		THREE_COMBO				= 12,//１２３コンボ
		REACTION = 13,
		DEATH = 14,
	};

	/*内部処理関数*/
	void Taunt();//咆哮
	void UpdateRotation();								//回転率の更新
	void UpdateMoveVector();
	void Attack();								//攻撃
	const bool CanMove()const;						//移動できるか
	const bool CanAttack()const;						//攻撃できるか
	void DecideOfAttack();//攻撃を決める
	void RushAttack();//突進攻撃
	void JumpAttack();//ジャンプ攻撃
	void SetState();
	bool IsRest();

	const bool CanRotation()const;
	
	/*メンバ変数*/
	std::map<unsigned int, std::function<void(void)>>	attackFunctionMap;//状態ごとの消費スタミナ
	std::map<int, unsigned int> attackTypeMap;
	Model*						model;					//モデル
	BitFlag*					state;					//状態
	Collider*					collider;				//コライダー
	float						velocity;				//速度
	VECTOR						moveVector;				//移動ベクトル
	VECTOR						moveTarget;				//移動目標
	int							stamina;				//スタミナ
	float targetRange;
	int waitingCountBeforeAttack;//攻撃前待機カウント
	int restCount;//休憩カウント
	int attackCount;//攻撃カウント
	bool isRest;
	int attackType;
	float jumpPower;

};

