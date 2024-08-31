#include <DxLib.h>
#include "UseSTL.h"
#include "GoriLib.h"
#include "GameObjectTag.h"
#include "Boss.h"
#include "Vector4.h"
#include "Collider.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
EnemyManager::EnemyManager()
	:boss(nullptr)
{
	this->boss = new Boss();
}

/// <summary>
/// デストラクタ
/// </summary>
EnemyManager::~EnemyManager()
{

}

/// <summary>
/// 初期化
/// </summary>
void EnemyManager::Initialize(GoriLib::Physics* _physics)
{
	this->boss->Initialize(_physics);
}
/// <summary>
/// 更新
/// </summary>
void EnemyManager::Update(GoriLib::Physics* _physics)
{
	this->boss->Update(_physics);
}
/// <summary>
/// アクション
/// </summary>
void EnemyManager::Finalize(GoriLib::Physics* _physics)
{
	this->boss->Finalize(_physics);
}
/// <summary>
/// 当たったか
/// </summary>
void EnemyManager::OnCollide(const GoriLib::Collidable& _colider)
{
	this->boss->OnCollide(_colider);
}

/// <summary>
/// 描画
/// </summary>
const void EnemyManager::Draw()const
{
	this->boss->Draw();
}

/// <summary>
/// 座標の取得
/// </summary>
const VECTOR EnemyManager::GetPosition()const
{
	return this->boss->GetPosition();
}
const VECTOR EnemyManager::GetHeadPosition()const
{
	return this->boss->GetHeadPosition();
}

/// <summary>
/// ダメージ処理
/// </summary>
const int EnemyManager::GetHP()const
{
	return this->boss->GetHP();
}
const int EnemyManager::GetAttackNumber()const
{
	return this->boss->GetAttackNumber();
}
const int EnemyManager::GetHitNumber()const
{
	return this->boss->GetHitNumber();
}
void EnemyManager::SetHitNumber(const int _attackNumber)
{
	this->boss->SetHitNumber(_attackNumber);
}
const bool EnemyManager::IsAttack()const
{
	return this->boss->IsAttack();
}
void EnemyManager::OnIsHitAttack()
{
	this->boss->OnIsHitAttack();
}