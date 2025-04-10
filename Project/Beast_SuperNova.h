//=====================================================
// @brief スーパーノヴァアクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class AttackSphereColliderData;
class Beast_SuperNova : public ActionNode
{
public:
	 Beast_SuperNova();//コンストラクタ
	~Beast_SuperNova();//デストラクタ

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
	AnimationStage							 stage;							//アニメーションの段階
	std::map<AnimationStage, int>			 animationSet;					//アニメーションのセット
	std::map<AnimationStage, AnimationStage> nextStageSet;					//次のステージのセット
	short									 effectStartCount;				//攻撃開始フレーム
	short									 attackStartCount;				//攻撃開始フレーム
	short									 attackEndCount;				//攻撃開始フレーム
	short									 frameCount;					//フレームカウント
	short									 frameIndexUsedSpherePosition;	//開く初の中心を決めるためのフレーム番号
	AttackSphereColliderData*				 collider;						//攻撃コライダー
};

