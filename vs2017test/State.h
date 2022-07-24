#pragma once
#include "NPC.h"


class State
{
public:
	virtual void Transform(NPC* ps) = 0;
	virtual void OnEnter(NPC* ps) = 0;
	virtual void OnExit(NPC* ps) = 0;
};

