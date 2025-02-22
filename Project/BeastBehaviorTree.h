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
		EXPLOSION			= 22,//爆発攻撃（スーパーノヴァよりも小さい)
		COMBO_ATTACK		= 23,//3連足攻撃
	};
	//レベルの段階
	enum class LevelStage
	{
		Lv1 = 0,
		Lv2 = 1,
		Lv3 = 2,
		Lv4 = 3,
	};
		  void Initialize	();		//初期化
		  void Update		();		//更新
	const void Draw			()const;//描画

	/*getter/setter*/
	const int		 GetDamage					()const				{ return this->damage; }						 //ダメージの取得
	const int		 GetPrevHp					()const				{ return this->prevHp; }						 //前フレームのHPの取得
	const int		 GetInterval				(const int _index)	{ return this->intervalSet[_index]; }			 //インターバルの取得
	const float		 GetToTargetDistance		()const				{ return this->toTargetDistance; }				 //目標までの距離を取得
	const int		 GetLevel					()const				{ return this->level; }							 //レベルの取得
	const bool		 GetIsDestroyedPart			()const				{ return this->isDestroyedPart; }				 //部位破壊されているか
	const bool		 GetIsSelectedBattleAction	()const				{ return this->isSelectedBattleAction; }		 //部位破壊されているか
	const bool		 GetIsSelectedReaction		()const				{ return this->isSelectedReaction; }			 //部位破壊されているか
	const short		 GetNowSelectAction			()const				{ return this->selectAction; }					 //選択されているアクションを取得
	const float		 GetDotOfDirAndToTarget		()const				{ return this->innerProductOfDirectionToTarget; }//内積の取得
	void SetDamage			(const int _set)						{ this->damage = _set; }						//ダメージを設定
	void SetInterval		(const int _index,const int _set = 600)	{ this->intervalSet[_index] = _set; }			//インターバルの設定
	void SetToTargetDistance(const float _set)						{ this->toTargetDistance = _set; }				//目標までの距離を設定
	void RaiseLevel			()										{ this->level++; }								//レベル上昇
	void ResetLevel			()										{ this->level = 0; }							//レベルのリセット
	void SetIsDestroyedPart	(const bool _isDestroyedPart)			{ this->isDestroyedPart = _isDestroyedPart; }	//部位破壊されたかどうかを設定
	void SetSelectAction	(const short _type)						{ this->selectAction = _type; }					//選択されたアクションを設定
	void EntryCurrentBattleAction(BehaviorTreeNode& _action);	//バトルアクションの登録
	void EntryCurrentReaction	 (BehaviorTreeNode& _action);	//リアクションアクションの登録
	void ExitCurrentReaction	 ();							//リアクションアクションの解除
	void ExitCurrentBattleAction ();							//バトルアクションの解除
	BehaviorTreeNode& GetBattleAction() { return *this->currentBattleAction; }
	BehaviorTreeNode& GetReaction()  { return *this->currentReaction; }
private:
	/*内部処理関数*/
		  BeastBehaviorTree	  ();//コンストラクタ
		 ~BeastBehaviorTree	  ();//デストラクタ
	void UpdateMemberVariables();//メンバ変数の更新
	void CreateBehaviorTree();//ビヘイビアツリーを作成

	/*メンバ変数*/
	std::array<int, static_cast<int>(ActionType::COMBO_ATTACK) + 1>	intervalSet;							//インターバル
	BehaviorTreeNode*												Selector_DeathOrReactionOrBattleOrBreak;//ビヘイビアツリーのrootノード
	int																prevHp;									//前フレームのHP
	int																damage;									//今までに受けたダメージ量（怒り状態が終了すると０にリセットする）
	int																level;									//レベル
	int																selectAction;							//選択された種類
	bool															isDestroyedPart;						//部位破壊されたか
	bool															isSelectedBattleAction;					//バトルアクションが選択されているか
	bool															isSelectedReaction;						//リアクションが選択されているか
	float															toTargetDistance;						//目標までの距離
	float															innerProductOfDirectionToTarget;		//向きベクトルと目標ベクトルの内積
	BehaviorTreeNode*												currentBattleAction;					//現在のバトルアクション
	BehaviorTreeNode*												currentReaction;						//現在のリアクションアクション
	BehaviorTreeNode*												debugActionNode;						//デバック用アクションノード
};