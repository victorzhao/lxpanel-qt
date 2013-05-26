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

#ifndef LXPANEL_TASKINFO_H
#define LXPANEL_TASKINFO_H

#include "../../netwm/netwm.h"
#include <QString>
#include <QPixmap>
#include "taskmanager.h"

namespace Lxpanel {
  
class TaskInfo: public NETWinInfo {
public:
  TaskInfo(TaskManager* manager, Window window);
  ~TaskInfo();

  Window window() const {
    return window_;
  }

  QString title();
  QPixmap iconPixmap(int size);

  TaskManager* manager() const {
    return manager_;
  }

  bool active() {
    return (manager_->activeWindow() == window_);
  }

  void setActive() {
    manager_->setActiveWindow(window_, NET::FromTool, QX11Info::appUserTime(), 0);
  }

  // window state
  bool modal() const {
    return (state() & NET::Modal) != 0;
  }

  bool sticky() const {
    return (state() & NET::Sticky) != 0;
  }

  bool maximizedVertically() const {
    return (state() & NET::MaxVert) != 0;
  }

  bool maximizedHorizontally() const {
    return (state() & NET::MaxHoriz) != 0;
  }

  bool maximized() const {
    return (state() & NET::Max) != 0;
  }
  void setMaximized();

  void setMinimized();
  
  bool shaded() const {
    return (state() & NET::Shaded) != 0;
  }
  void setShaded();

  bool skipTaskbar() const {
    return (state() & NET::SkipTaskbar) != 0;
  }

  bool skipPager() const {
    return (state() & NET::SkipPager) != 0;
  }

  bool keepAbove() const {
    return (state() & NET::KeepAbove) != 0;
  }
  void setKeepAbove();

  bool keepBelow() const {
    return (state() & NET::KeepBelow) != 0;
  }
  void setKeepBelow();

  bool hidden() const {
    return (state() & NET::Hidden) != 0;
  }

  bool fullScreen() const {
    return (state() & NET::FullScreen) != 0;
  }

  bool demandsAttention() const {
    return (state() & NET::DemandsAttention) != 0;
  }

  // should the task be shown in the task bar
  bool showInTaskbar() const {
    return (!skipTaskbar() && !skipPager() && windowType(~(NET::NormalMask|NET::DialogMask)) == NET::Unknown);
  }

  void close() {
    manager_->closeWindowRequest(window_);
  }

private:
  Window window_;
  TaskManager* manager_;
};
}

#endif // LXPANEL_TASKINFO_H
