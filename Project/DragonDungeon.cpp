#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "ModelColliderData.h"
#include "LoadingAsset.h"
#include "Dungeon.h"
#include "DragonDungeon.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
DragonDungeon::DragonDungeon()
	: Dungeon()
{
	/*�R���C�_�[�f�[�^�̍쐬*/
	this->collider = new ModelColliderData(ColliderData::Priority::STATIC, GameObjectTag::GROUND);

	/*���f���n���h���̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->collider->modelHandle	 = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::DRAGON_DUNGEON));
	this->modelHandle			 = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::DRAGON_DUNGEON));
	MV1SetupCollInfo (this->collider->modelHandle, this->collider->frameIndex, 16, 16, 16);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
DragonDungeon::~DragonDungeon()
{

}

/// <summary>
/// ������
/// </summary>
void DragonDungeon::Initialize()
{
	/*���W�b�h�{�f�B�̏�����*/
	auto& json = Singleton<JsonManager>::GetInstance();
	VECTOR position		= Gori::Convert(json.GetJson(JsonManager::FileType::MAP)["DRAGON_DUNGEON_POSITION"]);
	VECTOR scale		= Gori::Convert(json.GetJson(JsonManager::FileType::MAP)["DRAGON_DUNGEON_SCALE"]);
	VECTOR rotation		= Gori::Convert(json.GetJson(JsonManager::FileType::MAP)["DRAGON_DUNGEON_ROTATION"]);
		   rotation.y	= rotation.y * (DX_PI_F / 180.0f);
	this->collider->rigidbody.Initialize(false);
	this->collider->rigidbody.SetPosition(position);
	this->collider->rigidbody.SetRotation(rotation);
	this->collider->rigidbody.SetScale(scale);

	/*�����蔻��p���f���̐ݒ�*/
	this->collider->frameIndex = -1;
	this->collider->isDoHitCheck = true;
	MV1SetPosition	 (this->collider->modelHandle, this->collider->rigidbody.GetPosition());
	MV1SetRotationXYZ(this->collider->modelHandle, this->collider->rigidbody.GetRotation());
	MV1SetScale		 (this->collider->modelHandle, this->collider->rigidbody.GetScale());
	MV1RefreshCollInfo(this->collider->modelHandle, this->collider->frameIndex);
	//MV1SetupReferenceMesh(this->collider->modelHandle, this->collider->frameIndex, TRUE);
	//this->refPoly = MV1GetReferenceMesh(this->collider->modelHandle, this->collider->frameIndex, TRUE);

	MV1SetPosition	 (this->modelHandle, this->collider->rigidbody.GetPosition());
	MV1SetRotationXYZ(this->modelHandle, this->collider->rigidbody.GetRotation());
	MV1SetScale		 (this->modelHandle, this->collider->rigidbody.GetScale());
}

/// <summary>
/// �X�V
/// </summary>
void DragonDungeon::Update()
{
#ifdef _DEBUG
	if (this->collider->isDoHitCheck)
	{
		printfDx("DragonDungeon�͎g�p����Ă��܂�\n");
	}
#endif // _DEBUG
}
/// <summary>
/// �`��
/// </summary>
const void DragonDungeon::Draw()const
{
	if (this->collider->isDoHitCheck)
	{
		MV1DrawModel(this->modelHandle);
	}
//#ifdef _DEBUG
//	// �|���S���̐������J��Ԃ�
//	for (int i = 0; i < this->refPoly.PolygonNum; i++)
//	{
//		// �|���S�����`������O���_���g�p���ă��C���[�t���[����`�悷��
//		DrawLine3D(
//			this->refPoly.Vertexs[this->refPoly.Polygons[i].VIndex[0]].Position,
//			this->refPoly.Vertexs[this->refPoly.Polygons[i].VIndex[1]].Position,
//			GetColor(255, 255, 0));
//
//		DrawLine3D(
//			this->refPoly.Vertexs[this->refPoly.Polygons[i].VIndex[1]].Position,
//			this->refPoly.Vertexs[this->refPoly.Polygons[i].VIndex[2]].Position,
//			GetColor(255, 255, 0));
//
//		DrawLine3D(
//			this->refPoly.Vertexs[this->refPoly.Polygons[i].VIndex[2]].Position,
//			this->refPoly.Vertexs[this->refPoly.Polygons[i].VIndex[0]].Position,
//			GetColor(255, 255, 0));
//	}
//#endif // _DEBUG
}
