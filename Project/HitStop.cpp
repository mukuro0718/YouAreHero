#include <DxLib.h>
#include "USEJson.h"
#include "UseSTL.h"
#include "HitStop.h"
#include "InputManager.h"
#include "BitFlag.h"

/// <summary>
/// コンストラクタ
/// </summary>
HitStop::HitStop()
	: time(0.0f)
{
}

/// <summary>
/// デストラクタ
/// </summary>
HitStop::~HitStop()
{

}

/// <summary>
/// 初期化
/// </summary>
void HitStop::Initialize()
{
	this->time = 0.0f;
}

/// <summary>
/// ヒットストップ中か
/// </summary>
bool HitStop::IsHitStop()
{
	/*時間が残っていたらtrueを返す*/
	if (this->time > 0)
	{
		this->time--;
		return true;
	}
	this->time = 0;
	return false;
}

/// <summary>
/// 描画
/// </summary>
const void HitStop::Draw()const
{
#ifdef _DEBUG
	if (this->time != 0)
	{
		printfDx("HIT_STOP\n");
	}
#endif // _DEBUG
}