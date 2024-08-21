#include <DxLib.h>
#include "UseSTL.h"
#include "PlayerBoltMagic.h"
#include "PlayerEarthMagic.h"
#include "PlayerFlameMagic.h"
#include "PlayerIceMagic.h"
#include "PlayerStormMagic.h"
#include "PlayerMagicManager.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerMagicManager::PlayerMagicManager()
{
	this->magic[static_cast<int>(Element::FRAME)] = new PlayerFlameMagic();
	this->magic[static_cast<int>(Element::BOLT)]  = new PlayerBoltMagic ();
	this->magic[static_cast<int>(Element::ICE)]   = new PlayerIceMagic  ();
	this->magic[static_cast<int>(Element::EARTH)] = new PlayerEarthMagic();
	this->magic[static_cast<int>(Element::STORM)] = new PlayerStormMagic();
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerMagicManager::~PlayerMagicManager()
{

}

/// <summary>
/// 更新
/// </summary>
void PlayerMagicManager::Update()
{
	/*シングルトンクラスのインスタンスの取得*/
	//auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*プレイヤーの今の属性を取得*/
	int currentElement = player.GetCurrentElement();

	/*プレイヤーが攻撃していたら*/
	if (player.IsAttack())
	{
		this->magic[currentElement]->OnIsStart();
	}

	for (int i = 0; i < this->MAGIC_NUM; i++)
	{
		this->magic[i]->Update();
	}
}

/// <summary>
/// 描画
/// </summary>
const void PlayerMagicManager::Draw()const
{
	for (int i = 0; i < this->MAGIC_NUM; i++)
	{
		this->magic[i]->Draw();
	}
}