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


#ifndef LXPANEL_APPLICATION_H
#define LXPANEL_APPLICATION_H

#include <QApplication>
#include <QVector>
#include "appletmanager.h"
#include "desktopsettings.h"

#include <libfm-qt/libfmqt.h>

namespace Lxpanel {
  
class Panel;
class AppletFactory;

class Application : public QApplication {
  Q_OBJECT

public:
  Application(int& argc, char** argv, int  flags = ApplicationFlags);
  virtual ~Application();

  void init();
  
  // configuration profile
  QString profile() {
    return profile_;
  }

  // command used to launch file manager
  QString fileManager() {
    return fileManager_;
  }

  // command used to logout desktop session
  QString logoutCommand() {
    return logoutCommand_;
  }

  // command used to lauch a terminal emulator
  QString terminalCommand() {
    return terminalCommand_;
  }

  const QVector<Panel*>& panels() const {
    return panels_;
  }

  AppletManager& appletManager() {
    return appletManager_;
  }

protected:
  bool handleCommandLineArgs();
  bool loadSettings();
  bool saveSettings();
  bool loadConfigFile(QString path);
  bool loadPanels();

  void addPanel(Panel* panel);
  void removePanel(Panel* panel);
  
  virtual bool x11EventFilter(XEvent* event);

private Q_SLOTS:
  void onScreenResized(int screen);
  void onScreenCountChanged(int newCount);

private:
  QString profile_;
  QString fileManager_; // command used to launch file manager
  QString logoutCommand_; // command used to logout desktop session
  QString terminalCommand_; // command used to lauch a terminal emulator
  QString iconTheme_;

  Xdg::DesktopSettings desktopSettings_;
  AppletManager appletManager_;
  QVector<Panel*> panels_; // all desktop panels
  Fm::LibFmQt libfmQt_;
};

}

#endif // LXPANEL_APPLICATION_H
