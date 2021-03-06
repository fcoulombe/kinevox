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

#include <applayer/GCLRenderObject.h>
#include <applayer/GCLApplication.h>
#include <gcl/Time.h>
#include <input/Input.h>
#include <renderer/Camera.h>
#include <renderer/Material.h>
#include <renderer/Mesh.h>
#include <renderer/GLRenderer.h>


using namespace GCL;


class MeshRenderObject : public RenderObject
{
public:
	MeshRenderObject()
	: RenderObject("MyRenderObject", Matrix44(true)), //identity
	  mMesh(MESH_PATH"ExampleMesh.mesh")
	{
        VertexData data;
		data.mVertexCount =mMesh.GetVertexCount(0);
		data.mVertexType = mMesh.GetVertexType();
		data.mVertexData = mMesh.GetVertexData(0);
        mVertexData.push_back(data);
	}

	const VertexDataList &GetVertexData() const
	{
		return mVertexData;
	}
	const Material &GetMaterial() const { return mMesh.GetMaterial(); }
private:
	Mesh mMesh;
	VertexDataList mVertexData;
};

int main(int /*argc*/, char ** /*argv*/)
{
	std::cout << "start program" << std::endl;

	try
	{

		GCLApplication::Initialize("Mesh Loading Example");
		Camera myCamera;
		GCLApplication::SetViewportCamera(myCamera);


		MeshRenderObject myMesh;
		GCLApplication::RegisterCustomRenderObject(&myMesh);

		myMesh.SetPosition(0,0,-10.0);


		Real y = 0.0;
		bool isRunning = true;

		while (isRunning)
		{
			y+=0.05;
			myMesh.SetOrientation(0.0, y,0.0);

			GCLApplication::Update();
			if (Input::IsKeyUp(GCL_ESCAPE))
				isRunning=false;
			if (Input::IsKeyUp(GCL_UP))
				myCamera.MoveForward();
			if (Input::IsKeyUp(GCL_DOWN))
				myCamera.MoveBackward();

			if (Input::IsKeyUp(GCL_LEFT))
				myCamera.TurnLeft();
			if (Input::IsKeyUp(GCL_RIGHT))
				myCamera.TurnRight();

			if (Input::IsKeyUp(GCL_PAGEUP))
				myCamera.TiltUp();
			if (Input::IsKeyUp(GCL_PAGEDOWN))
				//myCamera.TiltDown();
			{
				static bool isTextureOn = false;
				isTextureOn = !isTextureOn;
				if (isTextureOn)
				{
					std::cout << "on" << std::endl;
					GCLApplication::GetRenderer()->mCurrentRenderState.SetTextureEnabled(true);
				}
				else
				{
					std::cout << "off" << std::endl;
					GCLApplication::GetRenderer()->mCurrentRenderState.SetTextureEnabled(false);
				}
			}

			GCLApplication::Render();
			Time::SleepMs(100);
			std::cout.flush();
		}
	}
	catch(GCLException &e)
	{
		std::cerr << e.what() << std::endl;
	}


	GCLApplication::Terminate();

	return 0;
}

