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


#ifndef LXPANEL_APPLETPLUGINFACTORY_H
#define LXPANEL_APPLETPLUGINFACTORY_H

#include <QtPlugin>
#include "appletfactory.h"
#include "appletplugin.h"

namespace Lxpanel {

class AppletPlugin;

// factory class used to load dynamic modules on demand and generate new applet instances
class AppletPluginFactory : public AppletFactory {

public:
  AppletPluginFactory(QString filename);
  virtual ~AppletPluginFactory();
  virtual Applet* create(QWidget* parent);

private:
  QString moduleFile_;
  AppletPlugin* plugin_;
};

}

#endif // LXPANEL_APPLETPLUGINFACTORY_H
