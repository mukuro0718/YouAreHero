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
	void		Move();		//移動
	void		Update();	//更新
	const void	Draw()const;//描画

	const VECTOR GetPosition()const;
	const Collider GetCollider();
private:
	/*静的定数*/
	static constexpr int IDLE		= (1 >> 0);//待機
	static constexpr int TAUNT		= (1 >> 1);//咆哮
	static constexpr int WALK		= (1 >> 2);//歩き
	static constexpr int MASK_ALL = IDLE | TAUNT | WALK;

	/*列挙体*/
	enum class AnimationType
	{
		RUSH = 0,
		TAUNT = 1,
		WALK = 2,
	};

	/*内部処理関数*/
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

