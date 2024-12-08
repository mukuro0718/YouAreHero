//====================================================
// @brief ルクスリオ用ビヘイビアツリー
//====================================================
#pragma once

class BehaviorTree;
class BeastBehaviorTree
{
public:
	/*列挙体*/
	//キャラクターの状態
	enum class BeastState
	{
		ANGRY		= 0,
		NORMAL		= 1,
		DOWN		= 2,
		FRIGHTENING = 3,
		BREAK		= 4,
	};

	 BeastBehaviorTree();
	~BeastBehaviorTree();

	void Update();
private:
	BehaviorTree* root;
};

