/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2013  PCMan <email>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#include "launcherapplet.h"
#include <QFrame>

using namespace Lxpanel;

LauncherApplet::LauncherApplet(QWidget* parent): Applet() {
  frame_ = new QFrame(parent);
  layout_ = new QBoxLayout(QBoxLayout::LeftToRight, frame_);
  frame_->setLayout(layout_);
  QToolButton* button = new QToolButton();
  button->setText("app1");
  layout_->addWidget(button);

  button = new QToolButton();
  button->setText("app2");
  layout_->addWidget(button);
  
  button = new QToolButton();
  button->setText("app3");
  layout_->addWidget(button);
}

LauncherApplet::~LauncherApplet() {
  delete frame_;
}
