//=================================================
// @brief ボスマネージャークラス
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

		  void Initialize();	 //初期化
		  void Finalize	 ();	 //後処理
		  void Update	 ();	 //更新
	const void Draw		 ()const;//描画

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
	void					 SetNearestEnemyIndent	(const VECTOR _targetPosition);		//一番近い敵の番号を取得
	const int				 GetNearestEnemyIndent	() { return this->nearestWeakEnemyIndent; }
	const bool				 GetIsEnemyWithinRange	()const;
	const vector<Character*>& GetWeakEnemyList		() { return this->weakEnemy; }
private:
	/*静的定数*/
	static constexpr int WEAK_ENEMY_POOL = 10;
	static constexpr int ATTACK_SWITCH_INTERVAL = 120;

	/*列挙体*/
	enum class WeakEnemyType
	{
		BRAWLER,
		MAGE,
		TANK,
	};
	
	/*内部処理関数*/
	 EnemyManager();//コンストラクタ
	~EnemyManager();//デストラクタ
	void		CreateWeakEnemy();
	void		AddWeakEnemy(int& _indent, const int _weakEnemyType, const int _bossType, vector<Character*>& _list);
	vector<int> GetWeakEnemyTypeList(const int _bossType);
	void UpdateWeakEnemyAttackPermission();

	/*メンバ変数*/
	Character*				boss;					//ボス
	vector<Character*>		bossList;				//ボス
	vector<Character*>		weakEnemy;				//雑魚敵
	vector<BrawlerEnemy*>	brawlerEnemyList;		//雑魚敵のリスト
	vector<MageEnemy*>		mageEnemyList;			//雑魚敵のリスト
	vector<TankEnemy*>		tankEnemyList;			//雑魚敵のリスト
	vector<VECTOR>			stageCenter;			//雑魚敵が存在するステージの中央座標
	int						mapType;				//マップの種類
	int						enemyType;				//ボスの種類
	int						frameTime;				
	int						nearestWeakEnemyIndent;	//
	bool					isEnemyWithinRange;		//プレイヤーの周囲に敵がいるかどうか
	int						useBrawlerEnemyIndex;
	int						useMageEnemyIndex;
	int						useTankEnemyIndex;
	int						attackSwitchTimer;

};

