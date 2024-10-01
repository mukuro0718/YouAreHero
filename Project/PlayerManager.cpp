#include <DxLib.h>
#include "UseSTL.h"
#include "ReactionType.h"
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
///	攻撃したか
/// </summary>
const bool PlayerManager::GetIsAttack()const
{
	return this->player->GetIsAttack();
}

/// <summary>
/// HPの取得
/// </summary>
const int PlayerManager::GetHP()const
{
	return this->player->GetHP();
}

/// <summary>
/// スタミナの取得
/// </summary>
const float PlayerManager::GetStamina()const
{
	auto& player = dynamic_cast<Player&>(*this->player);
	return player.GetStamina();
}

/// <summary>
/// 回復オーブの数の取得
/// </summary>
const int PlayerManager::GetHealOrbNum()const
{
	auto& player = dynamic_cast<Player&>(*this->player);
	return player.GetHealOrbNum();
}

/// <summary>
/// 生存フラグのgetter
/// </summary>
const bool PlayerManager::GetIsAlive()const
{
	return player->GetIsAlive();
}