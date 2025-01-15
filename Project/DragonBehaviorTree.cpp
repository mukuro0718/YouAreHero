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
/// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
/// </summary>
DragonBehaviorTree::DragonBehaviorTree()
	: Selector_DyingOrBattleOrIdle	 (nullptr)
	, debugActionNode				 (nullptr)
	, currentBattleAction			 (nullptr)
	, state							 (DragonState::NORMAL)
	, prevNodeState					 (BehaviorTreeNode::NodeState::NONE_ACTIVE)
	, toTargetDistance				 (0.0f)
	, innerProductOfDirectionToTarget(0.0f)
	, currentAction					 (0)
	, stamina						 (0)
	, isActive						 (true)
	, isSelectedBattleAction		 (false)
{
	auto& json = Singleton<JsonManager>::GetInstance();

	/*‘åŒ³‚ÌƒcƒŠ[‚Ìì¬*/
	this->Selector_DyingOrBattleOrIdle = new SelectorNode();
	
	/*ƒfƒXƒm[ƒh*/
	BehaviorTreeNode* Sequencer_DyingIfHpIsLessThanZero = new SequencerNode();
	Sequencer_DyingIfHpIsLessThanZero->AddChild(*new Condition_IsDragonHpIsLessThanZero());
	Sequencer_DyingIfHpIsLessThanZero->AddChild(*new Dragon_Dying());

	/*ƒoƒgƒ‹ƒAƒNƒVƒ‡ƒ“*/
	////ƒuƒŒƒX{’@‚«‚Â‚¯
	//BehaviorTreeNode* Sequencer_BreathSmashIfIntervalIsOver = new SequencerNode();
	//Sequencer_BreathSmashIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::BREATH_SMASH)));
	//Sequencer_BreathSmashIfIntervalIsOver->AddChild(*new Dragon_BreathSmash());
	////’@‚«‚Â‚¯{‰ñ“]UŒ‚
	//BehaviorTreeNode* Sequencer_SmashRotateIfIntervalIsOver = new SequencerNode();
	//Sequencer_SmashRotateIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::SMASH_ROTATE)));
	//Sequencer_SmashRotateIfIntervalIsOver->AddChild(*new Dragon_SmashRotate());
	////‚È‚¬•¥‚¢{’@‚«‚Â‚¯
	//BehaviorTreeNode* Sequencer_SweepSmashIfIntervalIsOver = new SequencerNode();
	//Sequencer_SweepSmashIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::SWEEP_SMASH)));
	//Sequencer_SweepSmashIfIntervalIsOver->AddChild(*new Dragon_SweepSmash());
	////‰ñ“]UŒ‚{‚È‚¬•¥‚¢
	//BehaviorTreeNode* Sequencer_RotateSweepIfIntervalIsOver = new SequencerNode();
	//Sequencer_RotateSweepIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::ROTATE_SWEEP)));
	//Sequencer_RotateSweepIfIntervalIsOver->AddChild(*new Dragon_RotateSweep());
	////‚È‚¬•¥‚¢{ƒuƒŒƒX
	//BehaviorTreeNode* Sequencer_SweepBreathIfIntervalIsOver = new SequencerNode();
	//Sequencer_SweepBreathIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::SWEEP_BREATH)));
	//Sequencer_SweepBreathIfIntervalIsOver->AddChild(*new Dragon_SweepBreath());
	////ƒuƒŒƒX{’@‚«‚Â‚¯{‰ñ“]UŒ‚
	//BehaviorTreeNode* Sequencer_BreathSmashRotateIfIntervalIsOver = new SequencerNode();
	//Sequencer_BreathSmashRotateIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::BREATH_SMASH_ROTATE)));
	//Sequencer_BreathSmashRotateIfIntervalIsOver->AddChild(*new Dragon_BreathSmashRotate());
	////‰ñ“]UŒ‚{‚È‚¬•¥‚¢{’@‚«‚Â‚¯
	//BehaviorTreeNode* Sequencer_RotateSweepSmashIfIntervalIsOver = new SequencerNode();
	//Sequencer_RotateSweepSmashIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::ROTATE_SWEEP_SMASH)));
	//Sequencer_RotateSweepSmashIfIntervalIsOver->AddChild(*new Dragon_RotateSweepSmash());
	////‚È‚¬•¥‚¢{ƒuƒŒƒX{’@‚«‚Â‚¯
	//BehaviorTreeNode* Sequencer_SweepBreathSmashIfIntervalIsOver = new SequencerNode();
	//Sequencer_SweepBreathSmashIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::SWEEP_BREATH_SMASH)));
	//Sequencer_SweepBreathSmashIfIntervalIsOver->AddChild(*new Dragon_SweepBreathSmash());
	////ƒuƒŒƒX{’@‚«‚Â‚¯{‚È‚¬•¥‚¢{‰ñ“]UŒ‚
	//BehaviorTreeNode* Sequencer_BreathSmashSweepRotateIfIntervalIsOver = new SequencerNode();
	//Sequencer_BreathSmashSweepRotateIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::BREATH_SMASH_SWEEP_ROTATE)));
	//Sequencer_BreathSmashSweepRotateIfIntervalIsOver->AddChild(*new Dragon_BreathSmashSweepRotate());
	////‰ñ“]UŒ‚{‚È‚¬•¥‚¢{ƒuƒŒƒX{’@‚«‚Â‚¯
	//BehaviorTreeNode* Sequencer_RotateSweepBreathSmashIfIntervalIsOver = new SequencerNode();
	//Sequencer_RotateSweepBreathSmashIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::ROTATE_SWEEP_BREATH_SMASH)));
	//Sequencer_RotateSweepBreathSmashIfIntervalIsOver->AddChild(*new Dragon_RotateSweepBreathSmash());
	////‚È‚¬•¥‚¢{’@‚«‚Â‚¯{‰ñ“]UŒ‚{ƒuƒŒƒX
	//BehaviorTreeNode* Sequencer_SweepSmashRotateBreathIfIntervalIsOver = new SequencerNode();
	//Sequencer_SweepBreathSmashIfIntervalIsOver->AddChild(*new Condition_IsDragonActionIntervalIsOver(static_cast<int>(ActionType::SWEEP_SMASH_ROTATE_BREATH)));
	//Sequencer_SweepBreathSmashIfIntervalIsOver->AddChild(*new Dragon_SweepSmashRotateBreath());
	//1’iŠK–Ú‚ÌUŒ‚ƒAƒNƒVƒ‡ƒ“
	BehaviorTreeNode* Selector_OneStepAttackAction = new RandomSelector();
	Selector_OneStepAttackAction->AddChild(*new Dragon_BreathSmash());	//ƒuƒŒƒX{’@‚«‚Â‚¯
	Selector_OneStepAttackAction->AddChild(*new Dragon_SmashRotate());	//’@‚«‚Â‚¯{‰ñ“]UŒ‚
	Selector_OneStepAttackAction->AddChild(*new Dragon_SweepSmash());	//‚È‚¬•¥‚¢{’@‚«‚Â‚¯
	Selector_OneStepAttackAction->AddChild(*new Dragon_RotateSweep());	//‰ñ“]UŒ‚{‚È‚¬•¥‚¢
	Selector_OneStepAttackAction->AddChild(*new Dragon_SweepBreath());	//‚È‚¬•¥‚¢{ƒuƒŒƒX
	//2’iŠK–Ú‚ÌUŒ‚ƒAƒNƒVƒ‡ƒ“
	BehaviorTreeNode* Selector_TwoStepAttackAction = new RandomSelector();
	Selector_TwoStepAttackAction->AddChild(*new Dragon_BreathSmashRotate());	//ƒuƒŒƒX{’@‚«‚Â‚¯{‰ñ“]UŒ‚
	Selector_TwoStepAttackAction->AddChild(*new Dragon_SmashRotate());	//’@‚«‚Â‚¯{‰ñ“]UŒ‚
	Selector_TwoStepAttackAction->AddChild(*new Dragon_SweepSmash());	//‚È‚¬•¥‚¢{’@‚«‚Â‚¯
	Selector_TwoStepAttackAction->AddChild(*new Dragon_RotateSweepSmash());	//‰ñ“]UŒ‚{‚È‚¬•¥‚¢{’@‚«‚Â‚¯
	Selector_TwoStepAttackAction->AddChild(*new Dragon_SweepBreathSmash());	//‚È‚¬•¥‚¢{ƒuƒŒƒX{’@‚«‚Â‚¯
	//3’iŠK–Ú‚ÌUŒ‚ƒAƒNƒVƒ‡ƒ“
	BehaviorTreeNode* Selector_ThreeStepAttackAction = new RandomSelector();
	Selector_ThreeStepAttackAction->AddChild(*new Dragon_BreathSmashSweepRotate());//ƒuƒŒƒX{’@‚«‚Â‚¯{‚È‚¬•¥‚¢{‰ñ“]UŒ‚
	Selector_ThreeStepAttackAction->AddChild(*new Dragon_SmashRotate());//’@‚«‚Â‚¯{‰ñ“]UŒ‚
	Selector_ThreeStepAttackAction->AddChild(*new Dragon_SweepSmashRotateBreath());//‚È‚¬•¥‚¢{’@‚«‚Â‚¯{‰ñ“]UŒ‚{ƒuƒŒƒX
	Selector_ThreeStepAttackAction->AddChild(*new Dragon_RotateSweepBreathSmash());//‰ñ“]UŒ‚{‚È‚¬•¥‚¢{ƒuƒŒƒX{’@‚«‚Â‚¯
	Selector_ThreeStepAttackAction->AddChild(*new Dragon_SweepBreathSmash());//‚È‚¬•¥‚¢{ƒuƒŒƒX{’@‚«‚Â‚¯
	//’iŠK‚²‚Æ‚ÌUŒ‚ƒAƒNƒVƒ‡ƒ“
	BehaviorTreeNode* Selector_StepByStepAttackAction = new SelectorNode();
	Selector_StepByStepAttackAction->AddChild(*Selector_OneStepAttackAction);
	Selector_StepByStepAttackAction->AddChild(*Selector_TwoStepAttackAction);
	Selector_StepByStepAttackAction->AddChild(*Selector_ThreeStepAttackAction);
	//ƒAƒNƒeƒBƒuƒtƒ‰ƒO‚ª—§‚Á‚Ä‚¢‚é‚Æ‚«‚ÌƒAƒNƒVƒ‡ƒ“
	BehaviorTreeNode* Sequencer_ActiveActionIfActiveFlagIsTrue = new SequencerNode();
	Sequencer_ActiveActionIfActiveFlagIsTrue->AddChild(*new Condition_IsDragonActiveFlagIsTrue());
	Sequencer_ActiveActionIfActiveFlagIsTrue->AddChild(*Selector_StepByStepAttackAction);
	//ƒAƒNƒVƒ‡ƒ“‚ª‘I‘ğ‚³‚ê‚Ä‚¢‚é‚©
	BehaviorTreeNode* Sequencer_PlayCurrentIfSelectedBattleAction = new SequencerNode();
	Sequencer_PlayCurrentIfSelectedBattleAction->AddChild(*new Condition_IsSelectedDragonBattleAction());
	Sequencer_PlayCurrentIfSelectedBattleAction->AddChild(*new Dragon_PlayCurrentBattleAction());
	//ƒAƒNƒeƒBƒu‚©‚Ç‚¤‚©‚ğ”»’f‚µƒAƒNƒVƒ‡ƒ“‚ğŒˆ‚ß‚éƒZƒŒƒNƒ^[ƒm[ƒh
	BehaviorTreeNode* Selector_ActiveOrNoneActiveAction = new SelectorNode();
	Selector_ActiveOrNoneActiveAction->AddChild(*Sequencer_PlayCurrentIfSelectedBattleAction);
	Selector_ActiveOrNoneActiveAction->AddChild(*Sequencer_ActiveActionIfActiveFlagIsTrue);
	Selector_ActiveOrNoneActiveAction->AddChild(*Selector_OneStepAttackAction);

	/*ƒ‹[ƒgƒm[ƒh‚ÉŠeƒAƒNƒVƒ‡ƒ“ƒcƒŠ[‚ğ‘}“ü*/
	this->Selector_DyingOrBattleOrIdle->AddChild(*Sequencer_DyingIfHpIsLessThanZero);
	this->Selector_DyingOrBattleOrIdle->AddChild(*Selector_ActiveOrNoneActiveAction);
	this->Selector_DyingOrBattleOrIdle->AddChild(*new Dragon_Idle());

	this->debugActionNode = new Dragon_RotateSweepBreathSmash();

	/*‰Šú‰»*/
	Initialize();
}

