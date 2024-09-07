#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "GoriLib.h"
#include "GameObjectTag.h"
#include "PlayerAttack.h"
#include "PlayerAttackManager.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerAttackManager::PlayerAttackManager()
{
	this->attack = new PlayerAttack();
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerAttackManager::~PlayerAttackManager()
{
	DeleteMemberInstance(this->attack);
}

/// <summary>
/// 初期化
/// </summary>
void PlayerAttackManager::Initialize(GoriLib::Physics* _physics)
{
	this->attack->Initialize(_physics);
}
/// <summary>
/// 後処理
/// </summary>
void PlayerAttackManager::Finalize(GoriLib::Physics* _physics)
{
	this->attack->Finalize(_physics);
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

	/*攻撃しているかの判定*/
	if (!this->attack->GetIsStart() && player.GetIsSlash())
	{
		this->attack->OnIsStart();
	}

	this->attack->Update(_physics, position, direction);
}
/// <summary>
/// 衝突したか
/// </summary>
void PlayerAttackManager::OnCollide(const GoriLib::Collidable& _colider)
{
	this->attack->OnCollide(_colider);
}

/// <summary>
/// 描画
/// </summary>
const void PlayerAttackManager::Draw()const
{
	this->attack->Draw();
}

void PlayerAttackManager::OnIsStart(const int _index)
{
	this->attack->OnIsStart();
}
