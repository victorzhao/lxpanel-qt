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

using namespace Lxpanel;

unsigned long PagerApplet::rootProps_[2] = {NET::NumberOfDesktops|NET::CurrentDesktop|NET::DesktopNames, 0};

PagerApplet::PagerApplet(QWidget* parent):
  Applet(parent),
  rootInfo_(QX11Info::display(), rootProps_, 2),
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
  delete frame_;
  delete group_;
  delete layout_;

  Application* app = static_cast<Application*>(qApp);
  app->removeXEventFilter(this);
}

void PagerApplet::reloadButtons() {
  // FIXME: remove all existing buttons
  Q_FOREACH(QAbstractButton* oldBtn, group_->buttons()) {
    delete oldBtn;
  }
  desktopCount_ = rootInfo_.numberOfDesktops();
  currentDesktop_ = rootInfo_.currentDesktop();
  QString title("%1");
  for(int i = 1; i <= desktopCount_; ++i) {
    QToolButton* btn = new QToolButton(frame_);
    btn->setCheckable(true);
    btn->setText(title.arg(i));
    const char* name = rootInfo_.desktopName(i);
    btn->setToolTip(QString::fromUtf8(name));
    layout_->addWidget(btn);
    group_->addButton(btn, i);
    if(i == currentDesktop_)
      btn->setChecked(true);
  }
}

void PagerApplet::onButtonClicked(int id) {
  if(id != currentDesktop_) {
    unsigned long props[] = {NET::CurrentDesktop, 0};
    currentDesktop_ = id;
    rootInfo_.setCurrentDesktop(currentDesktop_);
  }
}

bool PagerApplet::x11EventFilter(XEvent* event) {
  unsigned long ret = rootInfo_.event(event);
  if(ret & NET::NumberOfDesktops) {
    reloadButtons();
  }
  if(ret & NET::CurrentDesktop) {
    currentDesktop_ = rootInfo_.currentDesktop();
    QAbstractButton* btn = group_->button(currentDesktop_);
    if(btn)
      btn->setChecked(true);
  }
  return false;
}
