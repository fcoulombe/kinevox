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
#include "KinevoxQtViewPort.h"

using namespace GCL;

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
	///myMesh.Render();
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
