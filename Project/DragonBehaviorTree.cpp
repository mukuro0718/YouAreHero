#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DragonBehaviorTreeHeader.h"
#include "Rigidbody.h"
#include "EnemyManager.h"
#include "Character.h"
#include "Enemy.h"
#include "Dragon.h"
#include "Player.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
DragonBehaviorTree::DragonBehaviorTree()
	: Selector_DyingOrBattleOrIdle	(nullptr)
	, debugActionNode				(nullptr)
	, state							(DragonState::NORMAL)
{
	auto& json = Singleton<JsonManager>::GetInstance();

	/*大元のツリーの作成*/
	this->Selector_DyingOrBattleOrIdle = new SelectorNode();
	
	/*デスノード*/
	BehaviorTreeNode* Sequencer_DyingIfHpIsLessThanZero = new SequencerNode();
	Sequencer_DyingIfHpIsLessThanZero->AddChild(*new Condition_IsDragonHpIsLessThanZero());
	Sequencer_DyingIfHpIsLessThanZero->AddChild();

	/*バトルノード*/

	/*待機ノード*/

	/*初期化*/
	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
DragonBehaviorTree::~DragonBehaviorTree()
{

}

/// <summary>
/// 初期化
/// </summary>
void DragonBehaviorTree::Initialize()
{
	//auto& json	= Singleton<JsonManager>::GetInstance();
	this->state		= DragonState::NORMAL;
}

/// <summary>
/// メンバ変数の更新
/// </summary>
void DragonBehaviorTree::UpdateMemberVariables()
{
	/*インターバルの計算*/
	for (auto interval : this->intervalSet)
	{
		if (interval != 0)
		{
			interval--;
		}
	}

	/*目標までの距離を求める*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	VECTOR toTarget = VSub(player.GetRigidbody().GetPosition(), enemy.GetRigidbody().GetPosition());
	this->toTargetDistance = VSize(toTarget);

	/*ボスの向いている方向とプレイヤーの位置との内積を取る*/
	VECTOR enemyDirection = VTransform(VGet(0.0f, 0.0f, -1.0f), MGetRotY(enemy.GetRigidbody().GetRotation().y));
	this->innerProductOfDirectionToTarget = VDot(enemyDirection, VNorm(VSub(enemy.GetRigidbody().GetPosition(), player.GetRigidbody().GetPosition())));
}

/// <summary>
/// 更新
/// </summary>
void DragonBehaviorTree::Update()
{
	/*メンバ変数の更新*/
	UpdateMemberVariables();

	printfDx("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD:%f\n", this->innerProductOfDirectionToTarget);
	printfDx("BEAST_TO_TARGET:%f\n", this->toTargetDistance);
	printfDx("BEAST_ACTION_STATE:%d\n", this->currentAction);

	/*ツリーの実行*/
	this->prevNodeState = this->Selector_DeathOrReactionOrBattleOrBreak->Update();
	//this->prevNodeState = this->debugActionNode->Update();
}

/// <summary>
/// 描画
/// </summary>
const void DragonBehaviorTree::Draw()const
{
#ifdef _DEBUG
	this->debugActionNode->Draw();
#endif // _DEBUG

}

