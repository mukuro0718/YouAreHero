#include <DxLib.h>
#include "BehaviorTreeNode.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "Enemy.h"
#include "Beast.h"
#include "ActionNode.h"

ActionNode::ActionNode()
	: animationType		 (0)
	, rotateAnimationType(0)
	, actionType		 (0)
	, interval			 (0)
	, animationPlayTime	 (0.0f)
	, rotatePlayTime	 (0.0f)
	, maxSpeed			 (0.0f)
	, accel				 (0.0f)
	, decel				 (0.0f)
	, isFixRotate		 (false)
{
	this->rotateAnimationType = static_cast<int>(Beast::AnimationType::TURN_LEFT);
	auto& json = Singleton<JsonManager>::GetInstance();
	this->rotatePlayTime = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->rotateAnimationType];
}