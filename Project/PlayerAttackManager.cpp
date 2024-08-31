#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "GoriLib.h"
#include "GameObjectTag.h"
#include "PlayerMain_1Attack.h"
#include "PlayerMain_2Attack.h"
#include "PlayerSpecialAttack.h"
#include "PlayerAttackManager.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerAttackManager::PlayerAttackManager()
{
	this->main1 = new PlayerMain_1Attack();
	this->main2 = new PlayerMain_2Attack();
	this->special = new PlayerSpecialAttack();
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerAttackManager::~PlayerAttackManager()
{
	DeleteMemberInstance(this->main1);
	DeleteMemberInstance(this->main2);
	DeleteMemberInstance(this->special);
}

/// <summary>
/// 初期化
/// </summary>
void PlayerAttackManager::Initialize(GoriLib::Physics* _physics)
{
	this->main1->Initialize(_physics);
	this->main2->Initialize(_physics);
	this->special->Initialize(_physics);
}
/// <summary>
/// 後処理
/// </summary>
void PlayerAttackManager::Finalize(GoriLib::Physics* _physics)
{
	this->main1->Finalize(_physics);
	this->main2->Finalize(_physics);
	this->special->Finalize(_physics);
}
/// <summary>
/// 更新
/// </summary>
void PlayerAttackManager::Update(GoriLib::Physics* _physics)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	VECTOR position = player.GetPosition();
	VECTOR direction = player.GetDirection();

	this->main1->Update(_physics, position, direction);
	this->main2->Update(_physics, position, direction);
	this->special->Update(_physics, position, direction);
}
/// <summary>
/// 衝突したか
/// </summary>
void PlayerAttackManager::OnCollide(const GoriLib::Collidable& _colider)
{
	this->main1->OnCollide(_colider);
	this->main2->OnCollide(_colider);
	this->special->OnCollide(_colider);
}

/// <summary>
/// 描画
/// </summary>
const void PlayerAttackManager::Draw()const
{
	this->main1->Draw();
	this->main2->Draw();
	this->special->Draw();
}

void PlayerAttackManager::OnIsStart(const int _index)
{
	switch (_index)
	{
	case static_cast<int>(AttackType::MAIN_1):
		this->main1->OnIsStart();
		break;
	case static_cast<int>(AttackType::MAIN_2):
		this->main2->OnIsStart();
		break;
	case static_cast<int>(AttackType::SPECIAL):
		this->special->OnIsStart();
		break;
	}
}
