#include <DxLib.h>
#include "USEJson.h"
#include "UseSTL.h"
#include "DeleteInstance.h"
#include "HitStop.h"
#include "HitStopManager.h"
#include "InputManager.h"
#include "BitFlag.h"
#include "FPSController.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
HitStopManager::HitStopManager()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
HitStopManager::~HitStopManager()
{
	for (auto item : this->items)
	{
		DeleteMemberInstance(item);
	}
	this->items.clear();
}

/// <summary>
/// �q�b�g�X�g�b�v�N���X�̓o�^
/// </summary>
void HitStopManager::Entry(HitStop& _item)
{
	/*�o�^����Ă��邩���ׂ�*/
	bool found = (std::find(this->items.begin(), this->items.end(), &_item) != this->items.end());

	/*�o�^����Ă��Ȃ�������*/
	if (!found)
	{
		this->items.emplace_back(&_item);
	}
	/*�o�^����Ă�����G���[��f��*/
	else
	{
		//assert(0 && "�w���item�͓o�^�ς݂ł�");
	}
}

/// <summary>
/// �Փ˕��̓o�^����
/// </summary>
void HitStopManager::Exit(HitStop& _item)
{
	/*�o�^����Ă��邩���ׂ�*/
	bool found = (std::find(this->items.begin(), this->items.end(), &_item) != this->items.end());

	/*�o�^����Ă�����*/
	if (!found)
	{
		this->items.remove(&_item);
	}
	/*�o�^����Ă��Ȃ�������G���[��f��*/
	else
	{
		//assert(0 && "�w���item�͓o�^����Ă��܂���");
	}

}

/// <summary>
/// �X�V
/// </summary>
void HitStopManager::Update()
{
	for (auto& item : this->items)
	{
		item->Update();
	}
}