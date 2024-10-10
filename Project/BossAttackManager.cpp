#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "BossAttack.h"
#include "BossSlashAttack.h"
#include "BossStabAttack.h"
#include "BossRotatePunchAttack.h"
#include "BossSlapAttack.h"
#include "BossMeleeAttack.h"
#include "BossKickAttack.h"
#include "BossMeleeCombo3Attack.h"
#include "BossSlashCombo2Attack.h"
#include "BossAttackManager.h"
#include "EnemyManager.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossAttackManager::BossAttackManager()
{
	this->attack.emplace_back(new BossSlashAttack		(static_cast<int>(AttackType::SLASH)));
	this->attack.emplace_back(new BossStabAttack		(static_cast<int>(AttackType::STAB)));
	this->attack.emplace_back(new BossRotatePunchAttack	(static_cast<int>(AttackType::ROTATE_PUNCH)));
	this->attack.emplace_back(new BossSlapAttack		(static_cast<int>(AttackType::SLAP)));
	this->attack.emplace_back(new BossMeleeAttack		(static_cast<int>(AttackType::MELEE)));
	this->attack.emplace_back(new BossKickAttack		(static_cast<int>(AttackType::KICK)));
	this->attack.emplace_back(new BossMeleeCombo3Attack	(static_cast<int>(AttackType::MELEE_COMBO_3)));
	this->attack.emplace_back(new BossSlashCombo2Attack	(static_cast<int>(AttackType::SLASH_COMBO_2)));
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
void BossAttackManager::Initialize()
{
	for (int i = 0; i < this->attack.size(); i++)
	{
		this->attack[i]->Initialize();
	}
}
/// <summary>
/// 後処理
/// </summary>
void BossAttackManager::Finalize()
{
	for (int i = 0; i < this->attack.size(); i++)
	{
		this->attack[i]->Finalize();
	}
}
/// <summary>
/// 更新
/// </summary>
void BossAttackManager::Update()
{
	for (int i = 0; i < this->attack.size(); i++)
	{
		this->attack[i]->Update();
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
