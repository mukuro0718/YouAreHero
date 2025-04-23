//=================================================
// @brief �{�X�}�l�[�W���[�N���X
//=================================================
#pragma once
#include "Singleton.h"

class Character;
class CharacterData;
class Rigidbody;
class BrawlerEnemy;
class MageEnemy;
class TankEnemy;
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
	const CharacterData&	 GetCharacterData		()const;
	const Rigidbody&		 GetRigidbody			(const int _enemyIndent = -1)const;
	const bool				 IsAttack				()const;
	const int				 GetHP					(const int _enemyIndent = -1)const;
	const int				 GetModelHandle			()const;
	const bool				 GetIsAlive				()const;
	const int				 GetAttackCount			()const;
	const int				 GetBossState			()const;
	const VECTOR			 GetPositionForLockon	()const;
	const int				 GetWeakEnemyNum		()const { return this->weakEnemy.size(); }
	void					 SetNearestEnemyIndent	(const VECTOR _targetPosition);		//��ԋ߂��G�̔ԍ����擾
	const int				 GetNearestEnemyIndent	() { return this->nearestWeakEnemyIndent; }
	const bool				 GetIsEnemyWithinRange	()const;
	const vector<Character*>& GetWeakEnemyList		() { return this->weakEnemy; }
private:
	/*�ÓI�萔*/
	static constexpr int WEAK_ENEMY_POOL = 10;
	static constexpr int ATTACK_SWITCH_INTERVAL = 120;

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
	void UpdateWeakEnemyAttackPermission();

	/*�����o�ϐ�*/
	Character*				boss;					//�{�X
	vector<Character*>		bossList;				//�{�X
	vector<Character*>		weakEnemy;				//�G���G
	vector<BrawlerEnemy*>	brawlerEnemyList;		//�G���G�̃��X�g
	vector<MageEnemy*>		mageEnemyList;			//�G���G�̃��X�g
	vector<TankEnemy*>		tankEnemyList;			//�G���G�̃��X�g
	vector<VECTOR>			stageCenter;			//�G���G�����݂���X�e�[�W�̒������W
	int						mapType;				//�}�b�v�̎��
	int						enemyType;				//�{�X�̎��
	int						frameTime;				
	int						nearestWeakEnemyIndent;	//
	bool					isEnemyWithinRange;		//�v���C���[�̎��͂ɓG�����邩�ǂ���
	int						useBrawlerEnemyIndex;
	int						useMageEnemyIndex;
	int						useTankEnemyIndex;
	int						attackSwitchTimer;

};

