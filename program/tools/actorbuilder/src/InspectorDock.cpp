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


#include "InspectorDock.h"
#include "ComponentSelectionDialog.h"
#include "ComponentView.h"
#include "FileBrowser.h"
#include <iostream>
#include <applayer/Actor.h>
using namespace GCL;

InspectorDock::InspectorDock(QWidget *parent)
	: QDockWidget( tr("inspector"), parent)
{
	setAllowedAreas(Qt::AllDockWidgetAreas );
	QWidget *w = new QWidget(this);

	mLayout = new QVBoxLayout(w);
	w->setLayout(mLayout);
	QPushButton *addComponentBtn = new QPushButton("Add Component...", w);
	connect(addComponentBtn, SIGNAL(released()), this, SLOT(createComponentSelectionDialog()));
	mLayout->addWidget(addComponentBtn);
	this->setWidget(w);
}

void InspectorDock::createComponentSelectionDialog()
{
	mComponentSelectionDialog = new ComponentSelectionDialog(this);
	mComponentSelectionDialog->exec();
	mComponentSelectionDialog->raise();
	mComponentSelectionDialog->activateWindow();
	const VisualComponent &comp = mComponentSelectionDialog->GetCurrentComponent();
	try
	{
		const std::string &compName = comp.mName+"Component";
		mActor->CreateComponent(compName);
		//Component *tempComponent = mActor->GetComponent(compName);
	}
	catch (GCLException & e)
	{
		std::cerr << e.what() << std::endl;
		return;
	}
	ComponentView *compView = new ComponentView(comp, this);
	mLayout->addWidget(compView);
	delete mComponentSelectionDialog;
}
void InspectorDock::SaveActor()
{

}
void InspectorDock::SetActor(Actor *actor)
{
	mActor = actor;
}