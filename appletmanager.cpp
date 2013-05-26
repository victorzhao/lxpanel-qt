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


#include "appletmanager.h"
#include <QDir>

// built-in applets
#include "appletpluginfactory.h"
#include "applets/clock/clockapplet.h"
#include "applets/showdesktop/showdesktopapplet.h"
#include "applets/launcher/launcherapplet.h"
#include "applets/netstatus/netstatusapplet.h"
#include "applets/blank/blankapplet.h"
#include "applets/pager/pagerapplet.h"
#include "applets/task/taskapplet.h"
#include "applets/systray/systrayapplet.h"

using namespace Lxpanel;

LXPANEL_DECLARE_BUILTIN_APPLET(ClockApplet)
LXPANEL_DECLARE_BUILTIN_APPLET(LauncherApplet)
LXPANEL_DECLARE_BUILTIN_APPLET(ShowDesktopApplet)
LXPANEL_DECLARE_BUILTIN_APPLET(NetStatusApplet)
LXPANEL_DECLARE_BUILTIN_APPLET(BlankApplet)
LXPANEL_DECLARE_BUILTIN_APPLET(PagerApplet)
LXPANEL_DECLARE_BUILTIN_APPLET(TaskApplet)
LXPANEL_DECLARE_BUILTIN_APPLET(SysTrayApplet)

AppletManager::AppletManager() {

}

AppletManager::~AppletManager() {

}

void AppletManager::init() {
  // register built-in applets
  knownApplets_.insert("clock", new ClockAppletFactory());
  knownApplets_.insert("showdesktop", new ShowDesktopAppletFactory());
  knownApplets_.insert("launcher", new LauncherAppletFactory());
  knownApplets_.insert("netstatus", new NetStatusAppletFactory());
  knownApplets_.insert("blank", new BlankAppletFactory());
  knownApplets_.insert("pager", new PagerAppletFactory());
  knownApplets_.insert("task", new TaskAppletFactory());
  knownApplets_.insert("systray", new SysTrayAppletFactory());

  // find dynamic applets modules from module dirs
  QDir dir;
  dir.cd(QString(LXPANEL_LIB_DIR) + "/applets");
  QStringList files = dir.entryList();
  Q_FOREACH(QString file, files) {
    if(file[0] != '.' && file.endsWith(".so")) {
      QString name = file.left(file.length() - 3);
      AppletFactory* factory = new AppletPluginFactory(dir.absoluteFilePath(file));
      knownApplets_.insert(name, factory);
    }
  }
}

Applet* AppletManager::createApplet(QString typeName) {
  AppletFactory* appletFactory = knownApplets_.value(typeName, NULL);
  if(appletFactory)
    return appletFactory->create(NULL);
  return NULL;
}

void AppletManager::destroyApplet(Applet* applet) {
  delete applet;
}
