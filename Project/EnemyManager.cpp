#include <DxLib.h>
#include "UseSTL.h"
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
void EnemyManager::Initialize()
{
	this->boss->Initialize();
}
/// <summary>
/// 更新
/// </summary>
void EnemyManager::Update()
{
	this->boss->Update();
}
/// <summary>
/// アクション
/// </summary>
void EnemyManager::Action()
{
	this->boss->Action();
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


const Collider EnemyManager::GetCharacterCollider()
{
	return this->boss->GetCharacterCollider();
}
const Collider EnemyManager::GetAttackCollider()
{
	return this->boss->GetAttackCollider();
}
/// <summary>
/// ダメージの取得
/// </summary>
const int EnemyManager::GetDamage()const
{
	return this->boss->GetDamage();
}
/// <summary>
/// ダメージ処理
/// </summary>
void EnemyManager::CalcDamage(const int _damage)
{
	return this->boss->CalcDamage(_damage);
}
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