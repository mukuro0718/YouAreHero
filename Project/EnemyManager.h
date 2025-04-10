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

		  void Initialize();	 //������
		  void Finalize	 ();	 //�㏈��
		  void Update	 ();	 //�X�V
	const void Draw		 ()const;//�`��

	/*getter*/
	//Character&			 GetCharacter		 ();
	//Character&			 GetCharacter		 (const int _enemyIndex = 0);
	const CharacterData& GetCharacterData	  ()const;
	const Rigidbody&	 GetRigidbody		  ()const;
	const bool			 IsAttack			  ()const;
	const int			 GetHP				  (const int _enemyIndent = -1)const;
	const int			 GetModelHandle		  ()const;
	const bool			 GetIsAlive			  ()const;
	const int			 GetAttackCount		  ()const;
	const int			 GetBossState		  ()const;
	const VECTOR		 GetPositionForLockon ()const;
	const int			 GetWeakEnemyNum	  ()const { return this->weakEnemy.size(); }
	void				 SetNearestEnemyIndent(const VECTOR _targetPosition);		//��ԋ߂��G�̔ԍ����擾
	const int			 GetNearestEnemyIndent() { return this->nearestWeakEnemyIndent; }
	const bool			 GetIsEnemyWithinRnage()const;
private:
	/*�񋓑�*/
	enum class WeakEnemyType
	{
		BRAWLER,
		MAGE,
		TANK,
	};
	
	/*���������֐�*/
	 EnemyManager();//�R���X�g���N�^
	~EnemyManager();//�f�X�g���N�^
	void		CreateWeakEnemy();
	void		AddWeakEnemy(int& _indent, const int _weakEnemyType, const int _bossType, vector<Character*>& _list);
	vector<int> GetWeakEnemyTypeList(const int _bossType);

	/*�����o�ϐ�*/
	Character*			boss;					//�{�X
	vector<Character*>	bossList;				//�{�X
	vector<Character*>	weakEnemy;				//�G���G
	int					mapType;				//�}�b�v�̎��
	int					enemyType;				//�{�X�̎��
	int					frameTime;				
	int					nearestWeakEnemyIndent;	//
	bool				isEnemyWithinRange;		//�v���C���[�̎��͂ɓG�����邩�ǂ���


};

