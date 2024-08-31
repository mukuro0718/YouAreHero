//===============================================
// @brief ボスクラス
//===============================================
#pragma once

class BitFlag;
class Physics;
class Collidable;
class Animation;
class Boss : public GoriLib::Collidable
{
public:
	Boss();//コンストラクタ
	~Boss();//デストラクタ

	void		Initialize(GoriLib::Physics* _physics);		//初期化
	void		Finalize(GoriLib::Physics* _physics);		//後処理
	void		Update(GoriLib::Physics* _physics);		//更新
	void		OnCollide(const Collidable& _colider)override;//衝突したとき
	const void	Draw()const;//描画
	const int		GetDamage()const;									  //ダメージの取得
	const int		GetHP()const;				  //HPの取得
	const VECTOR GetHeadPosition()const;
	const VECTOR GetPosition()const;
	const int GetAttackNumber()const { return this->attackNumber; }
	const int GetHitNumber()const { return this->hitNumber; }
	void SetHitNumber(const int _attackNumber) { this->hitNumber = _attackNumber; }
	const bool IsAttack()const;
	void OnIsHitAttack() { this->isHitAttack = true; }
private:
	/*ファンクション*/
	typedef std::function<void(void)> FlagsState;//フラグごとの実行したい関数（引数、返り値無し）

	/*静的定数*/
	static constexpr int COUNT_NUM = 2;
	static constexpr int COLLIDER_NUM = 2;//コライダーの数
	//基本状態
	static constexpr unsigned int DYING	= (1 << 0);//デス
	static constexpr unsigned int IDLE	= (1 << 1);//待機
	static constexpr unsigned int ROAR	= (1 << 2);//咆哮
	static constexpr unsigned int WALK	= (1 << 3);//歩き
	static constexpr unsigned int REST	= (1 << 4);//休憩
	//フェーズ１
	static constexpr unsigned int PUNCH		  = (1 << 5);//パンチ
	static constexpr unsigned int SLASH		  = (1 << 6);//スラッシュ
	static constexpr unsigned int THROW_STORN = (1 << 7);//投石
	//フェーズ２
	static constexpr unsigned int HURRICANE_KICK = (1 << 8);//回転蹴り
	static constexpr unsigned int GRAND_SLAM	 = (1 << 9);//地面をたたく
	static constexpr unsigned int FLAME_MAGIC	 = (1 << 10);//火炎放射
	static constexpr unsigned int LIGHTNING		 = (1 << 11);//ランダムで落雷
	//フェーズ３
	static constexpr unsigned int CONTINUOUS_SLASH = (1 << 12);//連続スラッシュ
	static constexpr unsigned int COMBO			   = (1 << 12);//コンボ（ランダムな攻撃を組み合わせる）
	static constexpr unsigned int DARK_FIELD	   = (1 << 13);//ダークフィールド（範囲内にダメージ）
	static constexpr unsigned int METEO			   = (1 << 14);//隕石

	static constexpr unsigned int MASK_ATTACK = PUNCH | SLASH | THROW_STORN |HURRICANE_KICK | GRAND_SLAM |FLAME_MAGIC | LIGHTNING 
											   | CONTINUOUS_SLASH | COMBO | DARK_FIELD | METEO;
	static constexpr unsigned int MASK_ALL = MASK_ATTACK | WALK | DYING | REST | REST | IDLE;
	
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
		ATTACK = 1,
	};
	enum class AttackType
	{
		NONE			 = -1,
		PUNCH			 = 0,//パンチ
		SLASH			 = 1,//スラッシュ
		THROW_STORN		 = 2,//投石
		HURRICANE_KICK	 = 3,//回転蹴り
		GRAND_SLAM		 = 4,//地面をたたく
		FLAME_MAGIC		 = 5,//火炎放射
		LIGHTNING		 = 6,//落雷
		CONTINUOUS_SLASH = 7,//連続切り
		DARK_FIELD		 = 8,//ダークフィールド
		METEO			 = 9,//隕石
	};
	enum class AnimationType
	{
		DYING			 = 0,//デス
		IDLE			 = 1,//待機
		ROAR			 = 2,//咆哮
		WALK			 = 3,//歩き
		PUNCH			 = 4,//パンチ
		SLASH			 = 5,//スラッシュ
		THROW_STORN		 = 6,//投石
		HURRICANE_KICK	 = 7,//回転蹴り
		GRAND_SLAM		 = 8,//地面をたたく
		FLAME_MAGIC		 = 9,//火炎放射
		LIGHTNING		 = 10,//落雷
		CONTINUOUS_SLASH = 11,//連続切り
		DARK_FIELD		 = 12,//ダークフィールド
		METEO			 = 13,//隕石
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
	Animation*					animation;					//モデル
	BitFlag*					state;					//状態
	float						speed;				//速度
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

	int modelHandle;
	int nowAnimation;
	float animationPlayTime;
	float dot;
	bool isGround;
	float targetRange;
	int attackComboCount;//攻撃コンボ回数
	float jumpPower;
	bool isHitAttack;
	int hitNumber;
	int attackNumber;
	bool isDraw;
	int attackType;

};

