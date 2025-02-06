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
/// コンストラクタ
/// </summary>
DemonBehaviorTree::DemonBehaviorTree()
	: action_Dying(nullptr)
	, action_Idle (nullptr)
{
	this->action_Dying	= new Demon_Dying();
	this->action_Idle	= new Demon_Idle();
	
	/*初期化*/
	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
DemonBehaviorTree::~DemonBehaviorTree()
{
	DeleteMemberInstance(this->action_Dying);
	DeleteMemberInstance(this->action_Idle);
}

/// <summary>
/// 初期化
/// </summary>
void DemonBehaviorTree::Initialize()
{
	this->action_Dying->Initialize();
	this->action_Idle->Initialize();
}

/// <summary>
/// 更新
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
/// 描画
/// </summary>
const void DemonBehaviorTree::Draw()const
{
}
