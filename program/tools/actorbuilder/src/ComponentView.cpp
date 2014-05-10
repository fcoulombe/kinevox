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


#include "ComponentView.h"
#include "FileBrowser.h"
#include "VisualComponentDef.h"

#include <iostream>

using namespace GCL;

GCL::ComponentView::ComponentView( const VisualComponent &comp, QWidget *parent /*= 0*/ ) : QGroupBox(tr(comp.mName.c_str()), parent)
{
	QVBoxLayout *vbox = new QVBoxLayout;
	for (auto it = comp.mParameters.begin(); it != comp.mParameters.end(); ++it)
	{
		QLabel *parameterName = new QLabel(	tr(it->first.c_str()), this);
		vbox->addWidget(parameterName);
		const std::string &parameterTypeName = it->second;
		if (parameterTypeName == "filepath")
		{
			FileBrowser *parameterType = new FileBrowser(this);
			vbox->addWidget(parameterType);
			connect(parameterType, SIGNAL(EmitLineEditChange(const std::string &)), this, SLOT(ValueChanged(const std::string &, parameterName)));
		}
		else if (parameterTypeName == "number")
		{}
		else if (parameterTypeName == "real")
		{}
		else if (parameterTypeName == "matrix4")
		{}
		else if (parameterTypeName == "color")
		{}

	}
	vbox->addStretch(1);
	setLayout(vbox);
}
void ComponentView::ValueChanged(const std::string &value)
{
	std::cout << "Value Changed" << value <<  std::endl;
}