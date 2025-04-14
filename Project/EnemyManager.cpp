#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ReactionType.h"
#include "DeleteInstance.h"
#include "CharacterData.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Enemy.h"
#include "Boss.h"
#include "Beast.h"
#include "Dragon.h"
#include "TankEnemy.h"
#include "MageEnemy.h"
#include "BrawlerEnemy.h"
#include "EnemyManager.h"
#include "EnemyChanger.h"
#include "MapManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
EnemyManager::EnemyManager()
	: frameTime				(0)
	, boss					(nullptr)
	, mapType				(-1)
	, enemyType				(0)
	, nearestWeakEnemyIndent(0)
	, useMageEnemyIndex(0)
	, useBrawlerEnemyIndex(0)
	, useTankEnemyIndex(0)
	, isEnemyWithinRange	(false)
{
	/*ボスの作成*/
	this->bossList.emplace_back(new Boss());
	this->bossList.emplace_back(new Dragon());
	this->bossList.emplace_back(new Beast());
	this->boss = this->bossList[0];

	/*雑魚敵のプールを作成*/
	for (int i = 0; i < this->WEAK_ENEMY_POOL; i++)
	{
		this->brawlerEnemyList.emplace_back(new BrawlerEnemy());
		this->mageEnemyList.emplace_back(new MageEnemy());
		this->tankEnemyList.emplace_back(new TankEnemy());
	}
}

/// <summary>
/// デストラクタ
/// </summary>
EnemyManager::~EnemyManager()
{
	this->boss->Finalize();
}

/// <summary>
/// 初期化
/// </summary>
void EnemyManager::Initialize()
{
	/*表示するボスの種類*/
	auto& changer = Singleton<EnemyChanger>::GetInstance();
	this->enemyType = changer.GetEnemyType();

	/*現在のボスの後処理をして、次のボスを初期化する*/
	this->boss->Finalize();
	this->boss = this->bossList[this->enemyType];
	//this->boss->Initialize();

	/*ボスに合わせて雑魚敵を初期化する*/
	CreateWeakEnemy();

	auto& map = Singleton<MapManager>::GetInstance();
	this->mapType = static_cast<int>(map.GetMapType());
}
/// <summary>
/// 更新
/// </summary>
void EnemyManager::Update()
{
	auto& map = Singleton<MapManager>::GetInstance();
	int currentMapType = static_cast<int>(map.GetMapType());
	if (currentMapType != this->mapType)
	{
		switch (currentMapType)
		{
		case static_cast<int>(MapManager::MapType::BOSS):
			this->boss->Initialize();
			break;
		case static_cast<int>(MapManager::MapType::DUNGEON):
			for (auto& enemy : this->weakEnemy)
			{
				enemy->Initialize();
			}
			break;
		}
		this->mapType = currentMapType;
	}

	//int startTime = GetNowCount();
	//マップの状態に応じて、敵を変化させる
	switch (this->mapType)
	{
	case static_cast<int>(MapManager::MapType::BOSS):
		this->boss->Update();
		break;
	case static_cast<int>(MapManager::MapType::DUNGEON):
		for (auto& enemy : this->weakEnemy)
		{
			enemy->Update();
		}
		break;
	}
	//int endTime = GetNowCount();
	//this->frameTime = endTime - startTime;
}
/// <summary>
/// アクション
/// </summary>
void EnemyManager::Finalize()
{
}

/// <summary>
/// 描画
/// </summary>
const void EnemyManager::Draw()const
{
	switch (this->mapType)
	{
	case static_cast<int>(MapManager::MapType::BOSS):
		this->boss->Draw();
		break;
	case static_cast<int>(MapManager::MapType::DUNGEON):
		for (auto& enemy : this->weakEnemy)
		{
			enemy->Draw();
		}
		break;
	}

	//printfDx("M_ENEMY_FRAMETIME:%d\n", this->frameTime);
}

