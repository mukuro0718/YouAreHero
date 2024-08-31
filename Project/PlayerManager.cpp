#include <DxLib.h>
#include "UseSTL.h"
#include "GoriLib.h"
#include "GameObjectTag.h"
#include "VECTORtoUseful.h"
#include "Player.h"
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
void PlayerManager::Initialize(GoriLib::Physics* _physics)
{
	this->player->Initialize(_physics);
}

/// <summary>
/// 後処理
/// </summary>
void PlayerManager::Finalize(GoriLib::Physics* _physics)
{
	this->player->Finalize(_physics);
}

/// <summary>
/// 更新
/// </summary>
void PlayerManager::Update(GoriLib::Physics* _physics)
{
	this->player->Update(_physics);
}
/// <summary>
/// 描画
/// </summary>
const void PlayerManager::Draw()const
{
	this->player->Draw();
}

/// <summary>
/// 当たったか
/// </summary>
void PlayerManager::OnCollide(const GoriLib::Collidable& _colider)
{
	this->player->OnCollide(_colider);
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

const int PlayerManager::GetHP()const
{
	return this->player->GetHP();
}
const int PlayerManager::GetHitNumber()const
{
	return this->player->GetHitNumber();
}
