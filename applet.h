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


#ifndef LXPANEL_APPLET_H
#define LXPANEL_APPLET_H

#include <QWidget>
#include <QDomElement>

class QMenu;

namespace Lxpanel {

class AppletInfo;
  
class Applet : public QObject {
  Q_OBJECT

public:
  explicit Applet(AppletInfo* info, QWidget* parent = 0);
  virtual ~Applet();

  virtual QWidget* widget() = 0;

  virtual void setPanelIconSize(int size) {
  }

  virtual void setPanelOrientation(Qt::Orientation orientation) {
  }

  virtual bool expand() {
    return expand_;
  }

  virtual void setExpand(bool expand) {
    expand_ = expand;
  }

  virtual bool loadSettings(QDomElement& element) {
    return true;
  }

  virtual bool saveSettings(QDomElement& element) {
    return true;
  }

  AppletInfo* info() {
    return info_;
  }

  virtual void customizeContextMenu(QMenu* menu);

public Q_SLOTS:
  virtual void preferences();

private:
  bool expand_;
  AppletInfo* info_;
};

}

#endif // LXPANEL_APPLET_H