/// <summary>
/// ƒfƒXƒgƒ‰ƒNƒ^
/// </summary>
DragonBehaviorTree::~DragonBehaviorTree()
{

}

/// <summary>
/// ‰Šú‰»
/// </summary>
void DragonBehaviorTree::Initialize()
{
	//auto& json	= Singleton<JsonManager>::GetInstance();
	this->state		= DragonState::NORMAL;
}

/// <summary>
/// ƒƒ“ƒo•Ï”‚ÌXV
/// </summary>
void DragonBehaviorTree::UpdateMemberVariables()
{
	/*ƒCƒ“ƒ^[ƒoƒ‹‚ÌŒvZ*/
	for (auto interval : this->intervalSet)
	{
		if (interval != 0)
		{
			interval--;
		}
	}

	/*–Ú•W‚Ü‚Å‚Ì‹——£‚ğ‹‚ß‚é*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	VECTOR toTarget = VSub(player.GetRigidbody().GetPosition(), enemy.GetRigidbody().GetPosition());
	this->toTargetDistance = VSize(toTarget);

	/*ƒ{ƒX‚ÌŒü‚¢‚Ä‚¢‚é•ûŒü‚ÆƒvƒŒƒCƒ„[‚ÌˆÊ’u‚Æ‚Ì“àÏ‚ğæ‚é*/
	VECTOR enemyDirection = VTransform(VGet(0.0f, 0.0f, -1.0f), MGetRotY(enemy.GetRigidbody().GetRotation().y));
	this->innerProductOfDirectionToTarget = VDot(enemyDirection, VNorm(VSub(enemy.GetRigidbody().GetPosition(), player.GetRigidbody().GetPosition())));
}

