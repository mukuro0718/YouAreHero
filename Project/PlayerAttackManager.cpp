#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "Rigidbody.h"
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
void PlayerAttackManager::Initialize()
{
	this->attack->Initialize();
}
/// <summary>
/// 後処理
/// </summary>
void PlayerAttackManager::Finalize()
{
	this->attack->Finalize();
}
/// <summary>
/// 更新
/// </summary>
void PlayerAttackManager::Update()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	VECTOR position = player.GetRigidbody().GetPosition();
	VECTOR direction = player.GetRigidbody().GetDirection();

	this->attack->Update(position, direction);
}

/// <summary>
/// 描画
/// </summary>
const void PlayerAttackManager::Draw()const
{
	this->attack->Draw();
}

void PlayerAttackManager::OnIsStart()
{
	this->attack->OnIsStart();
}

/// <summary>
/// ダメージのセット
/// </summary>
void PlayerAttackManager::SetDamage(const float _damage)
{
	this->attack->SetDamage(_damage);
}
