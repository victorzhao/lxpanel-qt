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

#ifndef LXPANEL_TASKMANAGER_H
#define LXPANEL_TASKMANAGER_H

#include <QObject>
#include <QHash>

#include <QIcon> // need to be in xfitman.h
#include "../../xfitman.h"
#include "../../fixx11h.h"
#include "../../application.h"

#include "taskinfo.h"

namespace Lxpanel {

class TaskManager : public QObject, public XEventFilter {
  Q_OBJECT

public:
  
  TaskManager(QObject* parent = 0);
  ~TaskManager();

  virtual bool x11EventFilter(XEvent* event);
  QList<TaskInfo*> tasks() {
    return tasks_.values();
  }

  TaskInfo* active() const {
    return active_;
  }
  
  XfitMan& xfitMan() {
    return xfitman_;
  }

Q_SIGNALS:
  void taskAdded(TaskInfo* task);
  void taskRemoved(TaskInfo* task);
  void taskChanged(TaskInfo* task, TaskInfo::ChangeType changed);
  void activeChanged(TaskInfo* currentTask);

protected:
  virtual void addClient(Window window, bool emitSignal = true);
  virtual void removeClient(Window window);
  void updateClients();

private:
  QHash<Window, TaskInfo*> tasks_;
  TaskInfo* active_;
  XfitMan xfitman_;
  WindowList clientWindows_;
};
}

#endif // LXPANEL_TASKMANAGER_H
