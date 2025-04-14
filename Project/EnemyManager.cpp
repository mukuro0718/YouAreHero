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
/// �R���X�g���N�^
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
	/*�{�X�̍쐬*/
	this->bossList.emplace_back(new Boss());
	this->bossList.emplace_back(new Dragon());
	this->bossList.emplace_back(new Beast());
	this->boss = this->bossList[0];

	/*�G���G�̃v�[�����쐬*/
	for (int i = 0; i < this->WEAK_ENEMY_POOL; i++)
	{
		this->brawlerEnemyList.emplace_back(new BrawlerEnemy());
		this->mageEnemyList.emplace_back(new MageEnemy());
		this->tankEnemyList.emplace_back(new TankEnemy());
	}
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
EnemyManager::~EnemyManager()
{
	this->boss->Finalize();
}

/// <summary>
/// ������
/// </summary>
void EnemyManager::Initialize()
{
	/*�\������{�X�̎��*/
	auto& changer = Singleton<EnemyChanger>::GetInstance();
	this->enemyType = changer.GetEnemyType();

	/*���݂̃{�X�̌㏈�������āA���̃{�X������������*/
	this->boss->Finalize();
	this->boss = this->bossList[this->enemyType];
	//this->boss->Initialize();

	/*�{�X�ɍ��킹�ĎG���G������������*/
	CreateWeakEnemy();

	auto& map = Singleton<MapManager>::GetInstance();
	this->mapType = static_cast<int>(map.GetMapType());
}
/// <summary>
/// �X�V
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
	//�}�b�v�̏�Ԃɉ����āA�G��ω�������
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
/// �A�N�V����
/// </summary>
void EnemyManager::Finalize()
{
}

/// <summary>
/// �`��
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
/// �L�����N�^�[�f�[�^�̎擾
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
/// ���W�b�h�{�f�B�̎擾
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
/// �_���[�W����
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
/// �U���t���O�̎擾
/// </summary>
const bool EnemyManager::IsAttack()const
{
	return this->boss->GetIsAttack();
}

/// <summary>
/// ���f���n���h���̎擾
/// </summary>
const int EnemyManager::GetModelHandle()const
{
	return this->boss->GetModelHandle();
}

/// <summary>
/// �����t���O�̎擾
/// </summary>
const bool EnemyManager::GetIsAlive()const
{
	return this->boss->GetIsAlive();
}

/// <summary>
/// �U���񐔂̎擾
/// </summary>
const int EnemyManager::GetAttackCount()const
{
	auto& enemy = dynamic_cast<Enemy&>(*this->boss);
	return enemy.GetAttackCount();
}

/// <summary>
/// �{�X�̏�Ԃ̎擾
/// </summary>
const int EnemyManager::GetBossState()const
{
	auto& enemy = dynamic_cast<Enemy&>(*this->boss);
	return enemy.GetBossState();
}

/// <summary>
/// �{�X�̏�Ԃ̎擾
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
/// ��ԋ߂��G�̔ԍ����擾
/// </summary>
void EnemyManager::SetNearestEnemyIndent(const VECTOR _targetPosition)
{
	float minLength = 0.0f;
	int searchCount = 0;
	switch (this->mapType)
	{
	case static_cast<int>(MapManager::MapType::DUNGEON):
		//�^�[�Q�b�g�ƎG���G�̋��������߁A��ԋ߂��G�̔ԍ���Ԃ�
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
/// �G���G�̃��X�g���擾
/// </summary>
vector<int> EnemyManager::GetWeakEnemyTypeList(const int _bossType)
{
	/*�{�X�̎�ނɉ����ēo�ꂳ����G�̎�ނƂ��̈ʒu��ς���*/
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
/// �G���G���쐬
/// </summary>
void EnemyManager::CreateWeakEnemy()
{
	/*�����������ꂢ��*/
	this->weakEnemy.clear();
	this->useBrawlerEnemyIndex = 0;
	this->useMageEnemyIndex = 0;
	this->useTankEnemyIndex = 0;

	/*�ǂ̃{�X��o�ꂳ����̂�*/
	auto& enemyChanger = Singleton<EnemyChanger>::GetInstance();
	int bossType = enemyChanger.GetEnemyType();

	/*�G���G��vector�ɒǉ�*/
	vector<int> typeList = GetWeakEnemyTypeList(bossType);
	for (int i = 0; i < typeList.size(); i++)
	{
		AddWeakEnemy(i, typeList[i], bossType, this->weakEnemy);
	}
}

/// <summary>
/// �G���G��vector�ɒǉ�
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
