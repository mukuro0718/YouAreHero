#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h" 
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Brawler_PlayCurrentBattleAction.h"
#include "BrawlerEnemyBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Brawler_PlayCurrentBattleAction::Brawler_PlayCurrentBattleAction()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Brawler_PlayCurrentBattleAction::~Brawler_PlayCurrentBattleAction()
{

}

/// <summary>
/// �X�V����
/// </summary>
Brawler_PlayCurrentBattleAction::NodeState Brawler_PlayCurrentBattleAction::Update(BehaviorTree& _tree, Character& _chara)
{
	//�A�N�V�����̏�Ԃ��Z�b�g
	return _tree.GetBattleAction().Update(_tree, _chara);
}
