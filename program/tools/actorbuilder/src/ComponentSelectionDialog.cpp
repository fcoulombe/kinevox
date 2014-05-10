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


#include "ComponentSelectionDialog.h"
#include <script/ConfigLua.h>
#include <iostream>
using namespace GCL;


ComponentSelectionDialog::ComponentSelectionDialog(QWidget *parent)
    : QDialog(  parent)
{
	this->setModal(true);
	QStringList all_dirs;
	all_dirs << "data/ComponentDef";
	QDirIterator directories("data/ComponentDef", QDir::Files | QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

	QVBoxLayout *layout = new QVBoxLayout(this);
	while(directories.hasNext())
	{
		directories.next();
		all_dirs << directories.filePath();
		VisualComponent comp;
		ConfigLua compConf(directories.filePath().toStdString().c_str());
		const std::string componentName = compConf.GetString("Component.Name");
		comp.mName = componentName ;
		//mComponentList.push_back(compConf.GetString("Component.Name"));
		PtrLuaTableIterator it = compConf.GetTableIterator("Component.Parameters");
		while (!it->End())
		{
			const std::string parameter = it->GetKey();
			const std::string parameterType = it->GetString();
			comp.mParameters[parameter] = parameterType ;
			//PtrLuaTableIterator compIt = it->GetTableIterator();
			++(*it);
		}
		mComponentList[componentName] =  comp;
		QPushButton *componentCreate = new QPushButton(QString(componentName.c_str()), this);
		connect(componentCreate, SIGNAL(released()), this, SLOT(createComponent()));
		layout->addWidget(componentCreate);
	}
	this->setLayout(layout);
}
void ComponentSelectionDialog::createComponent()
{
	QPushButton *button = static_cast<QPushButton*>(QObject::sender());
	mCurrentComponent = mComponentList[button->text().toStdString()];
	close();
}
