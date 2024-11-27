#include <DxLib.h>
#include "UseSTL.h"
#include "ReactionType.h"
#include "CharacterData.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Boss.h"
#include "Beast.h"
#include "Dragon.h"
#include "EnemyManager.h"
#include "EnemyChanger.h"

/// <summary>
/// コンストラクタ
/// </summary>
EnemyManager::EnemyManager()
{
	this->boss.emplace_back(new Boss());
	this->boss.emplace_back(new Beast());
	this->boss.emplace_back(new Dragon());
	this->boss.emplace_back(new Boss());
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
	auto& changer = Singleton<EnemyChanger>::GetInstance();
	this->enemyType = changer.GetEnemyType();
	this->boss[this->enemyType]->Initialize();
}
/// <summary>
/// 更新
/// </summary>
void EnemyManager::Update()
{
	this->boss[this->enemyType]->Update();
}
/// <summary>
/// アクション
/// </summary>
void EnemyManager::Finalize()
{
	this->boss[this->enemyType]->Finalize();
}

/// <summary>
/// 描画
/// </summary>
const void EnemyManager::Draw()const
{
	this->boss[this->enemyType]->Draw();
}

/// <summary>
/// ダメージデータの取得
/// </summary>
const CharacterData& EnemyManager::GetCharacterData()const
{
	return this->boss[this->enemyType]->GetCharacterData();
}

/// <summary>
/// リジッドボディの取得
/// </summary>
const Rigidbody& EnemyManager::GetRigidbody()const
{
	return this->boss[this->enemyType]->GetRigidbody();
}

/// <summary>
/// ダメージ処理
/// </summary>
const int EnemyManager::GetHP()const
{
	return this->boss[this->enemyType]->GetHP();
}

/// <summary>
/// 攻撃フラグの取得
/// </summary>
const bool EnemyManager::IsAttack()const
{
	return this->boss[this->enemyType]->GetIsAttack();
}

/// <summary>
/// モデルハンドルの取得
/// </summary>
const int EnemyManager::GetModelHandle()const
{
	return this->boss[this->enemyType]->GetModelHandle();
}

const bool EnemyManager::GetIsAlive()const
{
	return this->boss[this->enemyType]->GetIsAlive();
}