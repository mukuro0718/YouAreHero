//===============================================
// @brief ボスクラス
//===============================================
#pragma once

class Character;
class Enemy;
class DragonBehaviorTree;
class CharacterData;
class CharacterColliderData;
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

	/*getter/setter*/
	const bool GetIsAttack	()const override;			//コウゲキしたか
		  void OffIsDraw	(){ this->isDraw = false; }	//描画フラグを下す
private:
	/*静的定数*/
	static constexpr float SHADOW_HEIGHT = 10.0f;//影を投影する高さ
	static constexpr float SHADOW_SIZE	 = 8.0f; //影のサイズ
};

