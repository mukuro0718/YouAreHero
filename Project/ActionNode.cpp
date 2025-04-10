#include <DxLib.h>
#include "UseSTL.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
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
}