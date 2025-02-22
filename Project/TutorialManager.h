//=============================================
// @brief �`���[�g���A���Ǘ��N���X
// �`���[�g���A���̐i�s�x���`�F�b�N����
//=============================================
#pragma once
#include "Singleton.h"

class TutorialManager : public Singleton<TutorialManager>
{
public:
	friend class Singleton<TutorialManager>;
	
	/*�G�l�~�[�̎��*/
	enum class Stage
	{
		CAMERA	= 0,
		MOVE	= 1,
		ATTACK	= 2,
		AVOID	= 3,
		HEAL	= 4,
		BATTLE	= 5,
	};

	void Initialize	();//������
	void Update		();//�X�V

	/*getter*/
	const int  GetStage()const { return this->stage; }
private:
	 TutorialManager();
	~TutorialManager();

	/*�����o�ϐ�*/
	int  stage;				//���݂̃`���[�g���A���̐i�s�x
	bool isMoveCamera;		//�A�N�V�����t���O
	bool isLockon;
	bool isWalk;
	bool isRun;
	bool isWeakAttack;
	bool isRotateAttack;
	bool isAvoid;
	bool isBlock;
	bool isHeal;
};

