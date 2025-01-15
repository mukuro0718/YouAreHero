//===============================================
// @brief ボスクラス
//===============================================
#pragma once

class Character;
class Enemy;
class DragonBehaviorTree;
class CharacterData;
class CharacterColliderData;
class AttackCapsuleColliderData;
class Dragon : public Enemy
{
public:
	 Dragon();//コンストラクタ
	~Dragon();//デストラクタ

	void		Initialize			() override;										//初期化
	void		Finalize			() override;										//後処理
	void		Update				() override;										//更新
	const void	DrawCharacterInfo	() const override;									//描画
	void		PlayAnimation		(const float _playAnimation, const float _playTime);//アニメーションの再生
	void		OnAttackCollider	(const short _index);								//指定の攻撃コライダーのフラグを立てる
	void		UpdateAttackCollider(const short _index);								//指定の攻撃コライダーを更新
	void		OffAttackCollider	(const short _index);								//指定の攻撃コライダーのフラグを下す

	/*列挙体*/
	//アニメーション
	enum class AnimationType
	{
		 DYING			= 0,//デス
		 IDLE			= 1,//待機
		 ROAR			= 2,//咆哮
		 WALK			= 3,//歩き
		 TURN_LEFT		= 4,//左を向く
		 TURN_RIGHT		= 5,//右を向く
		 SMASH			= 6,//叩きつける
		 SWEEP			= 7,//なぎ払い
		 ROTATE_ATTACK	= 8,//回転攻撃
		 BREATH			= 9,//ブレス
	};
	//部位ごとのコライダー
	enum class PartsCollider
	{
		HEAD		= 0,
		BODY		= 1,
		LEFT_HAND	= 2,
		RIGHT_HAND	= 3,
		LEFT_FOOT	= 4,
		RIGHT_FOOT	= 5,
		TAIL1		= 6,
		TAIL2		= 7,
		TAIL3		= 8,
	};
	//攻撃コライダー
	enum class AttackCollider
	{
		RIGHT_HAND	= 0,
		TAIL1		= 1,
		TAIL2		= 2,
		TAIL3		= 3,
		BREATH		= 4
	};
	/*getter/setter*/
	const bool GetIsAttack	()const override;			//コウゲキしたか
		  void OffIsDraw	(){ this->isDraw = false; }	//描画フラグを下す
private:
	/*静的定数*/
	static constexpr float SHADOW_HEIGHT = 10.0f;//影を投影する高さ
	static constexpr float SHADOW_SIZE	 = 30.0f; //影のサイズ

	/*メンバ変数*/
	vector<AttackCapsuleColliderData*>  attackCollider;				//部位ごとの攻撃コライダー
	vector<CharacterColliderData*>		partsCollider;				//部位ごとのコライダー
	vector<vector<short>>				frameIndexUsePartsColider;	//部位ごとのコライダーの指定に使用するフレーム番号
	vector<vector<short>>				frameIndexUseAttackColider;	//攻撃のコライダーの指定に使用するフレーム番号
	short								maxPartsColliderNum;		//コライダーの数
	vector<float>						prevPartsHp;				//1フレーム前の部位ごとのHP
	float								maxHp;						//部位の最大HP
	float								breathLength;				//ブレスの長さ

};

