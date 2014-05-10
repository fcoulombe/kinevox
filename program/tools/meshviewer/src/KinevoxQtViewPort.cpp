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


#include <3rdparty/Qt.h>
#include <math.h>

#include <renderer/Renderer.h>
#include <renderer/Mesh.h>
#include "KinevoxQtViewPort.h"

using namespace GCL;

namespace GCL
{
class MeshRenderObject 
{
public:
	MeshRenderObject()
		: mTransform(true),
		mMesh(MESH_PATH"ExampleMesh.mesh")

	{
		VertexData data;
		data.mVertexCount =mMesh.GetVertexCount(0);
		data.mVertexType = mMesh.GetVertexType();
		data.mVertexData = mMesh.GetVertexData(0);
		mVertexData.push_back(data);
		switch ((size_t)mMesh.GetVertexType())
		{
		case ePOSITION:
			obj = new RenderObject(mMesh.GetMaterial(), (const VertexP*)mMesh.GetVertexData(0), mMesh.GetVertexCount(0));
			break;
		case ePOSITION|eNORMAL:
			obj = new RenderObject(mMesh.GetMaterial(), (const VertexPN*)mMesh.GetVertexData(0), mMesh.GetVertexCount(0));
			break;
		case ePOSITION|eNORMAL|eTEXTURE_COORD:
			obj = new RenderObject(mMesh.GetMaterial(), (const VertexPNT*)mMesh.GetVertexData(0), mMesh.GetVertexCount(0));
			break;
		default:
			GCLAssert(false);
		}
	}
	~MeshRenderObject()
	{
		delete obj;
	}
	void Render()
	{
		const RenderObject *tempRenderObject = obj;
		const Material &tempMaterial = tempRenderObject->GetMaterial();
		tempMaterial.Bind();
		const Matrix44 &transform = mTransform;
		GPUProgram *tempProgram = tempMaterial.GetShader();
		tempProgram->SetProjectionMatrix();
		tempProgram->SetModelViewMatrix(transform);
		tempRenderObject->GetVBO().Render();
	}
	void SetPosition(Real x, Real y, Real z) { mTransform.SetPosition(x,y,z); }

	void SetOrientation(Real x, Real y, Real z) 
	{
		const WorldPoint4 backupPosition = mTransform[3];
		Matrix44 xRot;
		xRot.SetRotationX(x);
		Matrix44 yRot;
		yRot.SetRotationY(y);
		Matrix44 zRot;
		zRot.SetRotationZ(z);
		mTransform = xRot * yRot;// * zRot;

		mTransform.SetPosition(backupPosition);
	}
	RenderObject *GetObj() { return obj; }
private:
	Matrix44 mTransform;
	Mesh mMesh;
	RenderObject *obj;
	VertexDataList mVertexData;
};
}
KinevoxQtViewPort::KinevoxQtViewPort(QWidget *parent)
    : QWidget ( parent)
{
	mRenderer = nullptr;
	QPalette colourPalette = palette();
	colourPalette.setColor( QPalette::Active, QPalette::WindowText, Qt::black );
	colourPalette.setColor( QPalette::Active, QPalette::Window, Qt::black );
	setPalette( colourPalette );

	initialize();
}
void KinevoxQtViewPort::initialize()
{
	setAttribute(Qt::WA_NoSystemBackground);
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAutoFillBackground(false); 
	setFocusPolicy( Qt::StrongFocus );
	setMinimumSize(320,240);
	WId id  = this->winId();
	if (mRenderer)
		delete mRenderer;
	mRenderer = new Renderer((size_t)id);
	mesh = new MeshRenderObject();
	 mesh->SetPosition(0.0,0.0,-10.0);
	timer.start(60, this);
}
bool KinevoxQtViewPort::isInitialized() const
{
	return mRenderer!=nullptr;
}
void KinevoxQtViewPort::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == timer.timerId()) 
	{
		//++step;
		update();
	} 
	else 
	{
		QWidget::timerEvent(event);
	}

}


KinevoxQtViewPort::~KinevoxQtViewPort()
{
	delete mRenderer;
}

QSize KinevoxQtViewPort::minimumSizeHint() const
{
    return QSize(320, 240);
}

QSize KinevoxQtViewPort::sizeHint() const
{
    return QSize(800, 600);
}

void KinevoxQtViewPort::paintEvent(QPaintEvent *evt)
{
	if ( ! isInitialized() )
	{
		return;
	}
	//std::cout << "render"<<std::endl;
	mRenderer->PreRender();
	mesh->Render();
	mRenderer->PostRender();
	evt->accept();
}
QPaintEngine* KinevoxQtViewPort::paintEngine() const
{
	return NULL;
}



void KinevoxQtViewPort::resizeEvent(QResizeEvent *e)
{
	QWidget::resizeEvent(e);
	if(e->isAccepted())
	{
		const QSize &newSize = e->size();
		int side = qMin(newSize.width(), newSize.height());
		ViewPort viewport;
		viewport.Set((newSize.width() - side) / 2,(newSize.height() - side) / 2,side,side);
		mRenderer->SetViewPort(viewport);
		//glViewport((width - side) / 2, (height - side) / 2, side, side);
	}
}

void KinevoxQtViewPort::mousePressEvent(QMouseEvent * /*event*/)
{

}

void KinevoxQtViewPort::mouseMoveEvent(QMouseEvent * /*event*/)
{

}
