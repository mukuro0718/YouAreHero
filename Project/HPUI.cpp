#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "LoadingAsset.h"
#include "HPUI.h"
#include "Character.h"
#include "Player.h"
#include "PlayerManager.h"
#include "EnemyManager.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
HPUI::HPUI()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	
	this->backgroundColor	 = ConvertColor(json.GetJson(JsonManager::FileType::UI)["BACKGROUND_COLOR"]		);
	this->playerHPColor		 = ConvertColor(json.GetJson(JsonManager::FileType::UI)["PLAYER_HP_COLOR"]		);
	this->bossHPColor		 = ConvertColor(json.GetJson(JsonManager::FileType::UI)["BOSS_HP_COLOR"]			);
	this->prevPlayerHPColor  = ConvertColor(json.GetJson(JsonManager::FileType::UI)["PLAYER_PREV_HP_COLOR"]	);
	this->prevBossHPColor	 = ConvertColor(json.GetJson(JsonManager::FileType::UI)["BOSS_PREV_HP_COLOR"]	);
	this->staminaColor		 = ConvertColor(json.GetJson(JsonManager::FileType::UI)["STAMINA_COLOR"]			);
	this->playerHPBar		 = asset.GetImage(LoadingAsset::ImageType::PLAYER_HP_BAR);
	this->barTable			 = asset.GetImage(LoadingAsset::ImageType::HP_TABLE);
	this->playerStaminaBar	 = asset.GetImage(LoadingAsset::ImageType::PLAYER_STAMINA_BAR);
	this->bossHPBar			 = asset.GetImage(LoadingAsset::ImageType::BOSS_HP_BAR);
	this->bossHPBarFrame	 = asset.GetImage(LoadingAsset::ImageType::BOSS_HP_BAR_FRAME);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
HPUI::~HPUI()
{

}

/// <summary>
/// ������
/// </summary>
void HPUI::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy  = Singleton<EnemyManager>::GetInstance();
	/*������*/
	int		playerHP	  = player.GetHP();
	int		bossHP		  = enemy.GetHP();
	int		playerStamina = static_cast<int>(player.GetStamina());

	/*�͈͂̐ݒ�*/
	this->playerHP		.SetRange(playerHP		, 0, playerHP);
	this->bossHP		.SetRange(bossHP		, 0, bossHP);
	this->playerStamina	.SetRange(playerStamina	, 0, playerStamina);
}

/// <summary>
/// �X�V
/// </summary>
void HPUI::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	/*HP�v�Z*/
	this->playerHP.SetNow(player.GetHP());
	this->bossHP.SetNow(enemy.GetHP());
	this->playerStamina.SetNow(static_cast<int>(player.GetStamina()));

	this->playerHP.PrevDecrease();
	this->bossHP.PrevDecrease();
	this->playerStamina.PrevDecrease();
}

/// <summary>
/// �`��
/// </summary>
const void HPUI::Draw()const
{
	auto& json = Singleton<JsonManager>::GetInstance();
	//HP
	{
		vector<int> table = json.GetJson(JsonManager::FileType::UI)["PLAYER_HP_TABLE_POSITION"];
		vector<int>	hp = json.GetJson(JsonManager::FileType::UI)["PLAYER_HP_POSITION"];
		int			height = json.GetJson(JsonManager::FileType::UI)["PLAYER_HP_HEIGHT"];
		int			indexBase = json.GetJson(JsonManager::FileType::UI)["PLAYER_HP_WIDTH"];
		int			nowHP = static_cast<int>(this->playerHP.GetNow() / this->playerHP.GetMax() * indexBase);
		int			prevHP = static_cast<int>(this->playerHP.GetPrev() / this->playerHP.GetMax() * indexBase);

		DrawExtendGraph(table[0], table[1], table[2], table[3], this->barTable, TRUE);
		DrawBox			(hp[0], hp[1], hp[0] + prevHP, hp[1] + height, this->prevPlayerHPColor, TRUE);
		DrawExtendGraph	(hp[0], hp[1], hp[0] + nowHP, hp[1] + height, this->playerHPBar, TRUE);
	}
	//STAMINA
	{
		vector<int> stamina		= json.GetJson(JsonManager::FileType::UI)["PLAYER_STAMINA_POSITION"];
		vector<int> table		= json.GetJson(JsonManager::FileType::UI)["PLAYER_STAMINA_TABLE_POSITION"];
		int			indexBase	= json.GetJson(JsonManager::FileType::UI)["PLAYER_STAMINA_WIDTH"];
		int			nowStamina	= static_cast<int>(this->playerStamina.GetNow() / this->playerStamina.GetMax() * indexBase);
		int			height		= json.GetJson(JsonManager::FileType::UI)["PLAYER_STAMINA_HEIGHT"];

		DrawExtendGraph(table[0], table[1], table[2], table[3], this->barTable, TRUE);
		DrawExtendGraph(stamina[0], stamina[1], stamina[0] + nowStamina, stamina[1] + height, this->playerStaminaBar, TRUE);
	}
	//BOSS
	{
		vector<int> table		= json.GetJson(JsonManager::FileType::UI)["BOSS_HP_TABLE_POSITION"];
		vector<int> hp			= json.GetJson(JsonManager::FileType::UI)["BOSS_HP_POSITION"];
		int			indexBase	= json.GetJson(JsonManager::FileType::UI)["BOSS_HP_WIDTH"];
		int			nowHP		= static_cast<int>(this->bossHP.GetNow() / this->bossHP.GetMax() * indexBase);
		int			prevHP		= static_cast<int>(this->bossHP.GetPrev() / this->bossHP.GetMax() * indexBase);
		int			height		= json.GetJson(JsonManager::FileType::UI)["BOSS_HP_HEIGHT"];

		DrawExtendGraph(table[0], table[1], table[2], table[3], this->barTable, TRUE);
		DrawBox(hp[0], hp[1], hp[0] + prevHP, hp[1] + height, this->prevBossHPColor, TRUE);
		DrawExtendGraph(hp[0], hp[1], hp[0] + nowHP, hp[1] + height, this->bossHPBar, TRUE);
	}
}
/// <summary>
/// �͈͂̐ݒ�
/// </summary>
void HPUI::RangeNum::SetRange(const int _max, const int _min, const int _now)
{
	this->max = static_cast<float>(_max);
	this->min = static_cast<float>(_min);
	this->now = static_cast<float>(_now);
	this->prev = this->now;
}

/// <summary>
/// �͈͂̐ݒ�
/// </summary>
void HPUI::RangeNum::Add(const int _add)
{
	this->now += _add;
	if (this->now > this->max)
	{
		this->now = this->max;
	}
	if (this->now < this->min)
	{
		this->now = this->min;
	}
}
/// <summary>
/// HP�̃Z�b�g
/// </summary>
void HPUI::RangeNum::SetNow(const int _hp)
{
	this->now = static_cast<float>(_hp);
	if (this->now < this->min)
	{
		this->now = this->min;
	}
	if (this->now > this->max)
	{
		this->now = this->max;
	}
}
/// <summary>
/// �ߋ���HP������
/// </summary>
void HPUI::RangeNum::PrevDecrease()
{
	if (this->now < this->prev)
	{
		this->prev -= 0.5f;
	}
}

/// <summary>
/// �F�擾
/// </summary>
/// <param name="_color"></param>
int HPUI::ConvertColor(const std::vector<int> _color)
{
	return GetColor(_color[0], _color[1], _color[2]);
}