#include <DxLib.h>
#include "UseSTL.h"
#include "ReactionType.h"
#include "DeleteInstance.h"
#include "CharacterData.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Enemy.h"
#include "Boss.h"
#include "Beast.h"
#include "Dragon.h"
#include "Demon.h"
#include "EnemyManager.h"
#include "EnemyChanger.h"

/// <summary>
/// コンストラクタ
/// </summary>
EnemyManager::EnemyManager()
	: frameTime	(0)
	, boss		(nullptr)
{
	this->bossList.emplace_back(new Boss());
	this->bossList.emplace_back(new Dragon());
	this->bossList.emplace_back(new Beast());
	this->bossList.emplace_back(new Demon());
	this->boss = this->bossList[0];
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
	this->boss->Finalize();
	
	auto& changer = Singleton<EnemyChanger>::GetInstance();
	this->enemyType = changer.GetEnemyType();

	this->boss = this->bossList[this->enemyType];

	this->boss->Initialize();
}
/// <summary>
/// 更新
/// </summary>
void EnemyManager::Update()
{
	//int startTime = GetNowCount();
	this->boss->Update();
	//int endTime = GetNowCount();
	//this->frameTime = endTime - startTime;
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
	//printfDx("M_ENEMY_FRAMETIME:%d\n", this->frameTime);
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

const bool EnemyManager::GetIsAlive()const
{
	return this->boss->GetIsAlive();
}