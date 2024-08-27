#include <DxLib.h>
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "Player.h"
#include "Vector4.h"
#include "Collider.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerManager::PlayerManager()
	:player(nullptr)
{
	this->player = new Player();
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerManager::~PlayerManager()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerManager::Initialize()
{
	this->player->Initialize();
}

/// <summary>
/// アクション
/// </summary>
void PlayerManager::Action()
{
	this->player->Action();
}

/// <summary>
/// 更新
/// </summary>
void PlayerManager::Update()
{
	this->player->Update();
}
/// <summary>
/// 描画
/// </summary>
const void PlayerManager::Draw()const
{
	this->player->Draw();
}

/// <summary>
/// 移動ベクトルの補正
/// </summary>
void PlayerManager::FixMoveVector(const VECTOR _fixVector)
{
	this->player->FixMoveVector(_fixVector);
}

/// <summary>
/// 座標の取得
/// </summary>
const VECTOR PlayerManager::GetPosition()const
{
	return this->player->GetPosition();
}

/// <summary>
/// 向いている方向の取得
/// </summary>
const VECTOR PlayerManager::GetDirection()const
{
	return this->player->GetDirection();
}

/// <summary>
/// 移動フラグの取得
/// </summary>
const bool PlayerManager::IsMove()const
{
	return this->player->IsMove();
}
/// <summary>
/// ダメージの取得
/// </summary>
const int PlayerManager::GetDamage()const
{
	return this->player->GetDamage();
}
/// <summary>
/// ダメージ処理
/// </summary>
void PlayerManager::CalcDamage(const int _damage)
{
	return this->player->CalcDamage(_damage);
}

/// <summary>
/// ショットフラグ
/// </summary>
const bool PlayerManager::IsAttack()const
{
	return this->player->IsAttack();
}

const Collider PlayerManager::GetCharacterCollider()
{
	return this->player->GetCharacterCollider();
}
const Collider PlayerManager::GetAttackCollider()
{
	return this->player->GetAttackCollider();
}
const int PlayerManager::GetHP()const
{
	return this->player->GetHP();
}
const int PlayerManager::GetAttackNumber()const
{
	return this->player->GetAttackNumber();
}
const int PlayerManager::GetHitNumber()const
{
	return this->player->GetHitNumber();
}
void PlayerManager::SetHitNumber(const int _attackNumber)
{
	this->player->SetHitNumber(_attackNumber);
}