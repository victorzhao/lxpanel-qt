/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2013  <copyright holder> <email>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "addappletdialog.h"
#include "appletmanager.h"
#include "application.h"

using namespace Lxpanel;

AddAppletDialog::AddAppletDialog(QWidget* parent, Qt::WindowFlags f):
  QDialog(parent, f),
  appletInfo_(NULL) {

  ui.setupUi(this);
  initAppletList();
}

AddAppletDialog::~AddAppletDialog() {
}

void AddAppletDialog::initAppletList() {
  AppletManager& manager = static_cast<Application*>(qApp)->appletManager();
  Q_FOREACH(AppletInfo* info, manager.applets()) {
    QListWidgetItem* item = new QListWidgetItem(info->name());
    item->setData(Qt::UserRole, qVariantFromValue<void*>(info));
    ui.listWidget->addItem(item);
  }
}

void AddAppletDialog::accept() {
  QListWidgetItem* item = ui.listWidget->currentItem();
  if(item) {
    AppletInfo* info = reinterpret_cast<AppletInfo*>(qVariantValue<void*>(item->data(Qt::UserRole)));
    if(info)
      appletInfo_ = info;
  }
  QDialog::accept();
}
