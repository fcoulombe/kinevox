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


#include "ResourceBrowser.h"

#include <iostream>
using namespace GCL;
ResourceBrowser::ResourceBrowser(QWidget *parent)
	: QWidget(parent)
{
	filemodel = new QFileSystemModel;
	dirview = new QTreeView;

	connect (dirview,SIGNAL(clicked(QModelIndex)),this, SLOT(OnClick(QModelIndex)));
	connect (dirview,SIGNAL(doubleClicked (QModelIndex)),this, SLOT(OnDoubleClick(QModelIndex)));
	
	dirview->setModel(filemodel);
	filemodel->setRootPath("data");

	dirview->setRootIndex(filemodel->index("data"));	
	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(dirview);
	this->setLayout(hbox);
}


void ResourceBrowser::OnClick(QModelIndex index)
{

	QString spath;
	spath = this->filemodel->fileInfo(index).absoluteFilePath();
	std::cout << "click: " << spath.toStdString() << std::endl;
}

void ResourceBrowser::OnDoubleClick(QModelIndex index)
{

	QString spath;
	spath = this->filemodel->fileInfo(index).absoluteFilePath();
	std::cout << "double click: " << spath.toStdString() << std::endl;
	QDesktopServices::openUrl(QUrl("file:///"+spath));
}


