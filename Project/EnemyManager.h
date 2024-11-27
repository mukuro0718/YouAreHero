//=================================================
// @brief ボスマネージャークラス
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

	void Initialize(); //初期化
	void Finalize();
	void Update(); //更新
	const void Draw		 ()const;

	/*getter*/
	const int	   GetHP()const;
	const CharacterData& GetCharacterData()const;
	const Rigidbody& GetRigidbody()const;
	const bool IsAttack()const;
	const int GetModelHandle()const;
	const bool GetIsAlive()const;
private:
	/*内部処理関数*/
	EnemyManager();//コンストラクタ
	~EnemyManager();//デストラクタ

	/*メンバ変数*/
	std::vector<Character*> boss;//ボス
	int enemyType;
};

