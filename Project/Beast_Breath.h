//=====================================================
// @brief ブレスアクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class AttackCapsuleColliderData;
class Beast_Breath : public ActionNode
{
public:
	 Beast_Breath();//コンストラクタ
	~Beast_Breath();//デストラクタ

	void		Initialize	();													//初期化
	NodeState	Update		(BehaviorTree& _tree, Character& _chara)override;	//更新
	const void	Draw		()const;											//描画
private:
	/*構造体*/
	//アニメーションの段階
	enum class AnimationStage
	{
		START,
		LOOP,
		END,
	};

	/*メンバ変数*/
	AnimationStage stage;
	std::map<AnimationStage, int>			 animationSet;					 //アニメーションのセット
	std::map<AnimationStage, AnimationStage> nextStageSet;					 //次のステージのセット
	short									 attackStartCount;				 //攻撃開始フレーム
	short									 attackEndCount;				 //攻撃終了フレーム
	short									 frameCount;					 //フレームカウント
	short									 frameIndexUsedCapsuleDirection1;//ブレスの方向を決めるためのフレーム番号
	short									 frameIndexUsedCapsuleDirection2;//ブレスの方向を決めるためのフレーム番号
	AttackCapsuleColliderData*				 collider;						 //攻撃コライダー
	float									 breathLength;					 //ブレスの長さ
};

