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


#include "application.h"
#include <QDesktopWidget>

#include <glib.h>
#include <gio/gio.h>
#include <glib-unix.h>
#include <signal.h>

#include <QDomDocument>
#include <QFile>
#include <QIcon>
#include <QLocale>
#include <QLibraryInfo>
#include <QDBusConnection>
#include "applicationadaptor.h"

#include "panel.h"
#include "applet.h"

using namespace Lxpanel;

static const char* serviceName = "org.lxde.LxPanel";
static const char* ifaceName = "org.lxde.LxPanel.Application";

Application::Application(int& argc, char** argv, int flags):
  QApplication(argc, argv),
  desktopSettings_(),
  iconTheme_(),
  libfmQt_(),
  appletManager_(),
  profile_("default") {

  QDesktopWidget* desktopWidget = QApplication::desktop();
  connect(desktopWidget, SIGNAL(resized(int)), SLOT(onScreenResized(int)));
  connect(desktopWidget, SIGNAL(screenCountChanged(int)), SLOT(onScreenCountChanged(int)));
}

Application::~Application() {
  saveSettings();
  Q_FOREACH(Panel* panel, panels_) {
    delete panel;
  }

  // remove UNIX signal handlers
  g_source_remove_by_user_data(this);
}

bool Application::handleCommandLineArgs() {
  if(isPrimaryInstance) {
    
  }
  else {
    exit(0);
  }
  return true;
}

gboolean Application::onUnixSignal(Application* pThis) {
  // quit the applications cleanly when receiving SIGTERM & SIGINT
  // FIXME: currently there is a bug in libfm causing a segfault on termination.
  pThis->quit();
  return FALSE;
}

void Application::init() {

  // install UNIX signal handlers
  g_unix_signal_add(SIGTERM, GSourceFunc(onUnixSignal), this);
  g_unix_signal_add(SIGINT, GSourceFunc(onUnixSignal), this);

  // install the translations built-into Qt itself
  qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
  installTranslator(&qtTranslator);

  // install libfm-qt translator
  installTranslator(libfmQt_.translator());

  // install our own tranlations
  translator.load("lxpanel-qt_" + QLocale::system().name(), LXPANEL_DATA_DIR "/translations");
  installTranslator(&translator);

  // initialize dbus
  QDBusConnection dbus = QDBusConnection::sessionBus();
  if(dbus.registerService(serviceName)) {
    // we successfully registered the service
    isPrimaryInstance = true;
    new ApplicationAdaptor(this);
    dbus.registerObject("/Application", this);
    // connect(this, SIGNAL(aboutToQuit()), SLOT(onAboutToQuit()));
  }
  else {
    // an service of the same name is already registered.
    // we're not the first instance
    isPrimaryInstance = false;
  }

  handleCommandLineArgs();
  appletManager_.init();

  iconTheme_ = desktopSettings_.iconThemeName();
  qDebug("icon %s", qPrintable(iconTheme_));
  loadSettings();
}

