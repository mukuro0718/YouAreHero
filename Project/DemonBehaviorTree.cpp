#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DemonBehaviorTreeHeader.h"
#include "DeleteInstance.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "EnemyManager.h"
#include "Character.h"
#include "Player.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
DemonBehaviorTree::DemonBehaviorTree()
	: action_Dying(nullptr)
	, action_Idle (nullptr)
{
	this->action_Dying	= new Demon_Dying();
	this->action_Idle	= new Demon_Idle();
	
	/*������*/
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
DemonBehaviorTree::~DemonBehaviorTree()
{
	DeleteMemberInstance(this->action_Dying);
	DeleteMemberInstance(this->action_Idle);
}

/// <summary>
/// ������
/// </summary>
void DemonBehaviorTree::Initialize()
{
	this->action_Dying->Initialize();
	this->action_Idle->Initialize();
}

/// <summary>
/// �X�V
/// </summary>
void DemonBehaviorTree::Update()
{
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	if (enemy.GetHP() < 0)
	{
		this->action_Dying->Update();
	}
	else
	{
		this->action_Idle->Update();
	}
}

/// <summary>
/// �`��
/// </summary>
const void DemonBehaviorTree::Draw()const
{
}
