#include <DxLib.h>
#include "UseSTL.h"
#include "Character.h"
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
void PlayerManager::Initialize()
{
	this->player->Initialize();
}

/// <summary>
/// 後処理
/// </summary>
void PlayerManager::Finalize()
{
	this->player->Finalize();
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
/// ダメージデータの取得
/// </summary>
const CharacterData& PlayerManager::GetCharacterData()const
{
	return this->player->GetCharacterData();
}

/// <summary>
/// リジッドボディの取得
/// </summary>
const Rigidbody& PlayerManager::GetRigidbody()const
{
	return this->player->GetRigidbody();
}

/// <summary>
/// 移動フラグの取得
/// </summary>
const bool PlayerManager::IsMove()const
{
	auto player = dynamic_cast<Player&>(*this->player);
	return player.IsMove();
}
/// <summary>
/// ショットフラグ
/// </summary>
const bool PlayerManager::GetIsAttack()const
{
	return this->player->GetIsAttack();
}

const int PlayerManager::GetHP()const
{
	return this->player->GetHP();
}
const float PlayerManager::GetStamina()const
{
	auto& player = dynamic_cast<Player&>(*this->player);
	return player.GetStamina();
}
