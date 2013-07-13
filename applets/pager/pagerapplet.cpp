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


#include "pagerapplet.h"
#include <QToolButton>
#include <QFrame>
#include <QBoxLayout>
#include <QButtonGroup>

#include <QX11Info>
#include "../../xfitman.h"
#include "../../fixx11h.h"

using namespace Lxpanel;

PagerApplet::PagerApplet(AppletInfo* info, QWidget* parent):
  Applet(info, parent),
  desktopCount_(0),
  currentDesktop_(0),
  layout_(new QBoxLayout(QBoxLayout::LeftToRight)),
  group_(new QButtonGroup()),
  frame_(new QFrame()) {

  Application* app = static_cast<Application*>(qApp);
  app->addXEventFilter(this);

  frame_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  layout_->setMargin(0);
  layout_->setContentsMargins(0, 0, 0, 0);
  layout_->setSpacing(0);
  layout_->setSizeConstraint(QLayout::SetFixedSize);
  frame_->setLayout(layout_);

  group_->setExclusive(true);
  connect(group_, SIGNAL(buttonClicked(int)), SLOT(onButtonClicked(int)));
  reloadButtons();
}

PagerApplet::~PagerApplet() {
  Application* app = static_cast<Application*>(qApp);
  app->removeXEventFilter(this);

  delete layout_;
  delete group_;
  delete frame_;
}

void PagerApplet::reloadButtons() {
  // remove all existing buttons
  Q_FOREACH(QAbstractButton* oldBtn, group_->buttons()) {
    delete oldBtn;
  }
  XfitMan xfitman;
  desktopCount_ = xfitman.getNumDesktop();
  currentDesktop_ = xfitman.getActiveDesktop();
  QString title("%1");
  for(int i = 0; i < desktopCount_; ++i) {
    QToolButton* btn = new QToolButton(frame_);
    btn->setCheckable(true);
    btn->setText(title.arg(i + 1));
    QString name = xfitman.getDesktopName(i);
    btn->setToolTip(name);
    layout_->addWidget(btn);
    group_->addButton(btn, i);
    if(i == currentDesktop_)
      btn->setChecked(true);
  }
}

void PagerApplet::onButtonClicked(int id) {
  if(id != currentDesktop_) {
    XfitMan xfitman;
    currentDesktop_ = id;
    xfitman.setActiveDesktop(currentDesktop_);
  }
}

bool PagerApplet::x11EventFilter(XEvent* event) {
  XfitMan xfitman;
  if(event->type == PropertyNotify) {
    if(event->xany.window == QX11Info::appRootWindow()) {
      if(event->xproperty.atom == xfitman.atom("_NET_NUMBER_OF_DESKTOPS")) {
        reloadButtons();
      }
      else if(event->xproperty.atom == xfitman.atom("_NET_DESKTOP_NAMES")) {
        reloadButtons(); // FIXME: only update desktop names
      }
      else if(event->xproperty.atom == xfitman.atom("_NET_CURRENT_DESKTOP")) {
        currentDesktop_ = xfitman.getActiveDesktop();
        QAbstractButton* btn = group_->button(currentDesktop_);
        if(btn)
          btn->setChecked(true);
      }
    }
  }
  return false;
}
