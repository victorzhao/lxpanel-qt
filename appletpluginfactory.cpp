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


#include "appletpluginfactory.h"
#include <QPluginLoader>

using namespace Lxpanel;

AppletPluginFactory::AppletPluginFactory(QString filename):
  plugin_(NULL),
  moduleFile_(filename) {
  
}

AppletPluginFactory::~AppletPluginFactory() {

}

Applet* AppletPluginFactory::create(QWidget* parent) {
  Applet* applet = NULL;
  if(!plugin_) { // if the plugin is not yet loaded
    QPluginLoader loader(moduleFile_);
    QObject* obj = loader.instance();
    plugin_ = qobject_cast<Lxpanel::AppletPlugin*>(obj);
    // qDebug("module: %s, plugin(%p): %p\n%s", qPrintable(moduleFile_), obj, plugin_, qPrintable(loader.errorString()));
  }
  if(plugin_)
    applet = plugin_->create(parent);
  return applet;
}
