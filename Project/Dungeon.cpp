#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "ModelColliderData.h"
#include "LoadingAsset.h"
#include "Dungeon.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Dungeon::Dungeon()
	: collider(nullptr)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Dungeon::~Dungeon()
{
	/*�����o�^�̉���*/
	DeleteMemberInstance(this->collider);
}

/// <summary>
/// �`��
/// </summary>
const void Dungeon::Draw()const
{
	MV1DrawModel(this->modelHandle);
}

/// <summary>
/// ���f���n���h���̎擾
/// </summary>
const int Dungeon::GetModelHandle()const
{
	return this->modelHandle;
}

/// <summary>
/// �����蔻��t���O������
/// </summary>
void Dungeon::OffIsDoHitCheck()
{
	this->collider->isDoHitCheck = false;
}