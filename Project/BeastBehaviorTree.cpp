#include "UseSTL.h"
#include "UseJson.h"
#include "BeastBehaviorTreeHeader.h"

/// <summary>
/// コンストラクタ
/// </summary>
BeastBehaviorTree::BeastBehaviorTree()
	: Selector_DeathOrReactionOrBreakOrBattle(nullptr)
	, state(BeastState::NORMAL)
	, downValue(0)
	, prevHp(0)
	, damage(0)
	, level(0)
{
	/*インターバルを初期化*/
	this->intervalSet = { 0 };

	/*大元のツリーの作成*/
	this->Selector_DeathOrReactionOrBreakOrBattle = new SelectorNode();

	/*HPがないときのアクション*/
	{
		//デス判定ノード
		BehaviorTreeNode* Sequencer_DeathIfHpIsLessThanZero = new SequencerNode();
		Sequencer_DeathIfHpIsLessThanZero->AddChild(new CheckDyingCondition());
		Sequencer_DeathIfHpIsLessThanZero->AddChild(new Beast_Dying());
	}


	/*状態偏移用ツリーの作成*/
	BehaviorTreeNode* stateTransitionTree = new SelectorNode();
	{
		//状態偏移ノードの作成
		BehaviorTreeNode* stateTransitionNode = new SelectorNode();
		//怒りへの偏移
		BehaviorTreeNode* changeAngryNode = new SequencerNode();
		changeAngryNode->AddChild(new CheckAngryCondition());
		changeAngryNode->AddChild(new Beast_Roar());
		//状態偏移ノードへの追加
		stateTransitionNode->AddChild(changeDeathNode);
		stateTransitionNode->AddChild(changeAngryNode);

		//回復ノード

		//ツリーに追加
		stateTransitionTree->AddChild(stateTransitionNode);
	}

	/*戦闘用ツリーの作成*/
	BehaviorTreeNode* battleTree = new ExecuteAllNode();
	{
		//遠距離攻撃範囲外にプレイヤーがいる時のノード
		BehaviorTreeNode* moreFarTree = new SequencerNode();
		BehaviorTreeNode* moreFarNode = new SelectorNode();
		{
			//突進攻撃ノード
			BehaviorTreeNode* rushNode = new SequencerNode();
			rushNode->AddChild(new CheckRushCondition());
			rushNode->AddChild(new Beast_Rush());
			//歩きノード
			BehaviorTreeNode* walkNode = new SequencerNode();
			walkNode->AddChild(new CheckWalkCondition());
			rushNode->AddChild(new Beast_Walk());
			//ノードに追加
			moreFarNode->AddChild(rushNode);		 //突撃
			moreFarNode->AddChild(walkNode);		 //歩き
			moreFarNode->AddChild(new Beast_Run());//ダッシュ
		}
		moreFarTree->AddChild(new CheckMoreFarCondition());
		moreFarTree->AddChild(moreFarNode);

		//特殊攻撃可能時のノード
		BehaviorTreeNode* specialAttackNode = new SequencerNode();
		{
			//スーパーノヴァ
			BehaviorTreeNode* superNovaNode = new SequencerNode();
			superNovaNode->AddChild(new CheckSuperNovaCondition());
			superNovaNode->AddChild(new Beast_SuperNova());
			//特殊ノードに追加
			BehaviorTreeNode* specialNode = new SelectorNode();
			specialNode->AddChild(superNovaNode);
			specialNode->AddChild(new Beast_RaiseLevel());
			//特殊攻撃ノードに追加
			specialAttackNode->AddChild(new CheckSpecialNodeCondition());
			specialAttackNode->AddChild(specialNode);
		}

		//近接攻撃可能時のノード
		BehaviorTreeNode* nearActionNode = new SequencerNode();
		{
			//右足攻撃
			BehaviorTreeNode* rightFootAttackNode = new SequencerNode();
			rightFootAttackNode->AddChild(new CheckRightAttackCondition());
			rightFootAttackNode->AddChild(new Beast_RightFootAttack());
			//コンボアタック１
			BehaviorTreeNode* comboAttack1 = new SequencerNode();
			comboAttack1->AddChild(new CheckCombo1AttackCondition());
			comboAttack1->AddChild(new Beast_FootComboAttack());
			//右足溜め攻撃
			BehaviorTreeNode* chargeRightAttack = new SequencerNode();
			chargeRightAttack->AddChild(new CheckChargeRightAttackCondition());
			chargeRightAttack->AddChild(new Beast_ChargeRightFootAttack());
			//両足溜め攻撃
			BehaviorTreeNode* chargeBothAttack = new SequencerNode();
			chargeBothAttack->AddChild(new CheckChargeBothAttackCondition());
			chargeBothAttack->AddChild(new Beast_ChargeBothFootAttack());
			//コンボアタック２
			BehaviorTreeNode* comboAttack2 = new SequencerNode();
			comboAttack2->AddChild(new CheckCombo2AttackCondition());
			comboAttack2->AddChild(new Beast_RightFootAndRotAttack());
			//Normal時近接攻撃
			BehaviorTreeNode* nearNormalAttackNode = new SelectorNode();
			nearNormalAttackNode->AddChild(rightFootAttackNode);
			nearNormalAttackNode->AddChild(comboAttack1);
			//Angry時近接攻撃
			BehaviorTreeNode* nearAngryAttackNode = new SelectorNode();
			nearAngryAttackNode->AddChild(chargeRightAttack);
			nearAngryAttackNode->AddChild(chargeBothAttack);
			nearAngryAttackNode->AddChild(comboAttack2);
			//Normal時近接セット
			BehaviorTreeNode* nearNormalAttackNodeSet = new SequencerNode();
			//nearNormalAttackNodeSet->AddChild();
		}



		//バトルノードの作成
		BehaviorTreeNode* battleNode = new SelectorNode();
		battleNode->AddChild(new Beast_Idle());

		//ツリーに追加
		battleTree->AddChild(new Beast_CalcToTargetDistance());
		battleTree->AddChild(moreFarNode);
		battleTree->AddChild(battleNode);
	}

	/*rootに追加*/
	this->root->AddChild(stateTransitionTree);
	this->root->AddChild(battleTree);

	/*初期化*/
	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
BeastBehaviorTree::~BeastBehaviorTree()
{

}

/// <summary>
/// 初期化
/// </summary>
void BeastBehaviorTree::Initialize()
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->prevHp = json.GetJson(JsonManager::FileType::BEAST)["HP"];
	this->state = BeastState::NORMAL;
	this->downValue = 0;
	this->damage = 0;
	this->level = 0;
}

/// <summary>
/// 更新
/// </summary>
void BeastBehaviorTree::Update()
{
	/*インターバルの計算*/
	for (int& interval : this->intervalSet)
	{
		if (interval != 0)
		{
			interval--;
		}
	}

	/*目標までの距離を求める*/

	/*ツリーの実行*/
	this->root->Update();
}