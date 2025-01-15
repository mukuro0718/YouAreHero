//===============================================
// @brief ボスクラス
//===============================================
#pragma once

class Character;
class Enemy;
class BossAction;
class Boss : public Enemy
{
public:
	Boss();//コンストラクタ
	~Boss();//デストラクタ

	void		Initialize			() override;		//初期化
	void		Finalize			() override;		//後処理
	void		Update				() override;		//更新
	const void	DrawCharacterInfo	() const override;	//描画
	void		PlayAnimation		();					//アニメーションの再生
	void		SetAttackComboCount	();					//攻撃コンボの設定

	void		DecAttackComboCount	() { this->attackComboCount--; }//攻撃コンボを減らす


	/*列挙体*/
	//アニメーション
	enum class AnimationType
	{
		 DYING			= 0, //デス
		 IDLE			= 1, //待機
		 ROAR			= 2, //咆哮
		 WALK			= 3, //歩き
		 WALK_LEFT		= 4, //歩き
		 WALK_RIGHT		= 5, //歩き

		 SLASH_1		= 6, //スラッシュ
		 SLASH_2		= 7, //スラッシュ
		 STAB			= 8, //突き刺し攻撃
		 ROTATE_SLASH	= 9, //回転スラッシュ
		 PUNCH			= 10,//パンチ
		 SLASH_COMBO_1	= 11,//スラッシュコンボ１
		 SLASH_COMBO_2	= 12,//スラッシュコンボ２
		 JUMP_ATTACK	= 13,//ジャンプアタック
		 DOWN			= 14,//ダウン
		 DOWN_UP		= 15 //ダウンから起き上がる
	};
	//攻撃の種類
	enum class AttackType
	{
		NONE		  = -1,
		SLASH_1		  = 0, //スラッシュ
		SLASH_2		  = 1, //スラッシュ
		STAB		  = 2, //突き刺し攻撃
		ROTATE_SLASH  = 3, //回転スラッシュ
		PUNCH		  = 4,//パンチ
		SLASH_COMBO_1 = 5,//スラッシュコンボ１
		SLASH_COMBO_2 = 6,//スラッシュコンボ２
		JUMP_ATTACK	  = 7,//ジャンプアタック
	};
	//怒り状態の種類
	enum class BossState
	{
		TIRED,
		NORMAL,
		ANGRY,
	};

	/*getter/setter*/
	const int		 GetAttackComboCount	()const { return this->attackComboCount; }	//残り攻撃コンボ回数の取得
	const int		 GetAngryState			()const { return this->angryState; }		//怒り状態の取得
	const bool		 GetIsAttack			()const override;							//コウゲキしたか
	const AttackType GetPrevAttackType		()const { return this->prevAttack; }		//前のアタックタイプの取得
		  void		 SetAttackType			(const AttackType _type)	{ this->prevAttack = _type; }			//攻撃の種類のセット
private:
	/*静的定数*/
	static constexpr float SHADOW_HEIGHT = 10.0f;//影を投影する高さ
	static constexpr float SHADOW_SIZE	 = 8.0f; //影のサイズ
	static constexpr int   COUNT_NUM	 = 6;	 //フレームカウントの数
	//基本状態
	static constexpr unsigned int DYING	= (1 << 0);//デス
	static constexpr unsigned int IDLE	= (1 << 1);//待機
	static constexpr unsigned int ROAR	= (1 << 2);//咆哮
	static constexpr unsigned int WALK	= (1 << 3);//歩き
	static constexpr unsigned int REST	= (1 << 4);//休憩
	//攻撃
	static constexpr unsigned int SLASH_1		= (1 << 5);//スラッシュ１
	static constexpr unsigned int SLASH_2		= (1 << 6);//スラッシュ２
	static constexpr unsigned int STAB			= (1 << 7);//突き刺し
	static constexpr unsigned int ROTATE_SLASH	= (1 << 8);//回転スラッシュ
	static constexpr unsigned int PUNCH			= (1 << 9);//パンチ
	static constexpr unsigned int SLASH_COMBO_1	= (1 << 10);//スラッシュコンボ１
	static constexpr unsigned int SLASH_COMBO_2	= (1 << 11);//スラッシュコンボ２
	static constexpr unsigned int JUMP_ATTACK	= (1 << 12);//ジャンプアタック

	static constexpr unsigned int MASK_ATTACK = SLASH_1 | SLASH_2 | ROTATE_SLASH |
												STAB | PUNCH | SLASH_COMBO_1 | SLASH_COMBO_2 | JUMP_ATTACK;
	static constexpr unsigned int MASK_ALL	  = MASK_ATTACK | WALK | DYING | REST | REST | IDLE | ROAR;
	
	/*列挙体*/
	//フレームカウントの種類
	enum class FrameCountType
	{
		SLASH			= 0,
		STAB			= 1,
		ROTATE_PUNCH	= 2,
		REST			= 3,
	};
	enum class ActionType
	{
		NONE			= -1,
		DYING			= 0,//デス
		IDLE			= 1,//待機
		ROAR			= 2,//咆哮
		WALK			= 3,//歩き
		REST			= 4,//休憩

		SLASH_1			= 5,//スラッシュ
		SLASH_2			= 6,//スラッシュ
		STAB			= 7,//突き刺し攻撃
		ROTATE_SLASH	= 8,//回転スラッシュ
		PUNCH			= 9,//パンチ
		SLASH_COMBO_1	= 10,//スラッシュコンボ１
		SLASH_COMBO_2	= 11,//スラッシュコンボ２
		JUMP_ATTACK		= 12,//ジャンプアタック
	};

	/*内部処理関数*/
	void ChangeState	();//状態の変更
	void SetAngryState	();//怒り状態の変更

	/*メンバ変数*/
	std::map<int, unsigned int>	actionTypeMap;			//アクションタイプ
	std::vector<BossAction*>	parameters;				//アクションパラメーター
	AttackType					prevAttack;				//前の攻撃
	float						angryValue;				//怒りゲージ
	int							nowAction;				//現在のアクション
	int							angryState;				//怒り状態
	int							tiredInterval;			//疲労インターバル
	int							attackComboCount;		//攻撃コンボ数
	int							frameTime;
	int							normalTexture;
	int							angryTexture;
	int							tiredTexture;
};

