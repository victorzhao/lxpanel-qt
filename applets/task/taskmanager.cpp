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

#include "taskmanager.h"
#include "taskinfo.h"
#include <QX11Info>

using namespace Lxpanel;

TaskManager::TaskManager(QObject* parent):
  QObject(parent),
  xfitman_(),
  active_(NULL) {

  Application* app = static_cast<Application*>(qApp);
  app->addXEventFilter(this);

  clientWindows_ = xfitman_.getClientList();
  Q_FOREACH(Window window, clientWindows_) {
    TaskInfo* task = new TaskInfo(this, window);
    tasks_.insert(window, task);
  }
}

TaskManager::~TaskManager() {
  Application* app = static_cast<Application*>(qApp);
  app->removeXEventFilter(this);
}

void TaskManager::updateClients() {
  // FIXME: need better algorithm to speed it up

  // WindowList clientsToRemove;
  // WindowList clientsToAdd;
  WindowList newClientWindows = xfitman_.getClientList();
  Q_FOREACH(Window window, clientWindows_) {
    if(!newClientWindows.contains(window)) {
      // clientsToRemove.append(window);
      removeClient(window);
    }
  }

  Q_FOREACH(Window window, newClientWindows) {
    if(!clientWindows_.contains(window)) {
      // clientsToAdd.append(window);
      addClient(window);
    }
  }

  clientWindows_ = newClientWindows;
}

bool TaskManager::x11EventFilter(XEvent* event) {
  if(event->type == PropertyNotify) {
    if(event->xany.window == QX11Info::appRootWindow()) {
      if(event->xproperty.atom == xfitman_.atom("_NET_CLIENT_LIST")) {
        updateClients();
      }
      else if(event->xproperty.atom == xfitman_.atom("_NET_ACTIVE_WINDOW")) {
        Window window = xfitman_.getActiveWindow();
        QHash<Window, TaskInfo*>::iterator it = tasks_.find(window);

        if(it != tasks_.end()) // found
          active_ = *it;
        else
          active_ = NULL; // FIXME: should this happen?

        Q_EMIT activeChanged(active_);
      }
      else if(event->xproperty.atom == xfitman_.atom("_NET_CURRENT_DESKTOP")) {
      }
    }
    else {
      QHash<Window, TaskInfo*>::iterator it = tasks_.find(event->xany.window);

      if(it != tasks_.end()) {
        TaskInfo* task = *it;
        // Q_EMIT taskChanged(task, mask);
      }
    }
  }

#if 0
  long changedProps = NETRootInfo::event(event);

  if(changedProps) {
    if(changedProps & NET::ActiveWindow) {
      Window window = activeWindow();
      QHash<Window, TaskInfo*>::iterator it = tasks_.find(window);

      if(*it != active_) {
        active_ = *it;
        Q_EMIT activeChanged(active_);
      }
    }
  }

  QHash<Window, TaskInfo*>::iterator it = tasks_.find(event->xany.window);

  if(it != tasks_.end()) {
    TaskInfo* task = *it;
    unsigned long mask = task->event(event);

    if(mask != 0)
      Q_EMIT taskChanged(task, mask);
  }

#endif
  return false;
}

void TaskManager::addClient(Window window) {
  TaskInfo* task = new TaskInfo(this, window);
  tasks_.insert(window, task);
  Q_EMIT taskAdded(task);
}

void TaskManager::removeClient(Window window) {
  QHash<Window, TaskInfo*>::iterator it = tasks_.find(window);

  if(it != tasks_.end()) {
    TaskInfo* task = *it;
    Q_EMIT taskRemoved(task);
    delete task;
    tasks_.erase(it);
  }
}

