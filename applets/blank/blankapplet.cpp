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


#include "blankapplet.h"

using namespace Lxpanel;

BlankApplet::BlankApplet(AppletInfo* info, QWidget* parent):
  Applet(info, parent),
  size_(4),
  widget_(new QWidget(parent)) {
}

BlankApplet::~BlankApplet() {
  delete widget_;
}

bool BlankApplet::loadSettings(QDomElement& element) {
  Lxpanel::Applet::loadSettings(element);
  for(QDomNode node = element.firstChild(); !node.isNull(); node = node.nextSibling()) {
    QDomElement childElement = node.toElement();
    if(childElement.isNull())
      continue;
    QString tagName = childElement.tagName();
    QString value = childElement.text();
    if(tagName == "size") {
      size_ = value.toInt();
    }
    else if(tagName == "expand") {
      setExpand((value == "true"));
    }

    if(expand())
      widget_->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    else
      widget_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    widget_->setMinimumWidth(size_);
  }
  return true;
}

bool BlankApplet::saveSettings(QDomElement& element) {
  Lxpanel::Applet::saveSettings(element);
  // TODO: save config here
  return true;
}

void BlankApplet::preferences() {
  Lxpanel::Applet::preferences();
}
