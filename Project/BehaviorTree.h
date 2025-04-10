//============================================================
// @brief ビヘイビアツリーベースクラス
//============================================================
#pragma once

class BehaviorTreeNode;
class Character;
class BehaviorTree abstract
{
public:
			 BehaviorTree();//コンストラクタ
	virtual ~BehaviorTree();//デストラクタ

	virtual void		Initialize	()					abstract;		//初期化
	virtual void		Update		(Character& _chara) abstract;		//更新
	virtual const void	Draw		()					const abstract;	//描画

	/*getter/setter*/
	const int		  GetNowSelectAction		()const				{ return this->selectAction; }					 //選択されているアクションを取得
	const bool		  GetIsCancelAction			()const				{ return this->isCancelAction; }				 //キャンセルフラグの取得
	BehaviorTreeNode& GetBattleAction			()					{ return *this->currentBattleNode; }			 //現在実行中のバトルアクション
	BehaviorTreeNode& GetReaction				()					{ return *this->currentReactionNode; }			 //現在実行中のバトルアクション
	const int		  GetInterval				(const int _index)	{ return this->intervalSet[_index]; }			 //インターバルの取得
	const float		  GetDotOfDirAndToTarget	()const				{ return this->innerProductOfDirectionToTarget; }//内積の取得
	const float		  GetToTargetDistance		()const				{ return this->toTargetDistance; }				 //目標までの距離を取得
	const bool		  GetIsSelectedBattleAction	()const				{ return this->isSelectedBattleAction; }		 //部位破壊されているか
	const bool		  GetIsSelectedReaction		()const				{ return this->isSelectedReaction; }			 //部位破壊されているか
	const int		  GetPrevHp					()const				{ return this->prevHp; }						 //前フレームのHPの取得
	const bool		  GetIsDestroyedPart		()const				{ return false; }								 //部位破壊されているか
	const int		  GetLevel					()const				{ return 0; }									 //レベルの取得
	const int		  GetDamage					()const				{ return this->damage; }						 //ダメージの取得

	void ResetLevel			()										 { this->level = 0; }					//レベルを上げる
	void RaiseLevel			()										 { this->level++; }						//レベルを上げる
	void SetNowSelectAction	(const int _actionType)					 { this->selectAction = _actionType; }	//選択されたアクションを設定
	void SetInterval		(const int _index, const int _set = 600) { this->intervalSet[_index] = _set; }	//インターバルの設定
	void SetDamage			(const int _set)						 { this->damage = _set; }				//ダメージを設定

	void EntryCurrentBattleAction(BehaviorTreeNode& _action);	//バトルアクションの登録
	void ExitCurrentBattleAction ();							//バトルアクションの解除
	void EntryCurrentReaction	 (BehaviorTreeNode& _action);	//リアクションアクションの登録
	void ExitCurrentReaction	 ();							//リアクションアクションの解除
protected:
	BehaviorTreeNode*			mainNode;						//メインノード
	BehaviorTreeNode*			debugNode;						//デバック用アクションノード
	BehaviorTreeNode*			currentBattleNode;				//バトルアクションが選択されているときに実行されるノード
	BehaviorTreeNode*			currentReactionNode;			//リアクションが選択されているときに実行されるノード
	BehaviorTreeNode::NodeState nodeState;						//ノードの状態を代入する変数
	vector<int>					intervalSet;					//インターバル
	float						toTargetDistance;				//目標までの距離
	float						innerProductOfDirectionToTarget;//キャラクターの向きベクトルと目標へのベクトルの内積
	int							prevHp;							//前フレームのHP
	int							damage;							//今までに受けたダメージ量（怒り状態が終了すると０にリセットする）
	int							selectAction;					//現在選択中のアクション
	int							attackCount;					//攻撃可能回数
	int							level;							//ボスのレベル
	bool						isSelectedBattleAction;			//攻撃アクションが選択されているか
	bool						isSelectedReaction;				//リアクションが選択されているか
	bool						isCancelAction;					//アクションがキャンセルされたか
};

/*
memo
current~の変数は、各アクションが選択されたとき再度ツリーを実行することなく
アクションを実行するためのサブノード
これにより、攻撃中やリアクション中にほかのアクションが実行されることを防ぐ
*/