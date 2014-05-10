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


#include "FileBrowser.h"

#include <iostream>
using namespace GCL;
FileBrowser::FileBrowser(QWidget *parent)
	: QWidget(parent)
{
	QHBoxLayout *hbox = new QHBoxLayout;
	lineEdit = new QLineEdit();
	lineEdit->setMinimumWidth(50);
	lineEdit->setMaximumWidth(500);
	lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	hbox->addWidget(lineEdit, 1, Qt::AlignLeft);
	QPushButton *browserBtn = new QPushButton(tr("..."));
	
	browserBtn->setMinimumWidth(20);
	browserBtn->setMinimumHeight(20);
	browserBtn->setMaximumWidth(20);
	browserBtn->setMaximumHeight(20);
	connect(browserBtn, SIGNAL(released()), this, SLOT(BrowseFile()));
	hbox->addWidget(browserBtn, 1, Qt::AlignRight);
	this->setLayout(hbox);

}


void FileBrowser::BrowseFile()
{
	QString filename = QFileDialog::getOpenFileName(this);
	 lineEdit->setText(filename);
	 emit EmitLineEditChange(filename.toStdString());
}
