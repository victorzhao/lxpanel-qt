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

#ifndef LXPANEL_TASKBUTTON_H
#define LXPANEL_TASKBUTTON_H

#include "elidingbutton.h"
#include "taskinfo.h"

namespace Lxpanel {
  
class TaskButton: public Amarok::ElidingButton {
  Q_OBJECT
public:
  TaskButton(TaskInfo* task, QWidget* parent = 0);
  virtual ~TaskButton();

  TaskInfo* task() const {
    return task_;
  }

  virtual void contextMenuEvent(QContextMenuEvent * event);

private Q_SLOTS:
  
  // layers
  void onKeepAbove();
  void onKeepBelow();
  void onNormalLayer();

  // send to
  void onSendToDesktop();
  void onSendToCurrentDesktop();

  // general operations
  void onMaximize();
  void onRestore();
  void onMinimize();
  void onShade();
  void onMove();
  void onResize();
  void onClose();

private:
  TaskInfo* task_;
};

};

#endif // LXPANEL_TASKBUTTON_H
