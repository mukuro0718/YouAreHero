#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "HPUI.h"
#include "PlayerManager.h"
#include "EnemyManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
HPUI::HPUI()
{
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
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();
	int playerHP = player.GetHP();
	int bossHP = enemy.GetHP();
	/*初期化*/
	this->playerHPColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["PLAYER_HP_COLOR"]);
	this->bossHPColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["BOSS_HP_COLOR"]);
	this->prevPlayerHPColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["PLAYER_PREV_HP_COLOR"]);
	this->prevBossHPColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["BOSS_PREV_HP_COLOR"]);
	this->playerHP.SetRange(playerHP, 0, playerHP);
	this->bossHP.SetRange(bossHP, 0, bossHP);
	this->playerHPPosition.Set(json.GetJson(JsonManager::FileType::UI)["PLAYER_HP_POSITION"]);
	this->bossHPPosition.Set(json.GetJson(JsonManager::FileType::UI)["BOSS_PREV_HP_COLOR"]);
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
	this->playerHP.PrevDecrease();
	this->bossHP.PrevDecrease();
}

/// <summary>
/// 描画
/// </summary>
const void HPUI::Draw()const
{
	auto& json = Singleton<JsonManager>::GetInstance();

	Box prevPlayer;
	prevPlayer.lx = this->playerHPPosition.x;
	prevPlayer.ly = this->playerHPPosition.y;
	prevPlayer.rx = this->playerHPPosition.x + static_cast<int>(this->playerHP.GetPrev() / this->playerHP.GetMax() * json.GetJson(JsonManager::FileType::UI)["PLAYER_INDEX_BASE"]);
	prevPlayer.ry = this->playerHPPosition.y + json.GetJson(JsonManager::FileType::UI)["PLAYER_HP_HEIGHT"];
	DrawBox(prevPlayer.lx, prevPlayer.ly, prevPlayer.rx, prevPlayer.ry, this->prevPlayerHPColor, TRUE);

	Box player;
	player.lx = this->playerHPPosition.x;
	player.ly = this->playerHPPosition.y;
	player.rx = this->playerHPPosition.x + static_cast<int>(this->playerHP.GetNow() / this->playerHP.GetMax() * json.GetJson(JsonManager::FileType::UI)["PLAYER_INDEX_BASE"]);
	player.ry = this->playerHPPosition.y + json.GetJson(JsonManager::FileType::UI)["PLAYER_HP_HEIGHT"];
	DrawBox(player.lx, player.ly, player.rx, player.ry, this->playerHPColor, TRUE);

	Box prevBoss;
	prevBoss.lx = this->bossHPPosition.x;
	prevBoss.ly = this->bossHPPosition.y;
	prevBoss.rx = this->bossHPPosition.x + static_cast<int>(this->bossHP.GetPrev() / this->bossHP.GetMax() * json.GetJson(JsonManager::FileType::UI)["BOSS_HP_HEIGHT"]);
	prevBoss.ry = this->bossHPPosition.y + json.GetJson(JsonManager::FileType::UI)["BOSS_HP_HEIGHT"];
	DrawBox(prevBoss.lx, prevBoss.ly, prevBoss.rx, prevBoss.ry, this->prevBossHPColor, TRUE);

	Box boss;
	boss.lx = this->bossHPPosition.x;
	boss.ly = this->bossHPPosition.y;
	boss.rx = this->bossHPPosition.x + static_cast<int>(this->bossHP.GetNow() / this->bossHP.GetMax() * json.GetJson(JsonManager::FileType::UI)["BOSS_HP_HEIGHT"]);
	boss.ry = this->bossHPPosition.y + json.GetJson(JsonManager::FileType::UI)["BOSS_HP_HEIGHT"];
	DrawBox(boss.lx, boss.ly, boss.rx, boss.ry, this->bossHPColor, TRUE);
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
		this->prev -= 1.0f;
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