/// <summary>
/// キャラクターデータの取得
/// </summary>
const CharacterData& EnemyManager::GetCharacterData()const
{
	switch (this->mapType)
	{
	case static_cast<int>(MapManager::MapType::BOSS):
		return this->boss->GetCharacterData();
	case static_cast<int>(MapManager::MapType::DUNGEON):
		return this->weakEnemy[this->nearestWeakEnemyIndent]->GetCharacterData();
	}
}

/// <summary>
/// リジッドボディの取得
/// </summary>
const Rigidbody& EnemyManager::GetRigidbody(const int _enemyIndent)const
{
	switch (this->mapType)
	{
	case static_cast<int>(MapManager::MapType::BOSS):
		return this->boss->GetRigidbody();
	case static_cast<int>(MapManager::MapType::DUNGEON):
		return this->weakEnemy[_enemyIndent]->GetRigidbody();
	}

}

/// <summary>
/// ダメージ処理
/// </summary>
const int EnemyManager::GetHP(const int _enemyIndent)const
{
	int indent = _enemyIndent;
	if (indent == -1)
	{
		indent = this->nearestWeakEnemyIndent;
	}

	switch (this->mapType)
	{
	case static_cast<int>(MapManager::MapType::BOSS):
		return this->boss->GetHP();
	case static_cast<int>(MapManager::MapType::DUNGEON):
		return this->weakEnemy[indent]->GetHP();
	}
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

/// <summary>
/// 生存フラグの取得
/// </summary>
const bool EnemyManager::GetIsAlive()const
{
	return this->boss->GetIsAlive();
}

/// <summary>
/// 攻撃回数の取得
/// </summary>
const int EnemyManager::GetAttackCount()const
{
	auto& enemy = dynamic_cast<Enemy&>(*this->boss);
	return enemy.GetAttackCount();
}

/// <summary>
/// ボスの状態の取得
/// </summary>
const int EnemyManager::GetBossState()const
{
	auto& enemy = dynamic_cast<Enemy&>(*this->boss);
	return enemy.GetBossState();
}

/// <summary>
/// ボスの状態の取得
/// </summary>
const VECTOR EnemyManager::GetPositionForLockon()const
{
	auto& enemy = dynamic_cast<Enemy&>(*this->boss);
	switch (this->mapType)
	{
	case static_cast<int>(MapManager::MapType::BOSS):
		return enemy.GetPositionForLockon();
	case static_cast<int>(MapManager::MapType::DUNGEON):
		return this->weakEnemy[nearestWeakEnemyIndent]->GetRigidbody().GetPosition();
	}
}

/// <summary>
/// 一番近い敵の番号を取得
/// </summary>
void EnemyManager::SetNearestEnemyIndent(const VECTOR _targetPosition)
{
	float minLength = 0.0f;
	int searchCount = 0;
	switch (this->mapType)
	{
	case static_cast<int>(MapManager::MapType::DUNGEON):
		//ターゲットと雑魚敵の距離を求め、一番近い敵の番号を返す
		this->isEnemyWithinRange = false;
		for (int i = 0; i < this->weakEnemy.size(); i++)
		{
			if (this->weakEnemy[i]->GetIsAlive())
			{
				float length = VSquareSize(VSub(this->weakEnemy[i]->GetRigidbody().GetPosition(), _targetPosition));
				if (searchCount == 0)
				{
					minLength = length;
					this->nearestWeakEnemyIndent = i;
					this->isEnemyWithinRange = true;
					searchCount++;
				}
				else if (minLength > length)
				{
					minLength = length;
					this->nearestWeakEnemyIndent = i;
					this->isEnemyWithinRange = true;
				}
			}
		}
		break;
	default:
		break;
	}

}

/// <summary>
/// 雑魚敵のリストを取得
/// </summary>
vector<int> EnemyManager::GetWeakEnemyTypeList(const int _bossType)
{
	/*ボスの種類に応じて登場させる敵の種類とその位置を変える*/
	vector<int> empty;
	auto& json = Singleton<JsonManager>::GetInstance();
	switch (_bossType)
	{
	case static_cast<int>(EnemyChanger::EnemyType::MUTANT):
		return json.GetJson(JsonManager::FileType::MAP)["MUTANT_DUNGEON_SPOWN_ENEMY_LIST"];
	case static_cast<int>(EnemyChanger::EnemyType::DRAGON):
		return json.GetJson(JsonManager::FileType::MAP)["DRAGON_DUNGEON_SPOWN_ENEMY_LIST"];
	case static_cast<int>(EnemyChanger::EnemyType::BEAST):
		return json.GetJson(JsonManager::FileType::MAP)["BEAST_DUNGEON_SPOWN_ENEMY_LIST"];
	default:
		return empty;
	}

}

/// <summary>
/// 雑魚敵を作成
/// </summary>
void EnemyManager::CreateWeakEnemy()
{
	/*メモリをきれいに*/
	this->weakEnemy.clear();
	this->useBrawlerEnemyIndex = 0;
	this->useMageEnemyIndex = 0;
	this->useTankEnemyIndex = 0;

	/*どのボスを登場させるのか*/
	auto& enemyChanger = Singleton<EnemyChanger>::GetInstance();
	int bossType = enemyChanger.GetEnemyType();

	/*雑魚敵をvectorに追加*/
	vector<int> typeList = GetWeakEnemyTypeList(bossType);
	for (int i = 0; i < typeList.size(); i++)
	{
		AddWeakEnemy(i, typeList[i], bossType, this->weakEnemy);
	}
}

/// <summary>
/// 雑魚敵をvectorに追加
/// </summary>
void EnemyManager::AddWeakEnemy(int& _indent, const int _weakEnemyType, const int _bossType, vector<Character*>& _list)
{
	switch (_weakEnemyType)
	{
	case static_cast<int>(WeakEnemyType::BRAWLER):
		this->brawlerEnemyList[this->useBrawlerEnemyIndex]->Initialize();
		this->brawlerEnemyList[this->useBrawlerEnemyIndex]->SetSpownPosition(_indent, _bossType);
		this->weakEnemy.emplace_back(this->brawlerEnemyList[this->useBrawlerEnemyIndex]);
		++this->useBrawlerEnemyIndex;
		break;
	case static_cast<int>(WeakEnemyType::MAGE):
		this->mageEnemyList[this->useMageEnemyIndex]->Initialize();
		this->mageEnemyList[this->useMageEnemyIndex]->SetSpownPosition(_indent, _bossType);
		this->weakEnemy.emplace_back(this->mageEnemyList[this->useMageEnemyIndex]);
		++this->useMageEnemyIndex;
		break;
	case static_cast<int>(WeakEnemyType::TANK):
		this->tankEnemyList[this->useTankEnemyIndex]->Initialize();
		this->tankEnemyList[this->useTankEnemyIndex]->SetSpownPosition(_indent, _bossType);
		this->weakEnemy.emplace_back(this->tankEnemyList[this->useTankEnemyIndex]);
		++this->useTankEnemyIndex;
		break;
	default:
		break;
	}

}

const bool EnemyManager::GetIsEnemyWithinRnage()const 
{
	if (this->isEnemyWithinRange && !this->weakEnemy[this->nearestWeakEnemyIndent]->GetIsAlive())
	{
		return false;
	}
	return this->isEnemyWithinRange; 
}

//Character& EnemyManager::GetCharacter(const int _enemyIndex = 0)
//{ 
//	switch (this->mapType)
//	{
//	case static_cast<int>(MapManager::MapType::BOSS):
//		return *this->boss;
//	default:
//		break;
//	//case static_cast<int>(MapManager::MapType::DUNGEON):
//		//return *this->weakEnemyList[_enemyIndex];
//	}
//
//}
