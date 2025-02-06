#include <DxLib.h>
#include "UseSTL.h"
#include "Sound.h"
#include "SoundEffect.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Enemy.h"
#include "EnemySoundEffect3D.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
EnemySoundEffect3D::EnemySoundEffect3D(const int _soundHandle, const int _volume)
	: SoundEffect(_soundHandle, _volume)
{
	//�����������鋗����ݒ肷��
	Set3DRadiusSoundMem(this->DISTANCE_SOUND_CAN_HEARD, this->soundHandle);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
EnemySoundEffect3D::~EnemySoundEffect3D()
{
	StopSoundMem(this->soundHandle);
}

/// <summary>
/// �X�V
/// </summary>
void EnemySoundEffect3D::Update()
{
	if (!this->isPlay)return;
	//���̍Đ��ʒu��ݒ�
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	Set3DPositionSoundMem(enemy.GetRigidbody().GetPosition(), this->soundHandle);
}

/// <summary>
/// �Đ��t���O�𗧂Ă�
/// </summary>
void EnemySoundEffect3D::OnIsPlay()
{
	//�t���O�𗧂Ă�
	this->isPlay = true;
	//�����~����
	StopSoundMem(this->soundHandle);
	//�Đ��ʒu������������
	SetCurrentPositionSoundMem(0, this->soundHandle);
	//���̍Đ��ʒu��ݒ�
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	Set3DPositionSoundMem(enemy.GetRigidbody().GetPosition(), this->soundHandle);
	//�Đ�
	PlaySoundMem(this->soundHandle, DX_PLAYTYPE_BACK);
}
