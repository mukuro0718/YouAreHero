//===============================================
// @brief ボスクラス
//===============================================
#pragma once

class Character;
class BossAction;
class Boss : public Character
{
public:
	Boss();//コンストラクタ
	~Boss();//デストラクタ

	void		Initialize		 () override;		//初期化
	void		Finalize		 () override;		//後処理
	void		Update			 () override;		//更新
	const void	DrawCharacterInfo() const override;//描画

	/*getter*/
	const float  GetSpeed				()const { return this->speed; }
	const VECTOR GetHeadPosition		()const;
	const VECTOR GetNowMoveTarget		()const { return this->moveTarget; }
	const bool	 GetIsAttack			()const override;
	const int	 GetNowPhase			()const { return this->nowPhase; }									//今のフェー	ズの取得
	const int	 GetPrevPhase			()const { return this->prevPhase; }								//前のフェーズの取得
	const float	 GetAnimationPlayTime	()const;															//アニメーション再生時間の取得
	const float  GetNowAnimationPlayTime()const { return this->animationPlayTime; }
		  void	 UnifyPhases			() { this->prevPhase = this->nowPhase; }							//フェーズを統一する
		  void	 SetNowMoveTarget		(const VECTOR _moveTarget) { this->moveTarget = _moveTarget; }
		  void	 SetRotation			(const VECTOR _rotation);											//回転率の設定
		  void	 SetVelocity			(const VECTOR _velocity);											//移動ベクトルの設定
		  void	 SetSpeed				(const float _speed) { this->speed = _speed; }						//速さの設定
		  void	 SetAnimationPlayTime	(const float _playTime) { this->animationPlayTime = _playTime; }
private:
	/*静的定数*/
	static constexpr int COUNT_NUM = 6;
	//基本状態
	static constexpr unsigned int DYING	= (1 << 0);//デス
	static constexpr unsigned int IDLE	= (1 << 1);//待機
	static constexpr unsigned int ROAR	= (1 << 2);//咆哮
	static constexpr unsigned int WALK	= (1 << 3);//歩き
	static constexpr unsigned int REST	= (1 << 4);//休憩
	//攻撃
	static constexpr unsigned int SLASH			 = (1 << 5);//パンチ
	static constexpr unsigned int FLY_ATTACK	 = (1 << 6);//地面をたたく
	static constexpr unsigned int HURRICANE_KICK = (1 << 7);//回転蹴り
	static constexpr unsigned int JUMP_ATTACK	 = (1 << 8);//投石
	static constexpr unsigned int ROTATE_PUNCH	 = (1 << 9);//スラッシュ

	static constexpr unsigned int MASK_ATTACK = SLASH | ROTATE_PUNCH | JUMP_ATTACK |HURRICANE_KICK | FLY_ATTACK;
	static constexpr unsigned int MASK_ALL = MASK_ATTACK | WALK | DYING | REST | REST | IDLE | ROAR;
	
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
		SLASH = 0,
		FLY_ATTACK = 1,
		HURRICANE_KICK = 2,
		JUMP_ATTACK = 3,
		ROTATE_PUNCH = 4,
		REST = 5,
	};
	enum class Phase
	{
		PHASE_1,
		PHASE_2,
		PHASE_3,
	};
	enum class AttackType
	{
		NONE			 = -1,
		SLASH			 = 0,//パンチ
		FLY_ATTACK		 = 1,//地面をたたく
		HURRICANE_KICK	 = 2,//回転蹴り
		JUMP_ATTACK		 = 3,//突き刺し攻撃
		ROTATE_PUNCH	 = 4,//回転パンチ
	};
	enum class AnimationType
	{
		DYING			= 0,//デス
		IDLE			= 1,//待機
		ROAR			= 2,//咆哮
		WALK			= 3,//歩き

		SLASH			= 4,//スラッシュ
		FLY_ATTACK		= 5,//飛び攻撃
		HURRICANE_KICK	= 6,//回転蹴り
		JUMP_ATTACK		= 7,//突き刺し攻撃
		ROTATE_PUNCH	= 8,//回転パンチ
	};
	enum class ActionType
	{
		DYING			= 0,//デス
		IDLE			= 1,//待機
		ROAR			= 2,//咆哮
		WALK			= 3,//歩き
		REST			= 4,//歩き
		SLASH			= 5,//スラッシュ
		FLY_ATTACK		= 6,//飛び攻撃
		HURRICANE_KICK	= 7,//回転蹴り
		JUMP_ATTACK		= 8,//突き刺し攻撃
		ROTATE_PUNCH	= 9,//回転パンチ
	};

	/*内部処理関数*/
	void ChangeState();
	void SetPhase	();

	/*メンバ変数*/
	std::map<unsigned int, int>	stateAnimationMap;		//項目ごとのアニメーション
	std::map<int, unsigned int>	actionTypeMap;			//アクションタイプ
	std::vector<BossAction*>	parameters;				//アクションパラメーター
	VECTOR						moveTarget;				//移動目標
	float						animationPlayTime;		//アニメーション再生時間
	int							nowAnimation;			//現在のアニメーション
	int							nowPhase;				//現在のフェーズ
	int							prevPhase;				//前のフェーズ
	int							actionType;
};

