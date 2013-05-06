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


#ifndef LXPANEL_APPLETPLUGIN_H
#define LXPANEL_APPLETPLUGIN_H

#include <QtPlugin>
#include "applet.h"

namespace Lxpanel {

// abstract class which should be implemented by every dynamic applet plugins
// p.s: built-in applets do not need to implement this.
class AppletPlugin {
public:

  virtual ~AppletPlugin();
  virtual Applet* create(QWidget* parent) = 0;
};

};

Q_DECLARE_INTERFACE(Lxpanel::AppletPlugin, "org.lxde.LXPanel.AppletPlugin/1.0");


#endif // LXPANEL_APPLETPLUGIN_H
