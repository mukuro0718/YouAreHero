//================================================================
// @brief json�Ǘ��N���X
// JsonLoader�N���X���g�p����Json�t�@�C����ǂݍ��ݕێ�����
//================================================================
#pragma once
#include "Singleton.h"

class JsonManager : public Singleton<JsonManager>
{
public:
	friend class Singleton<JsonManager>;

	/*�t�@�C���̎��*/
	enum class FileType
	{
		FPS_CONTROLLER	= 0,
		SCREEN_SETUP	= 1,
		LOAD_SCENE		= 2,
		MODEL_PATH		= 3,
		IMAGE_PATH		= 4,
		FONT_PATH		= 5,
		SOUND_PATH		= 6,
		MAP			= 7,
		CAMERA			= 8,
		PLAYER			= 9,
		ENEMY			= 10,
		DEBUG			= 11,
	};

	void LoadFileAgain();

	/*getter*/
	const nlohmann::json GetJson(const FileType _index) { return json[static_cast<int>(_index)]; }//json�f�[�^��getter
protected:
	/*���������֐�*/
			 JsonManager();//�R���X�g���N�^
	virtual ~JsonManager();//�f�X�g���N�^
private:
	void	LoadJsonFile();//json�t�@�C���̓ǂݍ���

	/*�����o�ϐ�*/
	std::vector<nlohmann::json> json;
};
