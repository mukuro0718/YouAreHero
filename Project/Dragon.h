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
	void		PlayAnimation		(const int _playAnimation, const float _playTime);	//アニメーションの再生
	void		OnAttackCollider	(const short _index);								//指定の攻撃コライダーのフラグを立てる
	void		OffAttackCollider	(const short _index);								//指定の攻撃コライダーのフラグを下す
	void		UpdateAttackCollider(const short _colliderIndex, const float _nowTotalPlayTime);//指定の攻撃コライダーを更新

	/*列挙体*/
	//アニメーション
	enum class AnimationType
	{
		 DYING			= 0,//デス3
		 IDLE			= 1,//待機19
		 ROAR			= 2,//咆哮9
		 WALK			= 3,//歩き26
		 TURN_LEFT		= 4,//左を向く21
		 TURN_RIGHT		= 5,//右を向く22
		 SMASH			= 6,//叩きつける1
		 SWEEP			= 7,//なぎ払い0
		 ROTATE_ATTACK	= 8,//回転攻撃18
		 BREATH			= 9,//ブレス12
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
		SMASH = 0,
		SWEEP = 1,
		ROTATE= 2,
		BREATH= 3,
	};
	/*getter/setter*/
	const bool GetIsAttack	()const override;			//コウゲキしたか
		  void OffIsDraw	(){ this->isDraw = false; }	//描画フラグを下す
private:
	/*静的定数*/
	static constexpr float SHADOW_HEIGHT = 10.0f;//影を投影する高さ
	static constexpr float SHADOW_SIZE	 = 30.0f; //影のサイズ

	/*メンバ変数*/
	map<short,vector<AttackCapsuleColliderData*>>				attackCollider;				//攻撃コライダー
	map<short, vector<vector<short>>>							frameIndexUseAttackColider;	//攻撃コライダーに使用するフレーム番号
	vector<CharacterColliderData*>								partsCollider;				//部位ごとのコライダー
	vector<vector<short>>										frameIndexUsePartsColider;	//部位ごとのコライダーの指定に使用するフレーム番号
	array<float,static_cast<int>(AttackCollider::BREATH) + 1>	startHitCheckPlayTime;		//当たり判定開始フレーム
	array<float,static_cast<int>(AttackCollider::BREATH) + 1>	endHitCheckPlayTime;		//当たり判定終了フレーム
	array<bool, static_cast<int>(AttackCollider::BREATH) + 1>	isStartHitCheck;			//当たり判定終了フレーム
	short														maxPartsColliderNum;		//コライダーの数
	vector<int>													prevPartsHp;				//1フレーム前の部位ごとのHP
	int															maxHp;						//部位の最大HP
	float														breathLength;				//ブレスの長さ

};

