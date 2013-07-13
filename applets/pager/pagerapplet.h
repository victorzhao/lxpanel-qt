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


#ifndef LXPANEL_PAGERAPPLET_H
#define LXPANEL_PAGERAPPLET_H

#include "../../applet.h"
#include "../../application.h"

class QFrame;
class QBoxLayout;
class QButtonGroup;

namespace Lxpanel {

class PagerApplet : public Applet, XEventFilter {
  Q_OBJECT
public:
  PagerApplet(AppletFactory* factory, QWidget* parent = 0);

  virtual QWidget* widget() {
    return frame_;
  }
  virtual ~PagerApplet();
  
  virtual bool expand() {
    return false;
  }

  virtual bool x11EventFilter(XEvent* event);

private:
  void reloadButtons();

private Q_SLOTS:
  void onButtonClicked(int id);
  
private:
  QFrame* frame_;
  QButtonGroup* group_;
  QBoxLayout* layout_;
  int currentDesktop_;
  int desktopCount_;
};

};

#endif // LXPANEL_PAGERAPPLET_H
