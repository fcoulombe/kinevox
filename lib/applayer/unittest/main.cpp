/*
 * Copyright (C) 2011 by Francois Coulombe
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
#include <iostream>
#ifdef USE_VLD
#include <vld.h>
#endif
#include <kinetestlib/UnitTest.h>

#include "GCLApplicationTest.h"
#include "ActorTest.h"
#include "NodeTest.h"
#include "RigidBodyActorTest.h"
#include "GameStateTest.h"
#include "SpriteTest.h"
#include "SpriteComponentTest.h"
#include "ScriptComponentTest.h"
#include "ScriptedGameStateTest.h"
#include "GCLApplicationCameraTest.h"
#include "GCLRenderTargetTest.h"
//#include "GCLText2DTest.h"
#include "GCLWorldTest.h"

int main(int argc, char ** argv)
{
	 SUITE_INIT(argc, argv)
		NodeTest::Test();
#if 1
	 GCLApplicationTest::Test();
	 GCLApplicationCameraTest::Test();
	 SpriteTest::Test();	 
	 ActorTest::Test();
	 RigidBodyActorTest::Test();
	 SpriteComponentTest::Test();
	 ScriptComponentTest::Test();
	 GameStateTest::Test();
	 ScriptedGameStateTest::Test();
	 GCLRenderTargetTest::Test();
	 //GCLText2DTest::Test();
#endif
     GCLWorldTest::Test();
		SUITE_TERMINATE

	return 0;
}
