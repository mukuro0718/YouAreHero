//#include <DxLib.h>
//#include <Effekseer.h>
//#include <EffekseerRendererDX11.h>
//#include "UseSTL.h"
//#include "UseJson.h"
//#include "Transform.h"
//#include "Rigidbody.h"
//#include "Effect.h"
//#include "BossEntryEffect.h"
//#include "EnemyManager.h"
//
///// <summary>
///// コンストラクタ
///// </summary>
//BossEntryEffect::BossEntryEffect(Effekseer::ManagerRef& _manager, const EFK_CHAR* _path)
//	: Effect(_manager, _path)
//{
//}
//
///// <summary>
///// 初期化
///// </summary>
//void BossEntryEffect::Initialize()
//{
//	/*シングルトンクラスのインスタンスの取得*/
//	auto& json = Singleton<JsonManager>::GetInstance();
//	auto& enemy = Singleton<EnemyManager>::GetInstance();
//
//	this->transform->SetScale(Convert(json.GetJson(JsonManager::FileType::EFFECT)["BOSS_ENTRY_SCALE"]));
//	this->transform->SetRotation(enemy.GetRigidbody().GetRotation());
//}
//
///// <summary>
///// 更新
///// </summary>
//void BossEntryEffect::Update(Effekseer::ManagerRef& _manager)
//{
//	/*シングルトンクラスのインスタンスの取得*/
//	auto& json = Singleton<JsonManager>::GetInstance();
//	auto& enemy = Singleton<EnemyManager>::GetInstance();
//
//	/*座標の設定*/
//	this->transform->SetPosition(enemy.GetRigidbody().GetPosition());
//
//	//_manager->SetLocation(this->handle,)
//}
//
