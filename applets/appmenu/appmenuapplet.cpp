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

#include "appmenuapplet.h"
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QCursor>
#include <QToolTip>
#include <gio/gdesktopappinfo.h>

using namespace Lxpanel;

class AppMenuAction: public QAction {
public:
  explicit AppMenuAction(MenuCacheItem* item, QObject* parent = 0):
    item_(item),
    QAction(parent) {

    QString title = QString::fromUtf8(menu_cache_item_get_name(item));
    setText(title);

    QString comment = QString::fromUtf8(menu_cache_item_get_comment(item));
    setToolTip(comment);

    QIcon icon = QIcon::fromTheme(menu_cache_item_get_icon(item));
    setIcon(icon);
  }
  virtual ~AppMenuAction() {
  }

  MenuCacheItem* item() const {
    return item_;
  }

private:
  MenuCacheItem* item_;
};

AppMenuApplet::AppMenuApplet(QWidget* parent):
  Applet(parent),
  button_(new QPushButton()),
  menu_(NULL) {

  button_->setText("Start");
  button_->setIcon(QIcon::fromTheme("start-here"));
  button_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  menu_cache = menu_cache_lookup("applications.menu");
  if(menu_cache) {
    menu_cache_reload_notify = menu_cache_add_reload_notify(menu_cache, (MenuCacheReloadNotify)reloadNotify, this);
  }
}

AppMenuApplet::~AppMenuApplet() {
  delete button_;
  if(menu_)
    delete menu_;

  if(menu_cache) {
    menu_cache_remove_reload_notify(menu_cache, menu_cache_reload_notify);
    menu_cache_unref(menu_cache);
  }
}

void AppMenuApplet::addMenuItems(QMenu* menu, MenuCacheDir* dir) {
  for(GSList* l = menu_cache_dir_get_children(dir); l; l = l->next) {
    MenuCacheItem* item = (MenuCacheItem*)l->data;
    MenuCacheType type = menu_cache_item_get_type(item);

    if(type == MENU_CACHE_TYPE_SEP) {
      menu->addSeparator();
      continue;
    }
    else {
      AppMenuAction* action = new AppMenuAction(item, menu);
      menu->addAction(action);
      if(type == MENU_CACHE_TYPE_APP) {
        connect(action, SIGNAL(triggered(bool)), SLOT(onItemTrigerred()));
      }
      else if(type == MENU_CACHE_TYPE_DIR) {
        QMenu* submenu = new QMenu(menu);
        connect(submenu, SIGNAL(hovered(QAction*)), SLOT(onItemHovered(QAction*)));
        action->setMenu(submenu);
        addMenuItems(submenu, (MenuCacheDir*)item);
      }
    }
  }
}

void AppMenuApplet::reloadMenu() {
  if(menu_) {
    // TODO: remove old menu items
    delete menu_;
  }
  menu_ = new QMenu();
  connect(menu_, SIGNAL(hovered(QAction*)), SLOT(onItemHovered(QAction*)));
  button_->setMenu(menu_);

  // reload menu items
  MenuCacheDir* dir = menu_cache_get_root_dir(menu_cache);
  addMenuItems(menu_, dir);
  
  // FIXME: add "Run" and "Shutdown" menu items.
}

void AppMenuApplet::onItemTrigerred() {
  AppMenuAction* action = static_cast<AppMenuAction*>(sender());
  // MenuCacheApp* app = menu_item.get_data("MenuCacheItem");
  char* desktop_file = menu_cache_item_get_file_path(action->item());
  // use GDesktopAppInfo here
  GAppInfo* appinfo = (GAppInfo*)g_desktop_app_info_new_from_filename(desktop_file);
  g_free(desktop_file);
  g_app_info_launch(appinfo, NULL, NULL, NULL);
}

// Qt does not show tooltips for menu items natively
// Let's do it manually here
void AppMenuApplet::onItemHovered(QAction* action) {
  QString tooltip = action->toolTip();
  if(!tooltip.isEmpty()) {
    QMenu* menu = static_cast<QMenu*>(sender());
    QToolTip::showText(QCursor::pos(), tooltip, menu);
  }
}

