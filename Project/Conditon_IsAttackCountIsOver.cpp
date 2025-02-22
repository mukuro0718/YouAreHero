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
Conditon_IsAttackCountIsOver::Conditon_IsAttackCountIsOver()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Conditon_IsAttackCountIsOver::~Conditon_IsAttackCountIsOver()
{

}

/// <summary>
/// �X�V
/// </summary>
Conditon_IsAttackCountIsOver::NodeState Conditon_IsAttackCountIsOver::Update()
{
	auto& boss = Singleton<EnemyManager>::GetInstance();

	/*�C���^�[�o�����I�����Ă��Ȃ����FAILURE��Ԃ�*/
	if (boss.GetAttackCount() <= 0)
	{
		//printfDx("CONDITION_INTERVAL_FAILURE\n");
		return NodeState::FAILURE;
	}
	//printfDx("CONDITION_INTERVAL_SUCCESS\n");
	return NodeState::SUCCESS;
}