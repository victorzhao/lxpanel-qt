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

namespace Lxpanel {

class TaskInfo: public NETWinInfo {
public:
  TaskInfo(Window window);
  ~TaskInfo();

  Window window() const {
    return window_;
  }

  QString title();
  QPixmap iconPixmap(int size);

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

  bool shaded() const {
    return (state() & NET::Shaded) != 0;
  }
  
  bool skipTaskbar() const {
    return (state() & NET::SkipTaskbar) != 0;
  }

  bool skipPager() const {
    return (state() & NET::SkipPager) != 0;
  }

  bool keepAbove() const {
    return (state() & NET::KeepAbove) != 0;
  }

  bool keepBelow() const {
    return (state() & NET::KeepBelow) != 0;
  }

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

private:
  Window window_;
};
}

#endif // LXPANEL_TASKINFO_H
