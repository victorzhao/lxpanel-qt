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


#include "appletplugininfo.h"
#include <QPluginLoader>
#include <glib.h>

using namespace Lxpanel;

AppletPluginInfo::AppletPluginInfo(QString id, QString moduleFile, QString desktopFile):
  AppletInfo(id),
  plugin_(NULL),
  moduleFile_(moduleFile) {
  loadInfo(desktopFile);
}

AppletPluginInfo::~AppletPluginInfo() {

}

Applet* AppletPluginInfo::create(QWidget* parent) {
  Applet* applet = NULL;
  if(!plugin_) { // if the plugin is not yet loaded
    QPluginLoader loader(moduleFile_);
    QObject* obj = loader.instance();
    plugin_ = qobject_cast<Lxpanel::AppletPlugin*>(obj);
    // qDebug("module: %s, plugin(%p): %p\n%s", qPrintable(moduleFile_), obj, plugin_, qPrintable(loader.errorString()));
  }
  if(plugin_)
    applet = plugin_->create(this, parent);
  return applet;
}

bool AppletPluginInfo::loadInfo(QString desktopFile) {
  // glib GKeyFile supports localized key values while QSettings does not.
  GKeyFile* kf = g_key_file_new();
  if(g_key_file_load_from_file(kf, desktopFile.toLocal8Bit().constData(), GKeyFileFlags(0), NULL)) {
    char* type = g_key_file_get_string(kf, "Desktop Entry", "Type", NULL);
    if(type) {
      if(strcmp(type, "Applet") == 0) {
        char* str = g_key_file_get_locale_string(kf, "Desktop Entry", "Name", NULL, NULL);
        if(str) {
          setName(QString::fromUtf8(str));
          g_free(str);
        }
        str = g_key_file_get_locale_string(kf, "Desktop Entry", "Comment", NULL, NULL);
        if(str) {
          setDescription(QString::fromUtf8(str));
          g_free(str);
        }
        str = g_key_file_get_locale_string(kf, "Desktop Entry", "Icon", NULL, NULL);
        if(str) {
          setIconName(QString::fromUtf8(str));
          g_free(str);
        }
      }
      g_free(type);
    }
  }
  g_key_file_free(kf);
  return true;
}
