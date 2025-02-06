#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "Player.h"
#include "Sound.h"
#include "Bgm.h"
#include "SoundEffect.h"
#include "PlayerSoundEffect3D.h"
#include "EnemySoundEffect3D.h"
#include "SoundManager.h"
#include "LoadingAsset.h"
#include "PlayerManager.h"
#include "DeleteInstance.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
SoundManager::SoundManager()
{
	/*�T�E���h�N���X�̍쐬*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->bgms.emplace_back(new Bgm(asset.GetSound(LoadingAsset::SoundType::TITLE_BGM), json.GetJson(JsonManager::FileType::SOUND)["TITLE_BGM_VOLUME"]));
	this->bgms.emplace_back(new Bgm(asset.GetSound(LoadingAsset::SoundType::BATTLE_BGM), json.GetJson(JsonManager::FileType::SOUND)["BATTLE_BGM_VOLUME"]));

	this->effects.emplace_back(new SoundEffect(asset.GetSound(LoadingAsset::SoundType::CONFIRMATION)			 , json.GetJson(JsonManager::FileType::SOUND)["CONFIRMATION_SOUND_VOLUME"]));
	this->effects.emplace_back(new SoundEffect(asset.GetSound(LoadingAsset::SoundType::CANCEL)					 , json.GetJson(JsonManager::FileType::SOUND)["CANCEL_SOUND_VOLUME"]));
	this->effects.emplace_back(new SoundEffect(asset.GetSound(LoadingAsset::SoundType::CHANGE_SELECT_ICON)		 , json.GetJson(JsonManager::FileType::SOUND)["CHANGE_SELECT_ICON_VOLUME"]));

	this->effects.emplace_back(new PlayerSoundEffect3D(asset.GetSound(LoadingAsset::SoundType::SWORD_SLASH_1)			 , json.GetJson(JsonManager::FileType::SOUND)["SWORD_SLASH_1_VOLUME"]));
	this->effects.emplace_back(new PlayerSoundEffect3D(asset.GetSound(LoadingAsset::SoundType::SWORD_SLASH_1)			 , json.GetJson(JsonManager::FileType::SOUND)["SWORD_SLASH_1_VOLUME"]));
	this->effects.emplace_back(new PlayerSoundEffect3D(asset.GetSound(LoadingAsset::SoundType::SWORD_SLASH_2)			 , json.GetJson(JsonManager::FileType::SOUND)["SWORD_SLASH_2_VOLUME"]));
	this->effects.emplace_back(new PlayerSoundEffect3D(asset.GetSound(LoadingAsset::SoundType::SWORD_SLASH_3)			 , json.GetJson(JsonManager::FileType::SOUND)["SWORD_SLASH_3_VOLUME"]));
	this->effects.emplace_back(new PlayerSoundEffect3D(asset.GetSound(LoadingAsset::SoundType::SWORD_SWING_1)			 , json.GetJson(JsonManager::FileType::SOUND)["SWORD_SWING_1_VOLUME"]));
	this->effects.emplace_back(new PlayerSoundEffect3D(asset.GetSound(LoadingAsset::SoundType::SWORD_SWING_1)			 , json.GetJson(JsonManager::FileType::SOUND)["SWORD_SWING_1_VOLUME"]));
	this->effects.emplace_back(new PlayerSoundEffect3D(asset.GetSound(LoadingAsset::SoundType::SWORD_SWING_2)			 , json.GetJson(JsonManager::FileType::SOUND)["SWORD_SWING_2_VOLUME"]));
	this->effects.emplace_back(new PlayerSoundEffect3D(asset.GetSound(LoadingAsset::SoundType::SWORD_SWING_3)			 , json.GetJson(JsonManager::FileType::SOUND)["SWORD_SWING_3_VOLUME"]));
	this->effects.emplace_back(new PlayerSoundEffect3D(asset.GetSound(LoadingAsset::SoundType::PLAYER_WALK)			 , json.GetJson(JsonManager::FileType::SOUND)["PLAYER_WALK_VOLUME"]));
	this->effects.emplace_back(new PlayerSoundEffect3D(asset.GetSound(LoadingAsset::SoundType::PLAYER_RUN)				 , json.GetJson(JsonManager::FileType::SOUND)["PLAYER_RUN_VOLUME"]));
	this->effects.emplace_back(new PlayerSoundEffect3D(asset.GetSound(LoadingAsset::SoundType::SHIELD_BLOCK)			 , json.GetJson(JsonManager::FileType::SOUND)["PLAYER_BLOCK_VOLUME"]));
	this->effects.emplace_back(new PlayerSoundEffect3D(asset.GetSound(LoadingAsset::SoundType::HEAL_MAGIC)				 , json.GetJson(JsonManager::FileType::SOUND)["PLAYER_HEAL_VOLUME"]));
	this->effects.emplace_back(new PlayerSoundEffect3D(asset.GetSound(LoadingAsset::SoundType::PLAYER_KNOCKDOWN)		 , json.GetJson(JsonManager::FileType::SOUND)["PLAYER_KNOCKDOWN_VOLUME"]));
	this->effects.emplace_back(new PlayerSoundEffect3D(asset.GetSound(LoadingAsset::SoundType::PLAYER_ROLL)			 , json.GetJson(JsonManager::FileType::SOUND)["PLAYER_ROLL_VOLUME"]));

	this->effects.emplace_back(new EnemySoundEffect3D(asset.GetSound(LoadingAsset::SoundType::MONSTER_LIGHT_ATTACK)	 , json.GetJson(JsonManager::FileType::SOUND)["MONSTER_LIGHT_ATTACK_VOLUME"]));
	this->effects.emplace_back(new EnemySoundEffect3D(asset.GetSound(LoadingAsset::SoundType::MONSTER_HEAVY_ATTACK)	 , json.GetJson(JsonManager::FileType::SOUND)["MONSTER_HEAVY_ATTACK_VOLUME"]));
	this->effects.emplace_back(new EnemySoundEffect3D(asset.GetSound(LoadingAsset::SoundType::MONSTER_SWING_1)			 , json.GetJson(JsonManager::FileType::SOUND)["MONSTER_SWING_1_VOLUME"]));
	this->effects.emplace_back(new EnemySoundEffect3D(asset.GetSound(LoadingAsset::SoundType::MONSTER_SWING_2)			 , json.GetJson(JsonManager::FileType::SOUND)["MONSTER_SWING_2_VOLUME"]));
	this->effects.emplace_back(new EnemySoundEffect3D(asset.GetSound(LoadingAsset::SoundType::MONSTER_SWING_3)			 , json.GetJson(JsonManager::FileType::SOUND)["MONSTER_SWING_3_VOLUME"]));

	this->effects.emplace_back(new EnemySoundEffect3D(asset.GetSound(LoadingAsset::SoundType::MONSTER_ROAR)			 , json.GetJson(JsonManager::FileType::SOUND)["MONSTER_ROAR_VOLUME"]));
	this->effects.emplace_back(new EnemySoundEffect3D(asset.GetSound(LoadingAsset::SoundType::MONSTER_BREATH)			 , json.GetJson(JsonManager::FileType::SOUND)["MONSTER_BREATH_VOLUME"]));
	this->effects.emplace_back(new EnemySoundEffect3D(asset.GetSound(LoadingAsset::SoundType::MONSTER_FOOTSTEPS)		 , json.GetJson(JsonManager::FileType::SOUND)["MONSTER_FOOTSTEPS_VOLUME"]));

	this->effects.emplace_back(new EnemySoundEffect3D(asset.GetSound(LoadingAsset::SoundType::LUXURIO_EXPLOSION)		 , json.GetJson(JsonManager::FileType::SOUND)["LUXURIO_EXPLOSION_VOLUME"]));
	this->effects.emplace_back(new EnemySoundEffect3D(asset.GetSound(LoadingAsset::SoundType::LUXURIO_SUPER_NOVA)		 , json.GetJson(JsonManager::FileType::SOUND)["LUXURIO_SUPER_NOVA_VOLUME"]));
	this->effects.emplace_back(new EnemySoundEffect3D(asset.GetSound(LoadingAsset::SoundType::LUXURIO_ROAR)			 , json.GetJson(JsonManager::FileType::SOUND)["LUXURIO_ROAR_VOLUME"]));
	this->effects.emplace_back(new EnemySoundEffect3D(asset.GetSound(LoadingAsset::SoundType::LUXURIO_CHARGE)			 , json.GetJson(JsonManager::FileType::SOUND)["LUXURIO_CHARGE_VOLUME"]));
	this->effects.emplace_back(new EnemySoundEffect3D(asset.GetSound(LoadingAsset::SoundType::LUXURIO_WING	)			 , json.GetJson(JsonManager::FileType::SOUND)["LUXURIO_WING_VOLUME"]));
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
SoundManager::~SoundManager()
{
	/*�����o�C���X�^���X�̊J��*/
	for (auto& bgm : this->bgms)
	{
		DeleteMemberInstance(bgm);
	}
	for (auto& effect : this->effects)
	{
		DeleteMemberInstance(effect);
	}
	this->effects.clear();
	this->bgms.clear();
}

