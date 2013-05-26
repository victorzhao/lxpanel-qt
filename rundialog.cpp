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


#include "rundialog.h"
#include <QtDebug>

using namespace Lxpanel;

struct ThreadData {
  GCancellable* cancellable;
  QStringList files;
  RunDialog* dialog;
  
  ~ThreadData() {
    if(cancellable)
      g_object_unref(cancellable);
  }
  
  static void free(ThreadData* data) {
    delete data;
  }
};

RunDialog::RunDialog(QWidget* parent, Qt::WindowFlags f):
  QDialog(parent, f),
  app_list(NULL),
  cancellable_(g_cancellable_new()),
  inputTimer_(NULL),
  model_(new QStandardItemModel()),
  proxyModel_(new QSortFilterProxyModel()),
  reload_notify_id(0) {

  ui.setupUi(this);
  
  connect(ui.command, SIGNAL(textChanged(QString)), SLOT(onInputChanged()));

  proxyModel_->setSortCaseSensitivity(Qt::CaseInsensitive);
  proxyModel_->setFilterCaseSensitivity(Qt::CaseInsensitive);
  proxyModel_->setFilterKeyColumn(0);
  proxyModel_->setSortRole(Qt::DisplayRole);
  proxyModel_->setDynamicSortFilter(true);
  proxyModel_->sort(0);
  ui.treeView->setModel(proxyModel_);

  // load installed applications from menu
  menu_cache = menu_cache_lookup("applications.menu");

  if(menu_cache) {
    app_list = (GSList*)menu_cache_list_all_apps(menu_cache);
    if(app_list)
      addAppsToModel(app_list);
    reload_notify_id = menu_cache_add_reload_notify(menu_cache, MenuCacheReloadNotify(_reloadApps), NULL);
  }

  // discover commands from $PATH
  ThreadData* data = new ThreadData();
  data->cancellable = G_CANCELLABLE(g_object_ref(cancellable_));
  data->dialog = this;
  g_io_scheduler_push_job(GIOSchedulerJobFunc(_findProgramsThread), data,
                          GDestroyNotify(ThreadData::free),
                          G_PRIORITY_DEFAULT, cancellable_);
}

void RunDialog::accept() {
  QDialog::accept();
}

RunDialog::~RunDialog() {
  delete proxyModel_;
  delete model_;

  if(cancellable_) {
    g_object_unref(cancellable_);
  }

  if(app_list) {
    g_slist_foreach(app_list, (GFunc)menu_cache_item_unref, NULL);
    g_slist_free(app_list);
  }

  if(menu_cache) {
    menu_cache_remove_reload_notify(menu_cache, reload_notify_id);
    menu_cache_unref(menu_cache);
  }
}

void RunDialog::reloadApps() {
  g_debug("reload apps!");

  if(app_list) {
    g_slist_foreach(app_list, (GFunc)menu_cache_item_unref, NULL);
    g_slist_free(app_list);
  }

  app_list = (GSList*)menu_cache_list_all_apps(menu_cache);
}

// called from worker thread
gboolean RunDialog::_findProgramsThread(GIOSchedulerJob* job, GCancellable* cancellable, gpointer user_data) {
  ThreadData* data = reinterpret_cast<ThreadData*>(user_data);
  gchar** dirname;
  gchar** dirnames = g_strsplit(g_getenv("PATH"), ":", 0);
  for(dirname = dirnames; !g_cancellable_is_cancelled(cancellable) && *dirname; ++dirname) {
    GDir* dir = g_dir_open(*dirname, 0, NULL);
    if(!dir)
      continue;
    const char* name;
    while(!g_cancellable_is_cancelled(cancellable) && (name = g_dir_read_name(dir))) {
      char* filename = g_build_filename(*dirname, name, NULL);
      if(g_file_test(filename, G_FILE_TEST_IS_EXECUTABLE)) {
        if(g_cancellable_is_cancelled(cancellable))
          break;
        QString nameStr = QString::fromLocal8Bit(name);
        if(data->files.indexOf(nameStr) == -1)
          data->files.append(nameStr);
      }
      g_free(filename);
    }
    g_dir_close(dir);
  }
  g_strfreev(dirnames);

  // finding binary programs in $PATH is finished
  if(!g_cancellable_is_cancelled(cancellable)) {
    g_io_scheduler_job_send_to_mainloop(job, GSourceFunc(_findProgramsThreadFinished), data, NULL);
  }
  return FALSE;
}

// called from main GUI thread
gboolean RunDialog::_findProgramsThreadFinished(gpointer user_data) {
  ThreadData* data = reinterpret_cast<ThreadData*>(user_data);
  RunDialog* pThis = data->dialog;
  if(!g_cancellable_is_cancelled(data->cancellable)) {
    pThis->addProgramsToModel(data->files);
  }
  return FALSE;
}


void RunDialog::addAppsToModel(GSList* apps) {
  for(GSList* l = apps; l; l = l->next) {
    MenuCacheApp* app = reinterpret_cast<MenuCacheApp*>(l->data);
    const char* iconName = menu_cache_item_get_icon(MENU_CACHE_ITEM(app));
    QString dispName = QString::fromUtf8(menu_cache_item_get_name(MENU_CACHE_ITEM(app)));
    QString comment = QString::fromUtf8(menu_cache_item_get_comment(MENU_CACHE_ITEM(app)));
    QString exec = QString::fromLocal8Bit(menu_cache_app_get_exec(app));
    qDebug() << "app:" << dispName;
    QStandardItem* item = new QStandardItem(QIcon::fromTheme(iconName), dispName);
    item->setEditable(false);
    // item->setData();
    model_->appendRow(item);
  }
  ui.treeView->expandAll();
}

void RunDialog::addProgramsToModel(QStringList programs) {
  Q_FOREACH(QString program, programs) {
    QStandardItem* item = new QStandardItem(program);
    item->setEditable(false);
    model_->appendRow(item);
  }
  ui.treeView->expandAll();
}

void RunDialog::onInputChangedTimeout() {
  inputTimer_->deleteLater();
  inputTimer_ = NULL;

  // refilter
  QString newText = ui.command->text();
  if(newText.length() >= 2) {
    proxyModel_->setFilterFixedString(newText);
    proxyModel_->invalidate();
    proxyModel_->setSourceModel(model_);
  }
  else {
    proxyModel_->setSourceModel(NULL);
  }
}

void RunDialog::onInputChanged() {
  if(!inputTimer_) {
    inputTimer_ = new QTimer();
    inputTimer_->setSingleShot(true);
    connect(inputTimer_, SIGNAL(timeout()), SLOT(onInputChangedTimeout()));
  }
  inputTimer_->start(150);
}