/// <summary>
/// XV
/// </summary>
void DragonBehaviorTree::Update()
{
	/*ƒƒ“ƒo•Ï”‚ÌXV*/
	UpdateMemberVariables();

	printfDx("DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD:%f\n", this->innerProductOfDirectionToTarget);
	printfDx("BEAST_TO_TARGET:%f\n", this->toTargetDistance);
	printfDx("BEAST_ACTION_STATE:%d\n", this->currentAction);

	/*ƒcƒŠ[‚ÌÀs*/
	this->prevNodeState = this->Selector_DyingOrBattleOrIdle->Update();
	//this->prevNodeState = this->debugActionNode->Update();
}

/// <summary>
/// •`‰æ
/// </summary>
const void DragonBehaviorTree::Draw()const
{
#ifdef _DEBUG
	this->debugActionNode->Draw();
#endif // _DEBUG

}

/// <summary>
/// ƒoƒgƒ‹ƒAƒNƒVƒ‡ƒ“‚Ì“o˜^
/// </summary>
void DragonBehaviorTree::EntryCurrentBattleAction(BehaviorTreeNode& _action)
{
	this->currentBattleAction = &_action;
	this->isSelectedBattleAction = true;
}

/// <summary>
/// ƒoƒgƒ‹ƒAƒNƒVƒ‡ƒ“‚Ì‰ğœ
/// </summary>
void DragonBehaviorTree::ExitCurrentBattleAction()
{
	this->currentBattleAction = nullptr;
	this->isSelectedBattleAction = false;
}