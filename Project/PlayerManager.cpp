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
	: player	(nullptr)
	, frameTime	(0)
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
	//int startTime = GetNowCount();
	this->player->Update();
	//int endTime = GetNowCount();
	//this->frameTime = endTime - startTime;
}
/// <summary>
/// 描画
/// </summary>
const void PlayerManager::Draw()const
{
	this->player->Draw();
	//printfDx("PLAYER_FRAMETIME:%d\n", this->frameTime);
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
/// HPの取得
/// </summary>
const int PlayerManager::GetHP()const
{
	return this->player->GetHP();
}

/// <summary>
/// 現在の状態を取得
/// </summary>
const int PlayerManager::GetNowState()const
{
	return this->player->GetNowState();
}

/// <summary>
/// スタミナの取得
/// </summary>
const float PlayerManager::GetStamina()const
{
	return player->GetStamina();
}

/// <summary>
/// 回復オーブの数の取得
/// </summary>
const int PlayerManager::GetHealOrbNum()const
{
	return player->GetHealCount();
}

/// <summary>
/// 生存フラグのgetter
/// </summary>
const bool PlayerManager::GetIsAlive()const
{
	return this->player->GetIsAlive();
}

/// <summary>
/// ロックオンフラグの取得
/// </summary>
const bool PlayerManager::GetIsLockOn()const
{
	return this->player->GetIsLock();
}

/// <summary>
/// 抜刀フラグの取得
/// </summary>
const bool PlayerManager::GetIsDrawSword()const
{
	return player->GetIsDrawSword();
}

/// <summary>
/// モデルハンドルの取得
/// </summary>
const int PlayerManager::GetModelHandle() const
{
	return this->player->GetModelHandle();
}