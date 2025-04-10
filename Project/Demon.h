//===============================================
// @brief ボスクラス
//===============================================
#pragma once

class Character;
class Enemy;
class DemonBehaviorTree;
class CharacterData;
class CharacterColliderData;
class Demon : public Enemy
{
public:
	 Demon();//コンストラクタ
	~Demon();//デストラクタ

	void		Initialize			() override;		//初期化
	void		Finalize			() override;		//後処理
	void		Update				() override;		//更新
	const void	DrawCharacterInfo	() const override;	//描画
	void		PlayAnimation		();					//アニメーションの再生

	/*列挙体*/
	//アニメーション
	enum class AnimationType
	{
		DYING	,//ダイ
		IDLE	,//待機
	};

	/*getter/setter*/
	const bool GetIsAttack	 ()const override;	//コウゲキしたか
		  void SetAttackCount() override {}		//攻撃回数の設定
private:
	/*静的定数*/
	static constexpr float SHADOW_HEIGHT = 10.0f;//影を投影する高さ
	static constexpr float SHADOW_SIZE	 = 8.0f; //影のサイズ
	static constexpr float LOCKON_OFFSET = 10.0f;//ロックオンオフセット
};

