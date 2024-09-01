#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "GoriLib.h"
#include "GameObjectTag.h"
#include "BossAttack.h"
#include "BossAttackManager.h"
#include "EnemyManager.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossAttackManager::BossAttackManager()
{
	for (int i = 0; i < this->ATTACK_NUM; i++)
	{
		this->attack.emplace_back(new BossAttack(i));
	}
}

/// <summary>
/// デストラクタ
/// </summary>
BossAttackManager::~BossAttackManager()
{
	for (int i = 0; i < this->ATTACK_NUM; i++)
	{
		DeleteMemberInstance(this->attack[i]);
	}
}

/// <summary>
/// 初期化
/// </summary>
void BossAttackManager::Initialize(GoriLib::Physics* _physics)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	for (int i = 0; i < this->ATTACK_NUM; i++)
	{
		this->attack[i]->Initialize(_physics,json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_RADIUS"][i]);
	}
}
/// <summary>
/// 後処理
/// </summary>
void BossAttackManager::Finalize(GoriLib::Physics* _physics)
{
	for (int i = 0; i < this->ATTACK_NUM; i++)
	{
		this->attack[i]->Finalize(_physics);
	}
}
/// <summary>
/// 更新
/// </summary>
void BossAttackManager::Update(GoriLib::Physics* _physics)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	VECTOR direction = VSub(player.GetPosition(), enemy.GetPosition());
	direction = VNorm(direction);
	int modelHandle = enemy.GetModelHandle();
	this->attack[static_cast<int>(AttackType::SLASH)]->Update(_physics, MV1GetFramePosition(modelHandle,11), direction, false, 0.8f);
	this->attack[static_cast<int>(AttackType::ROTATE_PUNCH)]->Update(_physics, MV1GetFramePosition(modelHandle, 15), direction, false, 0.8f);
	this->attack[static_cast<int>(AttackType::JUMP_ATTACK)]->Update(_physics, MV1GetFramePosition(modelHandle, 1), direction, false, 0.0f);
}
/// <summary>
/// 衝突したか
/// </summary>
void BossAttackManager::OnCollide(const GoriLib::Collidable& _colider)
{
	for (int i = 0; i < this->ATTACK_NUM; i++)
	{
		this->attack[i]->OnCollide(_colider);
	}
}

/// <summary>
/// 描画
/// </summary>
const void BossAttackManager::Draw()const
{
	for (int i = 0; i < this->ATTACK_NUM; i++)
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