bool Application::loadConfigFile(QString path) {
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly))
      return false;

  QDomDocument doc("");
  if (!doc.setContent(&file)) {
      file.close();
      return false;
  }
  file.close();

  QIcon::setThemeName(iconTheme_);

  // the XML document is loaded
  QDomElement rootElement = doc.documentElement();
  for(QDomNode node = rootElement.firstChild(); !node.isNull(); node = node.nextSibling()) {
    QDomElement element = node.toElement(); // convert the node to an element.
    if(!element.isNull()) {
      if(element.tagName() == "global") {
        // load global settings
        for(QDomNode childNode = element.firstChild(); !childNode.isNull(); childNode = childNode.nextSibling()) {
          QDomElement childElement = childNode.toElement();
          if(!childElement.isNull()) {
            // qDebug("%s=%s", qPrintable(childElement.tagName()), qPrintable(childElement.text()));
            if(childElement.tagName() == "file_manager")
              fileManager_ = childElement.text();
            else if(childElement.tagName() == "logout_command")
              logoutCommand_ = childElement.text();
            else if(childElement.tagName() == "terminal_command")
              terminalCommand_ = childElement.text();
            //else if(childElement.tagName() == "icon_theme") {
            //  iconTheme_ = childElement.text();
            //  QIcon::setThemeName(iconTheme_);
            //}
            else if(childElement.tagName() == "theme") {
              // themeName_ = childElement.text();
            }
          }
        }
      }
      else if(element.tagName() == "panel") {
        // load panel
        Panel *panel = new Panel();
        if(panel->load(element)) {
          addPanel(panel);
	  qDebug("panel loaded");
        }
        else {
          delete panel;
        }
      }
    }
  }

  Q_FOREACH(Panel* panel, panels_) {
    panel->recalculateGeometry();
    panel->show();
  }
  
  /*
  // relayout all panels
  var display = Gdk.Display.get_default();
  int n_screens = display.get_n_screens();

  for(int i = 0; i < n_screens; ++i) {
    on_monitors_changed(display.get_screen(i));
  }

  // show them after repositioning.
  foreach(unowned Panel panel in all_panels) {
    panel.show();
  }
  */
  return true;
}

bool Application::loadSettings() {
  bool loaded = false;

  QIcon::setThemeName("elementary");

  // try to load user-specific config file first
  char* fpath = g_build_filename(g_get_user_config_dir(), "lxpanel-qt", qPrintable(profile_), "config.xml", NULL);
  if(loadConfigFile(fpath)) {
    loaded = true;
  }
  else {
    g_free(fpath);
    // failed, try to load system-wide config file
    const char* const * dirs = g_get_system_config_dirs();
    for(const char* const* dir = dirs; *dir; ++dir) {
      fpath = g_build_filename(*dir, "lxpanel-qt", qPrintable(profile_), "config.xml", NULL);
      if(loadConfigFile(fpath)) {
        loaded = true;
        break;
      }
    }
  }
  g_free(fpath);
  return loaded;
}

bool Application::saveSettings() {
  return true;
}


void Application::addPanel(Panel* panel) {
  panels_.append(panel);
}

void Application::removePanel(Panel* panel) {
  int i = panels_.indexOf(panel);
  if(i != -1)
    panels_.remove(i);
}

void Application::onScreenCountChanged(int newCount) {

}

void Application::onScreenResized(int screen) {
  // relayout panels as needed
  Q_FOREACH(Panel* panel, panels_) {
    if(panel->monitor() == screen)
      panel->recalculateGeometry();
  }
}

bool Application::x11EventFilter(XEvent* event) {
  if(desktopSettings_.x11EventFilter(event))
    return true;
  Q_FOREACH(XEventFilter* filter, xeventFilters_) {
    if(filter->x11EventFilter(event))
      return true;
  }
  return QApplication::x11EventFilter(event);
}


void Application::run() {
  qDebug("run");
}


#if 0
bool Application::save_all_panels(string profile_name) {
  var doc = new GMarkupDom.Doc();
  doc.root = new GMarkupDom.Node(null, "lxpanel", null, null);
  unowned GMarkupDom.Node global_node = doc.root.new_child("global", null);

  // save global config
  if(file_manager != null)
    global_node.new_child("file_manager", file_manager);

  if(logout_command != null)
    global_node.new_child("logout_command", logout_command);

  if(theme_name != null)
    global_node.new_child("theme", theme_name);

  // save panels
  foreach(weak Panel panel in all_panels) {
    unowned GMarkupDom.Node node;
    node = doc.root.new_child("panel", null, {"id"}, {panel.id});
    panel.save_panel(node);
  }

  // write the xml doc to a user-specific config file
  var dirpath = Path.build_filename(Environment.get_user_config_dir(), "lxpanel-qt", profile_name, null);

  if(GLib.DirUtils.create_with_parents(dirpath, 0700) == 0) {
    var fpath = Path.build_filename(dirpath, "config.xml", null);
    return doc.save(fpath);
  }

  return false;
}
#endif
