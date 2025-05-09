#include <DxLib.h>
#include "USEJson.h"
#include "UseSTL.h"
#include "DeleteInstance.h"
#include "HitStop.h"
#include "HitStopManager.h"
#include "InputManager.h"
#include "BitFlag.h"
#include "FPSController.h"

/// <summary>
/// コンストラクタ
/// </summary>
HitStopManager::HitStopManager()
{
}

/// <summary>
/// デストラクタ
/// </summary>
HitStopManager::~HitStopManager()
{
	for (auto item : this->items)
	{
		DeleteMemberInstance(item);
	}
	this->items.clear();
}

/// <summary>
/// ヒットストップクラスの登録
/// </summary>
void HitStopManager::Entry(HitStop& _item)
{
	/*登録されているか調べる*/
	bool found = (std::find(this->items.begin(), this->items.end(), &_item) != this->items.end());

	/*登録されていなかったら*/
	if (!found)
	{
		this->items.emplace_back(&_item);
	}
	/*登録されていたらエラーを吐く*/
	else
	{
		//assert(0 && "指定のitemは登録済みです");
	}
}

/// <summary>
/// 衝突物の登録解除
/// </summary>
void HitStopManager::Exit(HitStop& _item)
{
	/*登録されているか調べる*/
	bool found = (std::find(this->items.begin(), this->items.end(), &_item) != this->items.end());

	/*登録されていたら*/
	if (!found)
	{
		this->items.remove(&_item);
	}
	/*登録されていなかったらエラーを吐く*/
	else
	{
		//assert(0 && "指定のitemは登録されていません");
	}

}

/// <summary>
/// 更新
/// </summary>
void HitStopManager::Update()
{
	for (auto& item : this->items)
	{
		item->Update();
	}
}