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


#ifndef LXPANEL_RUNDIALOG_H
#define LXPANEL_RUNDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QTimer>
#include "ui_rundialog.h"
#include <menu-cache/menu-cache.h>
#include <gio/gio.h>

namespace Lxpanel {

class RunDialog : public QDialog {
  Q_OBJECT
public:
  explicit RunDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);
  virtual void accept();
  virtual ~RunDialog();

private Q_SLOTS:
  void onInputChanged();
  void onInputChangedTimeout();

private:
  void reloadApps();
  static void _reloadApps(MenuCache* mc, RunDialog* pThis) {
    pThis->reloadApps();
  }
  static gboolean _findProgramsThread(GIOSchedulerJob *job, GCancellable *cancellable, gpointer user_data);
  static gboolean _findProgramsThreadFinished(gpointer user_data);

  void addAppsToModel(GSList* apps);
  void addProgramsToModel(QStringList programs);

private:
  Ui::RunDialog ui;
  MenuCache* menu_cache;
  MenuCacheNotifyId reload_notify_id;
  GSList* app_list;
  GCancellable* cancellable_;
  QStandardItemModel* model_;
  QSortFilterProxyModel* proxyModel_;
  QTimer* inputTimer_;
};

}

#endif // LXPANEL_RUNDIALOG_H