/// <summary>
/// �X�V
/// </summary>
void SoundManager::Initialize()
{
	/*BGM�̍X�V*/
	for (auto& bgm : this->bgms)
	{
		bgm->OffIsPlay();
	}

	/*���ʉ��i�T�E���h�G�t�F�N�g�j�̍X�V*/
	for (auto& effect : this->effects)
	{
		effect->OffIsPlay();
	}
}

/// <summary>
/// �X�V
/// </summary>
void SoundManager::Update()
{
	//int startTime = GetNowCount();
	
	/*BGM�̍X�V*/
	for (auto& bgm : this->bgms)
	{
		bgm->Update();
	}

	/*���ʉ��i�T�E���h�G�t�F�N�g�j�̍X�V*/
	for (auto& effect : this->effects)
	{
		effect->Update();
	}

	//int endTime = GetNowCount();
	//this->frameTime = endTime - startTime;
}

/// <summary>
/// �`��
/// </summary>
const void SoundManager::Draw()const
{
	//printfDx("EFFEK_FRAMETIME:%d\n", this->frameTime);
}

/// <summary>
/// �Đ��t���O�𗧂Ă�
/// </summary>
void SoundManager::OnIsPlayBgm(const BgmType _type)
{
	int type = static_cast<int>(_type);
	this->bgms[type]->OnIsPlay();
}
void SoundManager::OnIsPlayEffect(const EffectType _type)
{
	int type = static_cast<int>(_type);
	this->effects[type]->OnIsPlay();
}

/// <summary>
/// �Đ��t���O������
/// </summary>
void SoundManager::OffIsPlayBgm(const BgmType _type)
{
	this->bgms[static_cast<int>(_type)]->OffIsPlay();
}
void SoundManager::OffIsPlayEffect(const EffectType _type)
{
	this->effects[static_cast<int>(_type)]->OffIsPlay();
}

/// <summary>
/// �����Đ����Ă��邩���擾����
/// </summary>
const bool SoundManager::GetCheckBGMSoundState(const BgmType _type)
{
	return this->bgms[static_cast<int>(_type)]->GetCheckSoundState();
}
const bool SoundManager::GetCheckEffectSoundState(const EffectType _type)
{
	return this->effects[static_cast<int>(_type)]->GetCheckSoundState();
}
