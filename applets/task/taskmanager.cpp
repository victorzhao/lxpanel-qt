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

using namespace Lxpanel;

static unsigned long desktop_properties[ 2 ] = {
  NET::ActiveWindow|
  NET::ClientList|
  NET::ClientListStacking|
  NET::CurrentDesktop|
  // NET::DesktopViewport |
  NET::NumberOfDesktops|
  NET::Supported,
  NET::WM2ShowingDesktop
};

TaskManager::TaskManager(QObject* parent):
  QObject(parent),
  NETRootInfo(QX11Info::display(), desktop_properties, 2),
  active_(NULL) {

  Application* app = static_cast<Application*>(qApp);
  app->addXEventFilter(this);
  
  int nWindows = clientListCount();
  const Window* windows = clientList();
  for(int i = 0; i < nWindows; ++i) {
    const Window window = windows[i];
    TaskInfo* task = new TaskInfo(this, window);
    tasks_.insert(window, task);
  }
}

TaskManager::~TaskManager() {
  Application* app = static_cast<Application*>(qApp);
  app->removeXEventFilter(this);
}

bool TaskManager::x11EventFilter(XEvent* event) {
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
  return false;
}

void TaskManager::addClient(Window window) {
  NETRootInfo::addClient(window);
  TaskInfo* task = new TaskInfo(this, window);
  tasks_.insert(window, task);
  Q_EMIT taskAdded(task);
}

void TaskManager::removeClient(Window window) {
  NETRootInfo::removeClient(window);
  QHash<Window, TaskInfo*>::iterator it = tasks_.find(window);
  if(it != tasks_.end()) {
    TaskInfo* task = *it;
    Q_EMIT taskRemoved(task);
    delete task;
    tasks_.erase(it);
  }
}


