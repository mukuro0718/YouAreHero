//===========================================
// @brief ビヘイビアツリーのノードのベースクラス
//===========================================
#pragma once

class BehaviorTree;
class Character;
class BehaviorTreeNode abstract
{
public:
	/*列挙体*/
	//ノードの状態
	enum class NodeState
	{
		NONE_ACTIVE,//実行されていない
		SUCCESS,	//成功
		FAILURE,	//失敗
		RUNNING,	//実行中
		COMPLETED,	//完了
	};

			 BehaviorTreeNode(); //コンストラクタ
	virtual ~BehaviorTreeNode(){}//デストラクタ

	virtual void	   Initialize(){}												//初期化
	virtual NodeState  Update	 (BehaviorTree& _tree, Character& _chara) abstract;	//更新、ノードの状態を返す
	virtual const void Draw		 ()const {}											//描画
	virtual void	   AddChild	 (BehaviorTreeNode& _child){}						//子供の追加（通常は処理なし）
protected:
	/*メンバ変数*/
	NodeState state;//ノードの状態
};

