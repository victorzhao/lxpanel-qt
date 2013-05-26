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

#ifndef LXPANEL_TASKAPPLET_H
#define LXPANEL_TASKAPPLET_H

#include "../../applet.h"

#include <QWidget>
#include <QBoxLayout>
#include <QButtonGroup>
#include "taskmanager.h"

namespace Lxpanel {

class TaskButton;

class TaskApplet : public Applet {
  Q_OBJECT
public:

  TaskApplet(QWidget* parent);
  ~TaskApplet();

  virtual QWidget* widget() {
    return widget_;
  }
  
  virtual bool expand() {
    return true;
  }

  virtual bool loadSettings(QDomElement& element);
  virtual bool saveSettings(QDomElement& element) ;

private Q_SLOTS:
  void onTaskAdded(TaskInfo* task);
  void onTaskRemoved(TaskInfo* task);
  void onActiveChanged(TaskInfo* task);
  void onTaskChanged(TaskInfo* task, TaskInfo::ChangeType changed);

  void onButtonClicked(QAbstractButton* abutton);

private:
  TaskButton* addButton(TaskInfo* task);
  TaskButton* findButton(TaskInfo* task);
  
private:
  QWidget* widget_;
  TaskManager taskManager_;
  QBoxLayout* layout_;
  QButtonGroup* buttonGroup_;
};
}

#endif // LXPANEL_TASKAPPLET_H
