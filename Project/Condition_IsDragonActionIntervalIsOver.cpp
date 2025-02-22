#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DragonBehaviorTreeHeader.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Condition_IsDragonActionIntervalIsOver::Condition_IsDragonActionIntervalIsOver(const int _actionIndex)
	: ACTION_INDEX (_actionIndex)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsDragonActionIntervalIsOver::~Condition_IsDragonActionIntervalIsOver()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsDragonActionIntervalIsOver::NodeState Condition_IsDragonActionIntervalIsOver::Update()
{
	auto& rootNode = Singleton<DragonBehaviorTree>::GetInstance();

	/*�C���^�[�o�����O��葽�����SUCCESS��Ԃ�*/
	int interval = rootNode.GetInterval(this->ACTION_INDEX);
	if (interval > 0)
	{
		//printfDx("CONDITION_STAMINA_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_STAMINA_FAILURE\n");
	return NodeState::FAILURE;
}