//==========================================================================
// @brief ステートマシン
//==========================================================================
#pragma once

class State;
class StateMachine
{
public:
			 StateMachine();//コンストラクタ
	virtual ~StateMachine();//デストラクタ

	void ChangeState(State* _newState);//状態の変更
	void AddFactory(int _id, std::function<State* (void)> _factory);//関数の追加
	void Update();//更新
	void Transition(int _nextId);//次のID
private:
	State* currentState;//現在の状態
	std::map<int, std::function<State* (void)>> stateFactoryMap;//状態偏移マップ
};

