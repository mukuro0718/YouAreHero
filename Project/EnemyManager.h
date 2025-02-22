//=================================================
// @brief �{�X�}�l�[�W���[�N���X
//=================================================
#pragma once
#include "Singleton.h"

class Character;
class CharacterData;
class Rigidbody;
class EnemyManager : public Singleton<EnemyManager>
{
public:
	friend class Singleton<EnemyManager>;

	void Initialize(); //������
	void Finalize();
	void Update(); //�X�V
	const void Draw		 ()const;

	/*getter*/
	const int			 GetHP				 ()const;
	Character&			 GetCharacter		 () { return *this->boss; }
	const CharacterData& GetCharacterData	 ()const;
	const Rigidbody&	 GetRigidbody		 ()const;
	const bool			 IsAttack			 ()const;
	const int			 GetModelHandle		 ()const;
	const bool			 GetIsAlive			 ()const;
	const int			 GetAttackCount		 ()const;
	const int			 GetBossState		 ()const;
	const VECTOR		 GetPositionForLockon()const;
private:
	/*���������֐�*/
	 EnemyManager();//�R���X�g���N�^
	~EnemyManager();//�f�X�g���N�^

	/*�����o�ϐ�*/
	Character*		   boss;//�{�X
	vector<Character*> bossList;//�{�X
	
	int enemyType;
	int frameTime;
};

