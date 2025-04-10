//==========================================
// @brief �_���[�W�p�J�v�Z��ColliderData
//==========================================
#pragma once
#include "GameObjectTag.h"

class ColliderData;
class CharacterData;
class AttackData;
class CharacterColliderData : public ColliderData
{
public:
	enum class CharaType
	{
		NONE = -1,
		PLAYER,
		GORG,
		MOL,
		LUX,
		DEMON,
		BRAWLER,
		TANK,
		MAGE
	};

	/*�R���X�g���N�^*/
	CharacterColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag, CharacterData* _data);

	void OnHit(const AttackData& _data, const VECTOR _attackPosition);//�q�b�g����

	/*getter/setter*/
	const CharacterData& GetCharacterData ()const;			//�L�����N�^�[�f�[�^�̎擾

	/*�����o�ϐ�*/
	CharacterData*	data;		//�L�����N�^�[�f�[�^
	float			radius;		//���a
	VECTOR			topPositon;	//�J�v�Z������W
	bool			isUseCollWithGround;//�n�ʂƂ̓����蔻����s����
	bool			isUseCollWithChara;//�L�����N�^�[�Ƃ̓����蔻����s����
	bool			isSetTopPosition;//�J�v�Z������W��ݒ肵�Ă��邩�ifalse�Ȃ�y��height�̂ݓ����Ă���j
	CharaType		type;
};
