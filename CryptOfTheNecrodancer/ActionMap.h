#pragma once

#include "IManager.h"
#include "IManagable.h"

#include "Action.h"

class ActionMap : public IManager<string, Action>, public IManagable<string>
{
	vector<Action*> actions;

public:
	ActionMap(const string& _name, const vector<ActionData>& _actions = vector<ActionData>());
	~ActionMap();

private:
	virtual void Register() override;

public:
	void Update(const Event& _event);
};