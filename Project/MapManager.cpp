#include <DxLib.h>
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "BossMap.h"
#include "Gate.h"
#include "Skydome.h"
#include "Dungeon.h"
#include "MutantDungeon.h"
#include "BeastDungeon.h"
#include "DragonDungeon.h"
#include "MapManager.h"
#include "EnemyChanger.h"

/// <summary>
/// コンストラクタ
/// </summary>
MapManager::MapManager()
	: bossMap(nullptr)
	, skydome(nullptr)
{
	this->bossMap = new BossMap();
	this->skydome = new Skydome();
	this->gate	  = new Gate();
	this->dungeon.emplace_back(new MutantDungeon());
	this->dungeon.emplace_back(new DragonDungeon());
	this->dungeon.emplace_back(new BeastDungeon());
}

/// <summary>
/// デストラクタ
/// </summary>
MapManager::~MapManager()
{
	DeleteMemberInstance(this->bossMap);
	DeleteMemberInstance(this->skydome);
	DeleteMemberInstance(this->gate);
}

/// <summary>
/// 初期化
/// </summary>
void MapManager::Initialize()
{
	auto& enemyChanger = Singleton<EnemyChanger>::GetInstance();
	this->bossType = enemyChanger.GetEnemyType();
	this->mapType		= MapType::DUNGEON;
	this->isChangeStage = false;

	for (auto& dungeon : this->dungeon)
	{
		dungeon->OffIsDoHitCheck();
	}
	this->dungeon[this->bossType]->Initialize();
	this->bossMap->Initialize();
	this->skydome->Initialize();
	this->gate->Initialize(this->bossType);
}

/// <summary>
/// 後処理
/// </summary>
void MapManager::Finalize()
{
	this->bossMap->Finalize();
}

/// <summary>
/// 更新
/// </summary>
void MapManager::Update()
{
	if (this->isChangeStage && this->mapType == MapType::DUNGEON)
	{
		this->mapType = MapType::BOSS;
		this->dungeon[this->bossType]->OffIsDoHitCheck();
		this->bossMap->OnIsDoHitCheck();
	}


	switch (this->mapType)
	{
	case MapType::BOSS:
		this->bossMap->Update();
		break;
	case MapType::DUNGEON:
		this->dungeon[this->bossType]->Update();
		this->gate->Update();
		break;
	default:
		break;
	}
	this->skydome->Update();
}

/// <summary>
/// 描画
/// </summary>
const void MapManager::Draw()const
{
	switch (this->mapType)
	{
	case MapType::BOSS:
		this->bossMap->Draw();
		break;
	case MapType::DUNGEON:
		this->dungeon[this->bossType]->Draw();
		this->gate->Draw();
		break;
	default:
		break;
	}
	this->skydome->Draw();
}

/// <summary>
/// モデルハンドルのgetter
/// </summary>
const int MapManager::GetStageModelHandle()const
{
	switch (this->mapType)
	{
	case MapType::BOSS:
		return this->bossMap->GetModelHandle();
	case MapType::DUNGEON:
		return this->dungeon[this->bossType]->GetModelHandle();
	}
	return -1;
}