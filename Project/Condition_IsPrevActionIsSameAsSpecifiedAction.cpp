#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BeastBehaviorTreeHeader.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Condition_IsPrevActionIsSameAsSpecifiedAction::Condition_IsPrevActionIsSameAsSpecifiedAction(const int _specifiedAction)
	: SPECIFIED_ACTION(_specifiedAction)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsPrevActionIsSameAsSpecifiedAction::~Condition_IsPrevActionIsSameAsSpecifiedAction()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsPrevActionIsSameAsSpecifiedAction::NodeState Condition_IsPrevActionIsSameAsSpecifiedAction::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();

	/*�C���^�[�o�����I�����Ă��Ȃ����FAILURE��Ԃ�*/
	if (rootNode.GetNowSelectAction() == this->SPECIFIED_ACTION)
	{
		//printfDx("CONDITION_NODESTATE_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_NODESTATE_FAILURE\n");
	return NodeState::FAILURE;
}