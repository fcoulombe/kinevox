/*
 * Copyright (C) 2014 by Francois Coulombe
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include "applayer/GameState.h"
#include <stack>
namespace GCL
{
class GameStateManager
{
public:
	static void Initialize()
	{

	}
	static void Terminate()
	{
		CleanStates();
	}
	static void ChangeToState(GameStatePtr state)
	{
		CleanStates();
		mStates.push(state);
	}
	static void PushState(GameStatePtr state)
	{
		mStates.push(state);
	}
	static void PopState()
	{
		mStates.pop();
	}
	static void CleanStates()
	{
		while (!mStates.empty())
			mStates.pop();
	}

	static void Update(Real dt)
	{
		if (mStates.size())
			mStates.top()->Update(dt);
	}
	static const std::string &GetCurrentStateName() { GCLAssert(!mStates.empty()); return mStates.top()->GetStateName(); }
private:
	static std::stack<GameStatePtr> mStates;

};
}
