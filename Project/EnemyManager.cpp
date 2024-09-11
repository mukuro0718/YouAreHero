#include <DxLib.h>
#include "UseSTL.h"
#include "CharacterData.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Boss.h"
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
void EnemyManager::Finalize()
{
	this->boss->Finalize();
}

/// <summary>
/// 描画
/// </summary>
const void EnemyManager::Draw()const
{
	this->boss->Draw();
}

/// <summary>
/// ダメージデータの取得
/// </summary>
const CharacterData& EnemyManager::GetCharacterData()const
{
	return this->boss->GetCharacterData();
}

/// <summary>
/// リジッドボディの取得
/// </summary>
const Rigidbody& EnemyManager::GetRigidbody()const
{
	return this->boss->GetRigidbody();
}

/// <summary>
/// ダメージ処理
/// </summary>
const int EnemyManager::GetHP()const
{
	return this->boss->GetHP();
}

/// <summary>
/// 攻撃フラグの取得
/// </summary>
const bool EnemyManager::IsAttack()const
{
	return this->boss->GetIsAttack();
}

/// <summary>
/// モデルハンドルの取得
/// </summary>
const int EnemyManager::GetModelHandle()const
{
	return this->boss->GetModelHandle();
}