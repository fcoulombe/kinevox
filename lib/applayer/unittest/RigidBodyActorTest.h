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
#include <sstream>

#include <gcl/UnitTest.h>
#include <applayer/Actor.h>
#include <applayer/GCLApplication.h>
#include <applayer/MeshComponent.h>
#include <applayer/RenderComponent.h>
#include <applayer/RigidBodyComponent.h>
#include <applayer/ScriptComponent.h>

using namespace GCL;
namespace RigidBodyActorTest
{

void Test()
{
	KINEVOX_TEST_START
	GCLApplication::Initialize("RigidBodyActorTest");
	{
		Actor actor("TestRigidBodyActor");
		try
		{
			actor.CreateComponent("RenderComponent");
		}
		catch (GCL::GCLException& e)
		{
			std::cerr << e.what() << std::endl;
		}

		actor.CreateComponent("MeshComponent");
		actor.CreateComponent("ScriptComponent");
		actor.CreateComponent("RigidBodyComponent");
		MeshComponent *tempMeshComponent = static_cast<MeshComponent*>(actor.GetComponent("MeshComponent"));
		Assert_Test(!tempMeshComponent->HasMesh());
		tempMeshComponent->SetMesh("ExampleMesh.mesh");
		Assert_Test(tempMeshComponent->HasMesh());
		ScriptComponent *tempScriptComponent = static_cast<ScriptComponent*>(actor.GetComponent("ScriptComponent"));
		tempScriptComponent->SetScript("TestScriptedComponent");

		actor.SetPosition(0.0,0.0,-10.0);
		Assert_Test(actor.GetPosition() == WorldPoint3(0.0,0.0,-10.0));
		Camera cam;
		KINEVOX_TEST_LOOP_START
		GCLApplication::SetViewportCamera(cam); 
		GCLApplication::Update();
		GCLApplication::Render();
		KINEVOX_TEST_LOOP_END
	}
	{
	Actor actor("TestRigidBodyActor", "TestRigidBodyActor");

	actor.SetPosition(0.0,0.0,-10.0);
	Assert_Test(actor.GetPosition() == WorldPoint3(0.0,0.0,-10.0));
	Camera cam;
	KINEVOX_TEST_LOOP_START
		GCLApplication::SetViewportCamera(cam); 
		GCLApplication::Update();
		GCLApplication::Render();
	KINEVOX_TEST_LOOP_END
	}
	GCLApplication::Terminate();
}
}
