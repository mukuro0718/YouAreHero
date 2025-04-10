#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "Character.h"
#include "BeastBehaviorTreeHeader.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Condition_IsActionIntervalIsOver::Condition_IsActionIntervalIsOver(const int _actionIndex)
	: ACTION_INDEX (_actionIndex)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Condition_IsActionIntervalIsOver::~Condition_IsActionIntervalIsOver()
{

}

/// <summary>
/// �X�V
/// </summary>
Condition_IsActionIntervalIsOver::NodeState Condition_IsActionIntervalIsOver::Update(BehaviorTree& _tree, Character& _chara)
{
	/*�C���^�[�o�����I�����Ă��Ȃ����FAILURE��Ԃ�*/
	if (_tree.GetInterval(this->ACTION_INDEX) > 0)
	{
		//printfDx("CONDITION_INTERVAL_FAILURE\n");
		return NodeState::FAILURE;
	}
	//printfDx("CONDITION_INTERVAL_SUCCESS\n");
	return NodeState::SUCCESS;
}