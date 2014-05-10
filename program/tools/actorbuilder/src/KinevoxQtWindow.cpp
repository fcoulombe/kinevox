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

#include <iostream>
#include "KinevoxQtViewPort.h"
#include "KinevoxQtWindow.h"
#include "InspectorDock.h"
#include "ResourceBrowser.h"

#include <applayer/Actor.h>
#include <applayer/GCLApplication.h>
#include <renderer/TextureResourceManager.h>
#include <renderer/FontResourceManager.h>
#include <renderer/MeshResourceManager.h>
#include <script/ScriptResourceManager.h>

using namespace GCL;
KinevoxQtWindow::~KinevoxQtWindow()
{
	MeshResourceManager::Terminate();
	ScriptResourceManager::Terminate();
	FontResourceManager::Terminate();
	TextureResourceManager::Terminate();
}
KinevoxQtWindow::KinevoxQtWindow()
{
	TextureResourceManager::Initialize();
	FontResourceManager::Initialize();
	ScriptResourceManager::Initialize();
	MeshResourceManager::Initialize();
	//mScriptApi = new ScriptApi();
	GCLApplication::InitializaAppLayerComponents();
	//GameStateManager::Initialize();
	setWindowTitle(tr("Actor Builder"));

	QDockWidget *dock = new QDockWidget(tr("console"), this);
	dock->setAllowedAreas( Qt::AllDockWidgetAreas);
	addDockWidget(Qt::BottomDockWidgetArea, dock);
	QTextEdit *customerList = new QTextEdit(dock);
	customerList->insertPlainText(QString("John Doe, Harmony Enterprises, 12 Lakeside, Ambleton"));
	dock->setWidget(customerList);

	dock = new QDockWidget(tr("resources"), this);
	dock->setAllowedAreas( Qt::AllDockWidgetAreas);
	addDockWidget(Qt::BottomDockWidgetArea, dock);
	ResourceBrowser *resourceBrowser = new ResourceBrowser(this);
	dock->setWidget(resourceBrowser);

	dock = new QDockWidget(tr("viewport"), this);
	dock->setAllowedAreas(Qt::AllDockWidgetAreas );
	addDockWidget(Qt::LeftDockWidgetArea, dock);
	mViewport = new KinevoxQtViewPort(dock);
	dock->setWidget(mViewport);


	mInspectorDock = new InspectorDock(this);
	addDockWidget(Qt::RightDockWidgetArea, mInspectorDock);


	CreateMenus();
	mActor = nullptr;
	NewActor();
}

void KinevoxQtWindow::NewActor()
{
	if (mActor)
		delete mActor;
	mActor = new Actor("TestActor");
	mInspectorDock->SetActor(mActor);
}
void KinevoxQtWindow::SaveActor()
{
	mInspectorDock->SaveActor();
}
void KinevoxQtWindow::CreateMenus()
{
	newAct = new QAction(tr("&New"), this);
	newAct->setShortcuts(QKeySequence::New);
	newAct->setStatusTip(tr("Create a new file"));
	connect(newAct, SIGNAL(triggered()), this, SLOT(NewActor()));

	saveAct = new QAction(tr("&Save"), this);
	saveAct->setShortcuts(QKeySequence::Save);
	saveAct->setStatusTip(tr("Save Actor To File"));
	connect(saveAct, SIGNAL(triggered()), this, SLOT(SaveActor()));

	exitAct = new QAction(tr("&Exit"), this);
	exitAct->setShortcuts(QKeySequence::Quit);
	exitAct->setStatusTip(tr("Exit program"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));


	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newAct);
	//fileMenu->addAction(openAct);
	fileMenu->addAction(saveAct);
	//fileMenu->addAction(printAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);
}


void KinevoxQtWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}
