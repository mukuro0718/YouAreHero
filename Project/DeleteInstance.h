#pragma once
template<typename T>
void DeleteMemberInstance(T& _instance)
{
	if (_instance != nullptr)
	{
		delete(_instance);
		_instance = nullptr;
	}
}