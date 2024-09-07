#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "GoriLib.h"
#include "GameObjectTag.h"
#include "BossAttack.h"
#include "BossSlashAttack.h"
#include "BossHurricaneKick.h"
#include "BossJumpAttack.h"
#include "BossFlyAttack.h"
#include "BossRotatePunch.h"
#include "BossAttackManager.h"
#include "EnemyManager.h"
#include "PlayerManager.h"

using namespace GoriLib;
/// <summary>
/// コンストラクタ
/// </summary>
BossAttackManager::BossAttackManager()
{
	this->attack.emplace_back(new BossSlashAttack	(static_cast<int>(AttackType::SLASH)));
	this->attack.emplace_back(new BossFlyAttack		(static_cast<int>(AttackType::FLY_ATTACK)));
	this->attack.emplace_back(new BossHurricaneKick	(static_cast<int>(AttackType::HURRICANE_KICK)));
	this->attack.emplace_back(new BossJumpAttack	(static_cast<int>(AttackType::JUMP_ATTACK)));
	this->attack.emplace_back(new BossRotatePunch	(static_cast<int>(AttackType::ROTATE_PUNCH)));
}

/// <summary>
/// デストラクタ
/// </summary>
BossAttackManager::~BossAttackManager()
{
	for (int i = 0; i < this->attack.size(); i++)
	{
		DeleteMemberInstance(this->attack[i]);
	}
}

/// <summary>
/// 初期化
/// </summary>
void BossAttackManager::Initialize(GoriLib::Physics* _physics)
{
	for (int i = 0; i < this->attack.size(); i++)
	{
		this->attack[i]->Initialize(_physics);
	}
}
/// <summary>
/// 後処理
/// </summary>
void BossAttackManager::Finalize(GoriLib::Physics* _physics)
{
	for (int i = 0; i < this->attack.size(); i++)
	{
		this->attack[i]->Finalize(_physics);
	}
}
/// <summary>
/// 更新
/// </summary>
void BossAttackManager::Update(GoriLib::Physics* _physics)
{
	for (int i = 0; i < this->attack.size(); i++)
	{
		this->attack[i]->Update(_physics);
	}
}
/// <summary>
/// 衝突したか
/// </summary>
void BossAttackManager::OnCollide(const GoriLib::Collidable& _colider)
{
	for (int i = 0; i < this->attack.size(); i++)
	{
		this->attack[i]->OnCollide(_colider);
	}
}

/// <summary>
/// 描画
/// </summary>
const void BossAttackManager::Draw()const
{
	for (int i = 0; i < this->attack.size(); i++)
	{
		this->attack[i]->Draw();
	}
}

void BossAttackManager::OnIsStart(const int _index)
{
	this->attack[_index]->OnIsStart();
}
const VECTOR BossAttackManager::GetPosition(const int _index)
{
	return this->attack[_index]->GetPosition();
}
const VECTOR BossAttackManager::GetThrowPosition()
{
	return this->attack[static_cast<int>(AttackType::JUMP_ATTACK)]->GetPosition();
}