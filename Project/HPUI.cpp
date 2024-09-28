#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "LoadingAsset.h"
#include "HPUI.h"
#include "PlayerManager.h"
#include "EnemyManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
HPUI::HPUI()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	
	this->backgroundColor	= ConvertColor(json.GetJson(JsonManager::FileType::UI)["BACKGROUND_COLOR"]		);
	this->playerHPColor		= ConvertColor(json.GetJson(JsonManager::FileType::UI)["PLAYER_HP_COLOR"]		);
	this->bossHPColor		= ConvertColor(json.GetJson(JsonManager::FileType::UI)["BOSS_HP_COLOR"]			);
	this->prevPlayerHPColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["PLAYER_PREV_HP_COLOR"]	);
	this->prevBossHPColor	= ConvertColor(json.GetJson(JsonManager::FileType::UI)["BOSS_PREV_HP_COLOR"]	);
	this->staminaColor		= ConvertColor(json.GetJson(JsonManager::FileType::UI)["STAMINA_COLOR"]			);
	this->playerHPBar		 = asset.GetImage(LoadingAsset::ImageType::PLAYER_HP_BAR);
	this->playerHPTable		 = asset.GetImage(LoadingAsset::ImageType::PLAYER_HP_TABLE);
	this->playerStaminaBar	 = asset.GetImage(LoadingAsset::ImageType::PLAYER_STAMINA_BAR);
	this->playerStaminaTable = asset.GetImage(LoadingAsset::ImageType::PLAYER_STAMINA_TABLE);
	this->bossHPBar			 = asset.GetImage(LoadingAsset::ImageType::BOSS_HP_BAR);
	this->bossHPTable		 = asset.GetImage(LoadingAsset::ImageType::BOSS_HP_TABLE);
}

/// <summary>
/// デストラクタ
/// </summary>
HPUI::~HPUI()
{

}

/// <summary>
/// 初期化
/// </summary>
void HPUI::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy  = Singleton<EnemyManager>::GetInstance();
	/*初期化*/
	int		playerHP	  = player.GetHP();
	int		bossHP		  = enemy.GetHP();
	float	playerStamina = player.GetStamina();

	/*範囲の設定*/
	this->playerHP		.SetRange(playerHP		, 0, playerHP);
	this->bossHP		.SetRange(bossHP		, 0, bossHP);
	this->playerStamina	.SetRange(playerStamina	, 0, playerStamina);
}

/// <summary>
/// 更新
/// </summary>
void HPUI::Update()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	/*HP計算*/
	this->playerHP.SetNow(player.GetHP());
	this->bossHP.SetNow(enemy.GetHP());
	this->playerStamina.SetNow(player.GetStamina());

	this->playerHP.PrevDecrease();
	this->bossHP.PrevDecrease();
	this->playerStamina.PrevDecrease();
}

/// <summary>
/// 描画
/// </summary>
const void HPUI::Draw()const
{
	auto& json = Singleton<JsonManager>::GetInstance();
	//HP
	{
		Vec2d position;
		Box box;
		int height;
		int indexBase = json.GetJson(JsonManager::FileType::UI)["PLAYER_HP_WIDTH"];
		int nowHP = static_cast<int>(this->playerHP.GetNow() / this->playerHP.GetMax() * indexBase);
		int prevHP = static_cast<int>(this->playerHP.GetPrev() / this->playerHP.GetMax() * indexBase);
		position.Set(json.GetJson(JsonManager::FileType::UI)["PLAYER_HP_POSITION"]);
		height = json.GetJson(JsonManager::FileType::UI)["PLAYER_HP_HEIGHT"];
		Box table;
		table.Set(json.GetJson(JsonManager::FileType::UI)["HP_TABLE_BOX"]);
		box.lx = position.x;
		box.ly = position.y;
		box.rx = box.lx + indexBase;
		box.ry = box.ly + height;

		Vec2d tablePosiiton;
		tablePosiiton.Set(json.GetJson(JsonManager::FileType::UI)["PLAYER_HP_TABLE_POSITION"]);

		DrawGraph		(tablePosiiton.x , tablePosiiton.y , this->playerHPTable, TRUE);
		DrawBox			(box.lx, box.ly, box.lx + prevHP, box.ry, this->prevPlayerHPColor, TRUE);
		DrawExtendGraph	(box.lx, box.ly, box.lx + nowHP, box.ry, this->playerHPBar, TRUE);
	}
	//STAMINA
	{
		Vec2d table;
		Vec2d position;
		Box box;
		int height;
		int indexBase = json.GetJson(JsonManager::FileType::UI)["PLAYER_STAMINA_WIDTH"];
		int nowStamina =static_cast<int>(this->playerStamina.GetNow() / this->playerStamina.GetMax() * indexBase);
		position.Set(json.GetJson(JsonManager::FileType::UI)["PLAYER_STAMINA_POSITION"]);
		table.Set(json.GetJson(JsonManager::FileType::UI)["PLAYER_STAMINA_TABLE_POSITION"]);
		height = json.GetJson(JsonManager::FileType::UI)["PLAYER_STAMINA_HEIGHT"];

		box.lx = position.x;
		box.ly = position.y;
		box.rx = box.lx + indexBase;
		box.ry = box.ly + height;

		DrawGraph(table.x, table.y, this->playerStaminaTable, TRUE);
		DrawExtendGraph(box.lx, box.ly, box.lx + nowStamina, box.ry, this->playerStaminaBar, TRUE);
	}
	//BOSS
	{
		Vec2d table;
		Vec2d position;
		Box box;
		int height;
		int indexBase = json.GetJson(JsonManager::FileType::UI)["BOSS_HP_WIDTH"];
		int nowHP = static_cast<int>(this->bossHP.GetNow() / this->bossHP.GetMax() * indexBase);
		int prevHP = static_cast<int>(this->bossHP.GetPrev() / this->bossHP.GetMax() * indexBase);
		position.Set(json.GetJson(JsonManager::FileType::UI)["BOSS_HP_POSITION"]);
		table.Set(json.GetJson(JsonManager::FileType::UI)["BOSS_HP_TABLE_POSITION"]);
		height = json.GetJson(JsonManager::FileType::UI)["BOSS_HP_HEIGHT"];

		box.lx = position.x;
		box.ly = position.y;
		box.rx = box.lx + indexBase;
		box.ry = box.ly + height;

		DrawGraph(table.x, table.y, this->bossHPTable, TRUE);
		DrawBox(box.lx, box.ly, box.lx + prevHP, box.ry, this->prevBossHPColor, TRUE);
		DrawExtendGraph(box.lx, box.ly, box.lx + nowHP, box.ry, this->bossHPBar, TRUE);
	}
}
/// <summary>
/// 範囲の設定
/// </summary>
void HPUI::RangeNum::SetRange(const int _max, const int _min, const int _now)
{
	this->max = static_cast<float>(_max);
	this->min = static_cast<float>(_min);
	this->now = static_cast<float>(_now);
	this->prev = this->now;
}

/// <summary>
/// 範囲の設定
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
/// HPのセット
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
/// 過去のHPを減少
/// </summary>
void HPUI::RangeNum::PrevDecrease()
{
	if (this->now < this->prev)
	{
		this->prev -= 0.5f;
	}
}

/// <summary>
/// 色取得
/// </summary>
/// <param name="_color"></param>
int HPUI::ConvertColor(const std::vector<int> _color)
{
	return GetColor(_color[0], _color[1], _color[2]);
}