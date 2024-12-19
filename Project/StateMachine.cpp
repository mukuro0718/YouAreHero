#include "UseSTL.h"
#include "State.h"
#include "StateMachine.h"

/// <summary>
/// コンストラクタ
/// </summary>
StateMachine::StateMachine()
{

}

/// <summary>
/// デストラクタ
/// </summary>
StateMachine::~StateMachine()
{

}

/// <summary>
/// 状態の変更
/// </summary>
void StateMachine::ChangeState(State* _newState)
{
	//状態がnullじゃなければ
	if (this->currentState)
	{
		//現在の状態を終了
		this->currentState->Exit();
	}
	currentState  = _newState;
	currentState->Entry();
}

/// <summary>
/// 関数の追加
/// </summary>
void StateMachine::AddFactory(int _id, std::function<State* (void)> _factory)
{
	this->stateFactoryMap[_id] = _factory;
}

/// <summary>
/// 更新
/// </summary>
void StateMachine::Update()
{
	if (this->currentState)
	{
		//現在の状態に基づいた更新
		this->currentState->Update();
	}
}

/// <summary>
/// 遷移条件をチェックして状態遷移
/// </summary>
void StateMachine::Transition(int _nextId)
{
	/*マップ内にIDが存在していたら*/
	if (this->stateFactoryMap.find(_nextId) != this->stateFactoryMap.end())
	{
		auto nextState = this->stateFactoryMap[_nextId]();
		ChangeState(nextState);
	}
}
