#pragma once
#include "State.h"
class SearchForCover :
	public State
{
public:
	SearchForCover();
	~SearchForCover();

	void Transform(NPC* ps);
	void OnEnter(NPC* ps);
	void OnExit(NPC* ps);
};

