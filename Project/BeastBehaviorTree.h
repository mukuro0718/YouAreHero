//====================================================
// @brief ルクスリオ用ビヘイビアツリー
//====================================================
#pragma once

class BehaviorTree;
class BehaviorTreeNode;
class Character;
class BeastBehaviorTree : public BehaviorTree
{
public:
	 BeastBehaviorTree();//コンストラクタ
	~BeastBehaviorTree();//デストラクタ

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
		  void Initialize	() override;	 //初期化
		  void Update		(Character& _chara) override;	 //更新
	const void Draw			()const override;//描画
private:
	/*内部処理関数*/
	void UpdateMemberVariables();//メンバ変数の更新
	void CreateBehaviorTree();//ビヘイビアツリーを作成
};