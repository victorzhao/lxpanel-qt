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

#include <QString>
#include <QPixmap>
#include "taskmanager.h"

namespace Lxpanel {
  
class TaskInfo {
public:
  TaskInfo(TaskManager* manager, Window window);
  ~TaskInfo();

  Window window() const {
    return window_;
  }

  QString title() {
    return xfitMan().getWindowTitle(window_);
  }

  QPixmap iconPixmap(int size);

  TaskManager* manager() const {
    return manager_;
  }

  XfitMan& xfitMan() {
    return manager_->xfitMan();
  }

  bool active() {
    return (xfitMan().getActiveWindow() == window_);
  }

  void setActive() {
    xfitMan().raiseWindow(window_);
  }

  // window state
  bool modal() const {
    return state_.Modal;
  }

  bool sticky() const {
    return state_.Sticky;
  }

  bool maximizedVertically() const {
    return state_.MaximizedVert;
  }

  bool maximizedHorizontally() const {
    return state_.MaximizedHoriz;
  }

  bool maximized() const {
    return state_.MaximizedVert && state_.MaximizedHoriz;
  }
  void setMaximized() {
    xfitMan().maximizeWindow(window_);
  }

  void setMinimized() {
    xfitMan().minimizeWindow(window_);
  }

  bool shaded() const {
    return state_.Shaded;
  }
  void setShaded(bool shade = true) {
    xfitMan().shadeWindow(window_, shade);
  }

  bool skipTaskbar() const {
    return state_.SkipTaskBar;
  }

  bool skipPager() const {
    return state_.SkipPager;
  }

  bool keepAbove() const {
    return state_.AboveLayer;
  }
  void setKeepAbove() {
    xfitMan().setWindowLayer(window_, XfitMan::LayerAbove);
  }

  bool keepBelow() const {
    return state_.BelowLayer;
  }
  void setKeepBelow() {
    xfitMan().setWindowLayer(window_, XfitMan::LayerBelow);
  }

  bool hidden() const {
    return state_.Hidden;
  }

  bool fullScreen() const {
    return state_.FullScreen;
  }

  bool demandsAttention() const {
    return state_.Attention;
  }

  // should the task be shown in the task bar
  bool showInTaskbar() {
    return xfitMan().acceptWindow(window_);
  }

  void close() {
    xfitMan().closeWindow(window_);
  }

private:
  Window window_;
  TaskManager* manager_;
  WindowState state_;
};
}

#endif // LXPANEL_TASKINFO_H
