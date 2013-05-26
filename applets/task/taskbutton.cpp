/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2013  <copyright holder> <email>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "taskbutton.h"
#include "taskinfo.h"
#include <QIcon>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>

using namespace Lxpanel;

TaskButton::TaskButton(TaskInfo* task, QWidget* parent):
  Amarok::ElidingButton(parent),
  task_(task) {

  setCheckable(true);
  QString title = task->title();
  setText(title);
  setToolTip(title);
  QPixmap pixmap = task->iconPixmap(24);
  QIcon icon(pixmap);
  setIcon(icon);
}

TaskButton::~TaskButton() {
}

void TaskButton::contextMenuEvent(QContextMenuEvent* event) {
  QWidget::contextMenuEvent(event);
  
  // create a context menu for the button
  QMenu* menu = new QMenu();
  QAction* action = menu->addAction(tr("Send &To"), this, SLOT(onSendToDesktop()));
  action = menu->addAction(tr("Send To Current &Desktop"), this, SLOT(onSendToCurrentDesktop()));
  menu->addSeparator();
  action = menu->addAction(tr("Ma&ximize"), this, SLOT(onMaximize()));
  action = menu->addAction(tr("&Restore"), this, SLOT(onRestore()));
  action = menu->addAction(tr("Mi&nimize"), this, SLOT(onMinimize()));
  action = menu->addAction(tr("&Shade"), this, SLOT(onShade()));
  action = menu->addAction(tr("&Move"), this, SLOT(onMove()));
  action = menu->addAction(tr("Resi&ze"), this, SLOT(onResize()));
  menu->addSeparator();
  action = menu->addAction(tr("&Layer"));
  menu->addSeparator();
  action = menu->addAction(tr("&Close"), this, SLOT(onClose()));

  connect(menu, SIGNAL(aboutToHide()), menu, SLOT(deleteLater()));
  // FIXME: how to make the menu attached to the button perfectly?
  QPoint buttonPos = pos();
  buttonPos = parentWidget()->mapToGlobal(buttonPos);
  menu->popup(buttonPos);
}

void TaskButton::moveEvent(QMoveEvent* event) {
  Amarok::ElidingButton::moveEvent(event);
  QRect iconRect = rect();
  QPoint pos = iconRect.topLeft();
  pos = mapToGlobal(pos);
  iconRect.moveTo(pos);
  xfitMan().setIconGeometry(task_->window(), &iconRect);
}

void TaskButton::resizeEvent(QResizeEvent* event) {
  Amarok::ElidingButton::resizeEvent(event);
  QRect iconRect = rect();
  QPoint pos = iconRect.topLeft();
  pos = mapToGlobal(pos);
  iconRect.moveTo(pos);
  xfitMan().setIconGeometry(task_->window(), &iconRect);
}

void TaskButton::onKeepAbove() {
  task_->setKeepAbove();
}

void TaskButton::onKeepBelow() {
  task_->setKeepBelow();
}

void TaskButton::onNormalLayer() {
  
}

void TaskButton::onMaximize() {
  task_->setMaximized();
}

void TaskButton::onMinimize() {
  task_->setMinimized();
}

void TaskButton::onMove() {
}

void TaskButton::onResize() {
}

void TaskButton::onRestore() {
}

void TaskButton::onShade() {
  task_->setShaded();
}

void TaskButton::onClose() {
  task_->close();
}

void TaskButton::onSendToCurrentDesktop() {

}

void TaskButton::onSendToDesktop() {

}

void TaskButton::update(TaskInfo::ChangeType changed) {
  switch(changed) {
    case TaskInfo::StateChanged:
      break;
    case TaskInfo::TitleChanged: {
      QString title = task_->title();
      setText(title);
      setToolTip(title);
      break;
    }
    case TaskInfo::IconChanged: {
      QPixmap pixmap = task_->iconPixmap(24);
      QIcon icon(pixmap);
      setIcon(icon);
      break;
    }
  }
}

