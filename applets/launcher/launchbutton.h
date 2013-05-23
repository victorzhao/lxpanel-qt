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


#ifndef LXPANEL_LAUNCHBUTTON_H
#define LXPANEL_LAUNCHBUTTON_H

#include <QToolButton>
#include <gio/gio.h>

namespace Lxpanel {

class LaunchButton: public QToolButton {
  Q_OBJECT
public:
  enum ButtonType {
    App,
    Custom,
    Command
  };

  LaunchButton(ButtonType type, QWidget* parent = 0);
  virtual ~LaunchButton();

  ButtonType type() const {
    return type_;
  }

  void setType(ButtonType type);

  QString desktopId() const {
    return desktopId_;
  }

  void setDesktopId(QString desktop_id);

  void setCommandExec(QString exec) {
    commandExec_ = exec;
  }

  QString commandExec() const {
    return commandExec_;
  }

  void setTerminal(bool terminal) {
    terminal_ = terminal;
  }

  bool terminal() const {
    return terminal_;
  }

private Q_SLOTS:
  void onClicked();

private:
  QString desktopId_;
  QString commandExec_;
  bool terminal_;
  GAppInfo* appInfo_;
  ButtonType type_;
};

}

#endif // LXPANEL_LAUNCHBUTTON_H
