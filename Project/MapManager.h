//========================================
// @brief �}�b�v�}�l�[�W���[�N���X
// �}�b�v�̊Ǘ����s��
//========================================
#pragma once
#include "Singleton.h"

class BossMap;
class Skydome;
class Dungeon;
class MutantDungeon;
class Gate;
class MapManager : public Singleton<MapManager>
{
public:
	friend class Singleton<MapManager>;

		  void Initialize();	 //������
		  void Finalize	 ();	 //�㏈��
		  void Update	 ();	 //�X�V
	const void Draw		 ()const;//�`��

	/*�񋓑�*/
	//�}�b�v�̎��
	enum class MapType
	{
		DUNGEON,
		BOSS,
	};

	/*getter*/
	const int	  GetStageModelHandle()const;
	const MapType GetMapType		 ()const { return this->mapType; }
		  void	  OnIsChangeStage	 () { this->isChangeStage = true; }
private:
	/*���������֐�*/
	 MapManager();//�R���X�g���N�^
	~MapManager();//�f�X�g���N�^

	/*�����o�ϐ�*/
	BossMap*		 bossMap;		//�{�X�̃}�b�v
	Skydome*		 skydome;		//�X�J�C�h�[��
	Gate*			 gate;			//�Q�[�g
	vector<Dungeon*> dungeon;		//�_���W����
	int				 bossType;		//�{�X�̎��
	MapType			 mapType;		//���ݕ\�����̃}�b�v
	bool			 isChangeStage;	//�_���W��������{�X�̃}�b�v�ɕύX����Ƃ��Ɏg�p����t���O
};

