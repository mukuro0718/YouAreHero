//================================================
// @brief ���f���p�R���C�_�[�f�[�^
// ���f���Ƃ̓����蔻����s���Ƃ��Ɏg��
//================================================
#pragma once
#include "GameObjectTag.h"

class ColliderData;
class ModelColliderData : public ColliderData
{
public:
	ModelColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag);

	int modelHandle;
	int frameIndex;
	bool isDoHitCheck;//�����蔻����s����
};

