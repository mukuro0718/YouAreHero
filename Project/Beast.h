//===============================================
// @brief ボスクラス
//===============================================
#pragma once

class Character;
class Enemy;
class BeastBehaviorTree;
class CharacterData;
class CharacterColliderData;
class Beast : public Enemy
{
public:
	 Beast();//コンストラクタ
	~Beast();//デストラクタ

	void		Initialize			() override;		//初期化
	void		Finalize			() override;		//後処理
	void		Update				() override;		//更新
	const void	DrawCharacterInfo	() const override;	//描画
	void		PlayAnimation		();					//アニメーションの再生

	/*列挙体*/
	//アニメーション
	enum class AnimationType
	{
		DYING					,//デス(97)
		IDLE					,//待機(111)
		ROAR					,//咆哮(通常→怒り状態の変化110)
		WALK					,//歩き(121)
		RUN						,//ダッシュ(117)
		TURN_LEFT				,//左を向く(119)
		TURN_RIGHT				,//右を向く(120)
		DOWN_START				,//ダウン(5)
		DOWN_LOOP				,//ダウン(4)
		DOWN_END				,//ダウン(3)
		LONG_FRIGHT_START		,//長い怯み(開始104)
		LONG_FRIGHT_LOOP		,//長い怯み(途中102)
		LONG_FRIGHT_END			,//長い怯み(終了100)
		SHORT_FRIGHT_START		,//短い怯み(開始107)
		SHORT_FRIGHT_LOOP		,//短い怯み(開始106)
		SHORT_FRIGHT_END		,//短い怯み(終了105)
		REST_START				,//休憩(開始89)
		REST_LOOP				,//休憩(途中90)
		REST_END				,//休憩(終了91)
		STEP_BACK				,//飛び下がる(8)
		RAISE_LEVEL				,//チャージレベルの上昇(14)
		BACKING_BREATH			,//下がりながらブレス(10)
		RUSH_START				,//突進(開始75)
		RUSH_LOOP				,//突進(途中76)
		RUSH_ROTATE_END			,//突進(回転して止まる77)
		RUSH_NORMAL_END			,//突進(普通に止まる78)
		CHARGE_RIGHT_ATTACK		,//右前足溜め攻撃(52)
		CHARGE_BOTH_ATTACK_START,//両前足溜め攻撃(開始50)
		CHARGE_BOTH_ATTACK_END	,//両前足溜め攻撃(終了51)
		BREATH_START			,//ブレス(開始45)
		BREATH_LOOP				,//ブレス(途中46)
		BREATH_END				,//ブレス(終了47)
		SUPER_NOVA_START		,//スーパーノヴァ(開始27)
		SUPER_NOVA_LOOP			,//スーパーノヴァ(途中28)
		SUPER_NOVA_END			,//スーパーノヴァ(終了29)
		WEAK_BREATH				,//弱ブレス(80)
		SMALL_EXPLOSION			,//小爆発(9)
		RIGHT_FOOT_ATTACK		,//右足攻撃(40)
		EXPLOSION				,//爆発攻撃（スーパーノヴァよりも小さい）(65)
		COMBO_ATTACK			,//右足で攻撃した後回転攻撃(22)
	};
	enum class Parts
	{
		HEAD,
		BODY,
		LEFT_HAND,
		RIGHT_HAND,
		LEFT_FOOT,
		RIGHT_FOOT,
	};

	/*getter/setter*/
	const bool		 GetIsAttack			()const override;													//コウゲキしたか
	const VECTOR GetNowMoveTarget()const { return this->moveTarget; }//移動目標の取得
		  void		 SetRotation			(const VECTOR _rotation);											//回転率の設定
		  void		 SetVelocity			(const VECTOR _velocity);											//移動ベクトルの設定
		  void		 SetNowMoveTarget		(const VECTOR _moveTarget)	{ this->moveTarget = _moveTarget; }		//現在の移動目標を設定
		  void		 SetSpeed				(const float _speed)		{ this->speed = _speed; }				//速さの設定
	const float		 GetAnimationPlayTime	()const;															//アニメーション再生時間の取得
	const float		 GetNowAnimationPlayTime()const { return this->animationPlayTime; }							//現在のアニメーション再生時間の取得
		  void		 SetAnimationPlayTime	(const float _playTime)		{ this->animationPlayTime = _playTime; }//アニメーション再生時間の設定
		  void		 SetNowAnimation		(const int _animation)		{ this->nowAnimation = _animation; }	//現在のアニメーションの設定
private:
	/*静的定数*/
	static constexpr float SHADOW_HEIGHT = 10.0f;//影を投影する高さ
	static constexpr float SHADOW_SIZE	 = 8.0f; //影のサイズ
	
	/*メンバ変数*/
	vector<CharacterColliderData*>  partsCollider;				//部位ごとのコライダー
	short							maxPartsColliderNum;		//コライダーの数
	VECTOR							moveTarget;					//移動目標
	float							animationPlayTime;			//アニメーション再生時間
	int								nowAnimation;				//現在のアニメーション
	vector<vector<short>>			frameIndexUsePartsColider;	//部位ごとのコライダーの指定に使用するフレーム番号
	vector<VECTOR>					pos1;						//部位ごとのコライダーの座標を入れている
	vector<VECTOR>					pos2;						//HACK:なぜかいったん代入しないとうまく入らない、なぜ
	vector<int>						prevPartsHp;				//前の部位ごとのHP
	int								maxHp;						//部位の最大HP
};

