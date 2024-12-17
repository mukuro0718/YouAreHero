//====================================================
// @brief ルクスリオ用ビヘイビアツリー
//====================================================
#pragma once
#include "Singleton.h"

class BehaviorTreeNode;
class BeastBehaviorTree : public Singleton<BeastBehaviorTree>
{
public:
	friend class Singleton<BeastBehaviorTree>;

	/*列挙体*/
	//キャラクターの状態
	enum class BeastState
	{
		ANGRY		= 0,
		NORMAL		= 1,
		DOWN		= 2,
		FRIGHTENING = 3,
		BREAK		= 4,
	};

	enum class ActionType
	{
		DYING				= 0, //デス
		IDLE				= 1, //待機
		ROAR				= 2, //咆哮(通常→怒り状態の変化)
		WALK				= 3, //歩き
		RUN					= 4, //ダッシュ
		TURN_LEFT			= 5, //左を向く
		TURN_RIGHT			= 6, //右を向く
		DOWN				= 7, //ダウン
		LONG_FRIGHT			= 8, //長い怯み
		SHORT_FRIGHT		= 9, //短い怯み
		REST				= 10,//休憩
		STEP_BACK			= 11,//後ろにジャンプして下がる
		RAISE_LEVEL			= 12,//炎のチャージ率を上昇させる（咆哮）
		BACKING_BREATH		= 13,//後ろに下がりながらブレス
		RUSH				= 14,//突進
		CHARGE_RIGHT_ATTACK	= 15,//右前足溜め攻撃
		CHARGE_BOTH_ATTACK	= 16,//両前足溜め攻撃
		BREATH				= 17,//ブレス
		SUPER_NOVA			= 18,//スーパーノヴァ
		WEAK_BREATH			= 19,//弱ブレス
		SMALL_EXPLOSION		= 20,//小爆発
		RIGHT_FOOT_ATTACK	= 21,//右足攻撃
		COMBO_ATTACK_1		= 22,//右足で攻撃した後回転攻撃
		COMBO_ATTACK_2		= 23,//3連足攻撃
	};
	//レベルの段階
	enum class LevelStage
	{
		Lv1 = 0,
		Lv2 = 1,
		Lv3 = 2,
		Lv4 = 3,
	};
	void Initialize	();//初期化
	void Update		();//更新

	/*getter/setter*/
	const int		 GetDownValue		()const				{ return this->downValue; }				//ダウン値の取得
	const int		 GetDamage			()const				{ return this->damage; }				//ダメージの取得
	const int		 GetPrevHp			()const				{ return this->prevHp; }				//前フレームのHPの取得
	const BeastState GetBeastState		()const				{ return this->state; }					//ボスの状態を取得
	const int		 GetInterval		(const int _index)	{ return this->intervalSet[_index]; }	//インターバルの取得
	const float		 GetToTargetDistance()const				{ return this->toTargetDistance; }		//目標までの距離を取得
	const int		 GetLevel			()const				{ return this->level; }					//レベルの取得
	void SetDownValue		(const int _set)						{ this->downValue = _set; }				//ダウン値をセット
	void SetDamage			(const int _set)						{ this->damage = _set; }				//ダメージをセット
	void SetPrevHp			(const int _set)			 			{ this->downValue = _set; }				//前フレームのHPをセット
	void SetBeastState		(const BeastState _state)				{ this->state = _state; }				//ボスの状態を設定
	void SetInterval		(const int _index,const int _set = 600)	{ this->intervalSet[_index] = _set; }	//インターバルのセット（）
	void SetToTargetDistance(const float _set)						{ this->toTargetDistance = _set; }		//目標までの距離をセット
	void RaiseLevel() { this->level++; }	//レベル上昇
	void ResetLevel() { this->level = 0; }	//レベルのリセット
private:
	/*内部処理関数*/
	 BeastBehaviorTree();//コンストラクタ
	~BeastBehaviorTree();//デストラクタ

	/*メンバ変数*/
	BehaviorTreeNode* root;		//ツリー
	BeastState		  state;	//ボスの状態
	int				  downValue;//ダウン値
	std::array<int, static_cast<int>(ActionType::COMBO_ATTACK_2) + 1> intervalSet;//インターバル
	int prevHp;//前フレームのHP
	int damage;//今までに受けたダメージ量（怒り状態が終了すると０にリセットする）
	int level;
	float toTargetDistance;//目標までの距離
};

/*
root--DyingNode
	|-ActionNode--Transition
				|-Recovering
				|-Serch
				|-Battle
*/