#include <DxLib.h>
#include "EnemyChanger.h"
#include "InputManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
EnemyChanger::EnemyChanger()
	: enemyType				(0)
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
	this->enemyType = static_cast<int>(EnemyType::GOLEM);
	this->inputInterval = 0;
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
	if (this->inputInterval <= 0)
	{
		if ((input.GetLStickState().YBuf < 0) || CheckHitKey(KEY_INPUT_UP))
		{
			this->enemyType--;
			int min = static_cast<int>(EnemyType::GOLEM);
			if (this->enemyType < min)
			{
				this->enemyType = min;
			}
			this->inputInterval = this->MAX_INPUT_INTERBVAL;
		}
		else if ((input.GetLStickState().YBuf > 0) || CheckHitKey(KEY_INPUT_DOWN))
		{
			this->enemyType++;
			int max = static_cast<int>(EnemyType::TUTORIAL);
			if (this->enemyType > max)
			{
				this->enemyType = max;
			}
			this->inputInterval = this->MAX_INPUT_INTERBVAL;
		}
	}
	else
	{
		this->inputInterval--;
	}

	/*決定*/
	int nowPad = input.GetNowPadState();
	int prevPad = input.GetPrevPadState();
	//パッドのBまたはキーのEが押されたら
	if ((!(prevPad & InputManager::PAD_B) && (nowPad & InputManager::PAD_B)) || CheckHitKey(KEY_INPUT_E))
	{
		if (!this->isProvDecide)
		{
			this->isProvDecide = true;
		}
		else
		{
			this->isFinalDecide = true;
		}
	}
	//パッドのAまたはキーのFが押されたら
	else if ((nowPad & InputManager::PAD_A) || CheckHitKey(KEY_INPUT_F))
	{
		this->isProvDecide = false;
	}
}