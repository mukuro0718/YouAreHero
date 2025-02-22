#include <DxLib.h>
#include "UseSTL.h"
#include "EnemyChanger.h"
#include "InputManager.h"
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
EnemyChanger::EnemyChanger()
	: enemyType			(0)
	, inputInterval		(0)
	, isProvDecide		(false)
	, isFinalDecide		(false)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
EnemyChanger::~EnemyChanger()
{

}

/// <summary>
/// ������
/// </summary>
void EnemyChanger::Initialize()
{
	this->enemyType		= 0;
	this->selectType	= 0;
	this->inputInterval = 0;
	this->isProvDecide	= false;
	this->isFinalDecide = false;
}

/// <summary>
/// �X�V
/// </summary>
void EnemyChanger::Update()
{
	/*�����ŏI���肪�s���Ă����瑁�����^�[��*/
	if (this->isFinalDecide)return;

	/*�G�l�~�[�̎�ނ�I��*/
	auto& input = Singleton<InputManager>::GetInstance();
	auto& sound = Singleton<SoundManager>::GetInstance();
	if (this->inputInterval <= 0)
	{
		if (!this->isProvDecide)
		{
			if ((input.GetLStickState().YBuf < 0) || CheckHitKey(KEY_INPUT_UP))
			{
				this->enemyType--;
				int min = static_cast<int>(EnemyType::TUTORIAL);
				if (this->enemyType < min)
				{
					this->enemyType = min;
				}
				else
				{
					sound.OnIsPlayEffect(SoundManager::EffectType::CHANGE_SELECT_ICON);
				}
				this->inputInterval = this->MAX_INPUT_INTERBVAL;
			}
			else if ((input.GetLStickState().YBuf > 0) || CheckHitKey(KEY_INPUT_DOWN))
			{
				this->enemyType++;
				int max = static_cast<int>(EnemyType::BEAST);
				if (this->enemyType > max)
				{
					this->enemyType = max;
				}
				else
				{
					sound.OnIsPlayEffect(SoundManager::EffectType::CHANGE_SELECT_ICON);
				}
				this->inputInterval = this->MAX_INPUT_INTERBVAL;
			}
		}
	}
	else
	{
		this->inputInterval--;
	}

	/*����*/
	int nowPad = input.GetNowPadState();
	int prevPad = input.GetPrevPadState();
	//�p�b�h��B�������ꂽ��
	if ((!(prevPad & InputManager::PAD_A) && (nowPad & InputManager::PAD_A)))
	{
		sound.OnIsPlayEffect(SoundManager::EffectType::CONFIRMATION_SOUND);
		if (!this->isProvDecide)
		{
			this->isProvDecide = true;
		}
		else
		{
			this->isFinalDecide = true;
		}
	}
	//�p�b�h��A�������ꂽ��
	else if ((!(prevPad & InputManager::PAD_B) && (nowPad & InputManager::PAD_B)))
	{
		sound.OnIsPlayEffect(SoundManager::EffectType::CANCEL_SOUND);
		this->isProvDecide = false;
	}
}