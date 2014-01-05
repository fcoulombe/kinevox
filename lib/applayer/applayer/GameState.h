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

#include <deque>
#include <memory>
#include <string>

namespace GCL
{
class GameState;
typedef std::shared_ptr<GameState> GameStatePtr;
class GameState
{
public:
	GameState(const std::string &name)
: mName(name)
	{

	}
	virtual ~GameState()
	{

	}
	void PushChildState(GameStatePtr state)
	{
		mChildStates.push_back(state);
	}
	void PopChildState()
	{
		mChildStates.pop_back();
	}
	virtual bool Update(Real dt)
	{
		for (ChildGameStateList::iterator it = mChildStates.begin(); it != mChildStates.end();)
		{
			if (!(*it)->Update(dt))
			{
				it = mChildStates.erase(it);
			}
			else
				++it;
		}
		return true;
	}
	const std::string &GetStateName()
	{
		return mName;
	}
private:
	typedef std::deque<GameStatePtr> ChildGameStateList;
	ChildGameStateList mChildStates;
	std::string mName;
};

}
