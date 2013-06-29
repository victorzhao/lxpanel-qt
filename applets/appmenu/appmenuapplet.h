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


#ifndef LXPANEL_APPMENUAPPLET_H
#define LXPANEL_APPMENUAPPLET_H

#include <QPushButton>
#include <menu-cache/menu-cache.h>

#include "../../applet.h"
#include "../../appletfactory.h"
#include "../../appletplugin.h"

class QMenu;

namespace Lxpanel {

class AppMenuApplet: public Applet {
  Q_OBJECT

public:
  explicit AppMenuApplet(QWidget* parent = 0);
  virtual ~AppMenuApplet();

  QWidget* widget() {
    return button_;
  }

private:

  void addMenuItems(QMenu* menu, MenuCacheDir* dir);
  void reloadMenu();
  static void reloadNotify(MenuCache* mc, AppMenuApplet* pThis) {
    pThis->reloadMenu();
  }

private Q_SLOTS:
  void onItemTrigerred();
  void onItemHovered(QAction * action);

private:
  QPushButton* button_;
  QMenu* menu_;
  MenuCache* menu_cache;
  MenuCacheNotifyId menu_cache_reload_notify;
};

}

#endif // LXPANEL_APPMENUAPPLET_H
