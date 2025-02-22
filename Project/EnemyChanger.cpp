#include <DxLib.h>
#include "UseSTL.h"
#include "EnemyChanger.h"
#include "InputManager.h"
#include "SoundManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
EnemyChanger::EnemyChanger()
	: enemyType			(0)
	, inputInterval		(0)
	, isProvDecide		(false)
	, isFinalDecide		(false)
{

}

/// <summary>
/// デストラクタ
/// </summary>
EnemyChanger::~EnemyChanger()
{

}

/// <summary>
/// 初期化
/// </summary>
void EnemyChanger::Initialize()
{
	this->enemyType		= 0;
	this->selectType	= 0;
	this->inputInterval = 0;
	this->isProvDecide	= false;
	this->isFinalDecide = false;
}

/// <summary>
/// 更新
/// </summary>
void EnemyChanger::Update()
{
	/*もし最終決定が行われていたら早期リターン*/
	if (this->isFinalDecide)return;

	/*エネミーの種類を選択*/
	auto& input = Singleton<InputManager>::GetInstance();
	auto& sound = Singleton<SoundManager>::GetInstance();
	if (this->inputInterval <= 0)
	{
		if (!this->isProvDecide)
		{
			if ((input.GetLStickState().YBuf < 0) || CheckHitKey(KEY_INPUT_UP))
			{
				this->enemyType--;
				int min = static_cast<int>(EnemyType::TUTORIAL);
				if (this->enemyType < min)
				{
					this->enemyType = min;
				}
				else
				{
					sound.OnIsPlayEffect(SoundManager::EffectType::CHANGE_SELECT_ICON);
				}
				this->inputInterval = this->MAX_INPUT_INTERBVAL;
			}
			else if ((input.GetLStickState().YBuf > 0) || CheckHitKey(KEY_INPUT_DOWN))
			{
				this->enemyType++;
				int max = static_cast<int>(EnemyType::BEAST);
				if (this->enemyType > max)
				{
					this->enemyType = max;
				}
				else
				{
					sound.OnIsPlayEffect(SoundManager::EffectType::CHANGE_SELECT_ICON);
				}
				this->inputInterval = this->MAX_INPUT_INTERBVAL;
			}
		}
	}
	else
	{
		this->inputInterval--;
	}

	/*決定*/
	int nowPad = input.GetNowPadState();
	int prevPad = input.GetPrevPadState();
	//パッドのBが押されたら
	if ((!(prevPad & InputManager::PAD_A) && (nowPad & InputManager::PAD_A)))
	{
		sound.OnIsPlayEffect(SoundManager::EffectType::CONFIRMATION_SOUND);
		if (!this->isProvDecide)
		{
			this->isProvDecide = true;
		}
		else
		{
			this->isFinalDecide = true;
		}
	}
	//パッドのAが押されたら
	else if ((!(prevPad & InputManager::PAD_B) && (nowPad & InputManager::PAD_B)))
	{
		sound.OnIsPlayEffect(SoundManager::EffectType::CANCEL_SOUND);
		this->isProvDecide = false;
	}
}