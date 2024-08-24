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
/// ショットフラグ
/// </summary>
const bool PlayerManager::IsAttack()const
{
	return this->player->IsAttack();
}

const Collider PlayerManager::GetCollider()
{
	return this->player->GetCollider();
}
const int PlayerManager::GetHP()const
{
	return this->player->GetHP();
}
