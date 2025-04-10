//=====================================================
// @brief 突撃アクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class AttackCapsuleColliderData;
class Beast_Rush : public ActionNode
{
public:
	 Beast_Rush();//コンストラクタ
	~Beast_Rush();//デストラクタ

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
		NORMAL_END,
		ANGRY_END,
	};

	/*定数*/
	const float STOP_DISTANCE;//停止距離
	const short FOOTSTEPS_SOUND_FRAME;//足音を再生する間隔

	/*メンバ変数*/
	short									 attackStartCount;				 //攻撃開始フレーム
	short									 attackEndCount;				 //攻撃終了フレーム
	short									 frameCount;					 //フレームカウント
	short									 frameIndexUsedCapsuleDirection1;//ブレスの方向を決めるためのフレーム番号
	short									 frameIndexUsedCapsuleDirection2;//ブレスの方向を決めるためのフレーム番号
	AnimationStage							 stage;							 //アニメーションの段階
	std::map<AnimationStage, int>			 animationSet;					 //アニメーションのセット
	std::map<AnimationStage, AnimationStage> nextStageSet;					 //次のステージのセット
	AttackCapsuleColliderData*				 collider;						 //攻撃コライダー
};

