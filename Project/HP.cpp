#include <DxLib.h>
#include "HP.h"

const int HP::BOSS_HP_COLOR   = GetColor(200, 50, 50);
const int HP::PLAYER_HP_COLOR = GetColor(50, 200, 50);
const int HP::PREV_HP_COLOR   = GetColor(200, 200, 50);

/// <summary>
/// コンストラクタ
/// </summary>
HP::HP()
{
	this->playerHP.SetRange(100, 0, 100);
	this->bossHP.SetRange(500, 0, 500);
	this->playerHPPosition.Set(50, 50);
	this->bossHPPosition.Set(460, 100);
}

/// <summary>
/// デストラクタ
/// </summary>
HP::~HP()
{

}

/// <summary>
/// 更新
/// </summary>
void HP::Update()
{
	/*HP計算*/
	this->playerHP.Add(0);
	this->bossHP.Add(0);
}

/// <summary>
/// 描画
/// </summary>
const void HP::Draw()const
{
	Box player;
	player.lx = this->playerHPPosition.x;
	player.ly = this->playerHPPosition.y;
	player.rx = this->playerHPPosition.x + this->playerHP.GetNow() / this->playerHP.GetMax() * this->PLAYER_INDEX_BASE;
	player.ry = this->playerHPPosition.y + this->HP_BAR_HEIGHT;
	DrawBox(player.lx, player.ly, player.rx, player.ry, this->PLAYER_HP_COLOR, TRUE);

	Box boss;
	boss.lx = this->bossHPPosition.x;
	boss.ly = this->bossHPPosition.y;
	boss.rx = this->bossHPPosition.x + this->bossHP.GetNow() / this->bossHP.GetMax() * this->BOSS_INDEX_BASE;
	boss.ry = this->bossHPPosition.y + this->HP_BAR_HEIGHT;
	DrawBox(boss.lx, boss.ly, boss.rx, boss.ry, this->BOSS_HP_COLOR, TRUE);
}

/// <summary>
/// 範囲の設定
/// </summary>
void HP::RangeNum::SetRange(const int _max, const int _min, const int _now)
{
	this->max = _max;
	this->min = _min;
	this->now = _now;
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