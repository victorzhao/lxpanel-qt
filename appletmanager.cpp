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
#include "applets/appmenu/appmenuapplet.h"
#include "applets/clock/clockapplet.h"
#include "applets/showdesktop/showdesktopapplet.h"
#include "applets/launcher/launcherapplet.h"
#include "applets/netstatus/netstatusapplet.h"
#include "applets/blank/blankapplet.h"
#include "applets/pager/pagerapplet.h"
#include "applets/task/taskapplet.h"
#include "applets/systray/systrayapplet.h"
#include "applets/volume/volumeapplet.h"

using namespace Lxpanel;

LXPANEL_DECLARE_BUILTIN_APPLET(AppMenuApplet, "appmenu", QObject::tr("Application menu"), "")
LXPANEL_DECLARE_BUILTIN_APPLET(ClockApplet, "clock", QObject::tr("Clock"), "")
LXPANEL_DECLARE_BUILTIN_APPLET(LauncherApplet, "launcher", QObject::tr("Application Launcher"), QObject::tr("Launch applications"))
LXPANEL_DECLARE_BUILTIN_APPLET(ShowDesktopApplet, "showdesktop", QObject::tr("Show desktop"), "")
LXPANEL_DECLARE_BUILTIN_APPLET(NetStatusApplet, "netstatus", QObject::tr("Network status monitor"), "")
LXPANEL_DECLARE_BUILTIN_APPLET(BlankApplet, "blank", QObject::tr("Blank space"), "")
LXPANEL_DECLARE_BUILTIN_APPLET(PagerApplet, "pager", QObject::tr("Pager"), QObject::tr("Switch among different desktops"))
LXPANEL_DECLARE_BUILTIN_APPLET(TaskApplet, "task", QObject::tr("Task list"), QObject::tr("Task buttons"))
LXPANEL_DECLARE_BUILTIN_APPLET(SysTrayApplet, "systray", QObject::tr("System tray (notification area)"), "")
LXPANEL_DECLARE_BUILTIN_APPLET(VolumeApplet, "volume", QObject::tr("Volume controller"), QObject::tr("Adjust volume"))

AppletManager::AppletManager() {

}

AppletManager::~AppletManager() {

}

void AppletManager::init() {
  // register built-in applets
  AppletFactory* factory;
  factory = new AppMenuAppletFactory();
  knownApplets_.insert(factory->id(), factory);

  factory = new ClockAppletFactory();
  knownApplets_.insert(factory->id(), factory);

  factory = new ShowDesktopAppletFactory();
  knownApplets_.insert(factory->id(), factory);

  factory = new LauncherAppletFactory();
  knownApplets_.insert(factory->id(), factory);

  factory = new NetStatusAppletFactory();
  knownApplets_.insert(factory->id(), factory);
  
  factory = new BlankAppletFactory();
  knownApplets_.insert(factory->id(), factory);

  factory = new PagerAppletFactory();
  knownApplets_.insert(factory->id(), factory);

  factory = new TaskAppletFactory();
  knownApplets_.insert(factory->id(), factory);
  
  factory = new SysTrayAppletFactory();
  knownApplets_.insert(factory->id(), factory);
  
  factory = new VolumeAppletFactory();
  knownApplets_.insert(factory->id(), factory);

  // Find dynamically loaded applets
  // By default, load the deaktop entry files from /usr/share/lxpanel-qt/applets
  QDir dir;
  dir.cd(LXPANEL_DATA_DIR "/applets");
  qDebug("loading: %s", LXPANEL_DATA_DIR "/applets");
  QStringList files = dir.entryList();
  Q_FOREACH(QString file, files) {
    if(file[0] != '.' && file.endsWith(".desktop")) {
      qDebug("find applet: %s", qPrintable(file));
      QString id = file.left(file.length() - 8);
      QString soPath = LXPANEL_LIB_DIR "/applets/" + id + ".so";
      AppletPluginFactory* factory = new AppletPluginFactory(id, soPath, dir.absoluteFilePath(file));
      knownApplets_.insert(factory->id(), factory);
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
