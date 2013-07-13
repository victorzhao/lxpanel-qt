/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2013  <copyright holder> <email>

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


#include "showdesktopapplet.h"
#include <QToolButton>
#include "../../xfitman.h"
#include "../../fixx11h.h"
#include <QX11Info>

using namespace Lxpanel;

ShowDesktopApplet::ShowDesktopApplet(AppletFactory* factory, QWidget* parent):
  Applet(factory, parent) {

  button_ = new QToolButton(parent);
  button_->setToolTip(tr("Show desktop"));
  button_->setText(tr("Show desktop"));
  button_->setIcon(QIcon::fromTheme("user-desktop"));
  //button_->setIconSize(QSize(32, 32));
  button_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  button_->setAutoRaise(true);
  
  connect(button_, SIGNAL(clicked(bool)), SLOT(onButtonClicked()));
}

ShowDesktopApplet::~ShowDesktopApplet() {
}

void ShowDesktopApplet::onButtonClicked() {
  bool show = xfitMan().getShowingDesktop(); // get current status
  XfitMan().setShowingDesktop(!show);
}

