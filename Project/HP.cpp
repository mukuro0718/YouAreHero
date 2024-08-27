#include <DxLib.h>
#include "HP.h"
#include "PlayerManager.h"
#include "EnemyManager.h"

const int HP::BOSS_HP_COLOR   = GetColor(200, 50, 50);
const int HP::PLAYER_HP_COLOR = GetColor(50, 200, 50);
const int HP::PREV_HP_COLOR   = GetColor(200, 200, 50);

/// <summary>
/// コンストラクタ
/// </summary>
HP::HP()
{
}

/// <summary>
/// デストラクタ
/// </summary>
HP::~HP()
{

}

/// <summary>
/// 初期化
/// </summary>
void HP::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	int playerHP = player.GetHP();
	int bossHP = enemy.GetHP();
	this->playerHP.SetRange(playerHP, 0, playerHP);
	this->bossHP.SetRange(bossHP, 0, bossHP);
	this->playerHPPosition.Set(50, 50);
	this->bossHPPosition.Set(460, 100);
}

/// <summary>
/// 更新
/// </summary>
void HP::Update()
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
const void HP::Draw()const
{
	Box prevPlayer;
	prevPlayer.lx = this->playerHPPosition.x;
	prevPlayer.ly = this->playerHPPosition.y;
	prevPlayer.rx = this->playerHPPosition.x + static_cast<int>(this->playerHP.GetPrev() / this->playerHP.GetMax() * this->PLAYER_INDEX_BASE);
	prevPlayer.ry = this->playerHPPosition.y + this->HP_BAR_HEIGHT;
	DrawBox(prevPlayer.lx, prevPlayer.ly, prevPlayer.rx, prevPlayer.ry, this->PREV_HP_COLOR, TRUE);

	Box player;
	player.lx = this->playerHPPosition.x;
	player.ly = this->playerHPPosition.y;
	player.rx = this->playerHPPosition.x + static_cast<int>(this->playerHP.GetNow() / this->playerHP.GetMax() * this->PLAYER_INDEX_BASE);
	player.ry = this->playerHPPosition.y + this->HP_BAR_HEIGHT;
	DrawBox(player.lx, player.ly, player.rx, player.ry, this->PLAYER_HP_COLOR, TRUE);

	Box prevBoss;
	prevBoss.lx = this->bossHPPosition.x;
	prevBoss.ly = this->bossHPPosition.y;
	prevBoss.rx = this->bossHPPosition.x + static_cast<int>(this->bossHP.GetPrev() / this->bossHP.GetMax() * this->BOSS_INDEX_BASE);;
	prevBoss.ry = this->bossHPPosition.y + this->HP_BAR_HEIGHT;
	DrawBox(prevBoss.lx, prevBoss.ly, prevBoss.rx, prevBoss.ry, this->PREV_HP_COLOR, TRUE);

	Box boss;
	boss.lx = this->bossHPPosition.x;
	boss.ly = this->bossHPPosition.y;
	boss.rx = this->bossHPPosition.x + static_cast<int>(this->bossHP.GetNow() / this->bossHP.GetMax() * this->BOSS_INDEX_BASE);;
	boss.ry = this->bossHPPosition.y + this->HP_BAR_HEIGHT;
	DrawBox(boss.lx, boss.ly, boss.rx, boss.ry, this->BOSS_HP_COLOR, TRUE);
}

/// <summary>
/// 範囲の設定
/// </summary>
void HP::RangeNum::SetRange(const int _max, const int _min, const int _now)
{
	this->max = static_cast<float>(_max);
	this->min = static_cast<float>(_min);
	this->now = static_cast<float>(_now);
	this->prev = this->now;
}

/// <summary>
/// 範囲の設定
/// </summary>
void HP::RangeNum::Add(const int _add)
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
void HP::RangeNum::SetNow(const int _hp)
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
void HP::RangeNum::PrevDecrease()
{
	if (this->now < this->prev)
	{
		this->prev -= 1.0f;
	}
}