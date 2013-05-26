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

#include "taskapplet.h"
#include "taskinfo.h"
#include <QIcon>
#include "taskbutton.h"

using namespace Lxpanel;

TaskApplet::TaskApplet(QWidget* parent):
  Applet(parent),
  layout_(new QBoxLayout(QBoxLayout::LeftToRight)),
  buttonGroup_(new QButtonGroup()),
  widget_(new QWidget(parent)) {

  layout_->setContentsMargins(0, 0, 0, 0);
  layout_->setMargin(0);
  layout_->setSpacing(1);
  layout_->addStretch(1);
  widget_->setLayout(layout_);

  Window active = xfitMan().getActiveWindow();
  QList<TaskInfo*> tasks = taskManager_.tasks();
  Q_FOREACH(TaskInfo* task, tasks) {
    if(task->showInTaskbar()) {
      TaskButton* button = addButton(task);
      if(task->window() == active)
	button->setChecked(true);
    }
  }

  connect(&taskManager_, SIGNAL(taskAdded(TaskInfo*)), SLOT(onTaskAdded(TaskInfo*)));
  connect(&taskManager_, SIGNAL(taskRemoved(TaskInfo*)),  SLOT(onTaskRemoved(TaskInfo*)));
  connect(&taskManager_, SIGNAL(taskChanged(TaskInfo*,TaskInfo::ChangeType)), SLOT(onTaskChanged(TaskInfo*,TaskInfo::ChangeType)));
  connect(&taskManager_, SIGNAL(activeChanged(TaskInfo*)), SLOT(onActiveChanged(TaskInfo*)));

  connect(buttonGroup_, SIGNAL(buttonClicked(QAbstractButton*)), SLOT(onButtonClicked(QAbstractButton*)));
}

TaskApplet::~TaskApplet() {
  delete buttonGroup_;
  delete layout_;
  delete widget_;
}

bool TaskApplet::loadSettings(QDomElement& element) {
  return true;
}

bool TaskApplet::saveSettings(QDomElement& element) {
  return true;
}

TaskButton* TaskApplet::findButton(TaskInfo* task) {
  Q_FOREACH(QAbstractButton* abutton, buttonGroup_->buttons()) {
    TaskButton* button = static_cast<TaskButton*>(abutton);
    if(button->task() == task) {
      return button;
    }
  }
  return NULL;
}

TaskButton* TaskApplet::addButton(TaskInfo* task) {
  TaskButton* button = new TaskButton(task);
  button->setMaximumWidth(150);
  button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  layout_->insertWidget(layout_->count() - 1, button, 0, Qt::AlignLeft);
  buttonGroup_->addButton(button);
  return button;
}

void TaskApplet::onTaskAdded(TaskInfo* task) {
  if(task->showInTaskbar()) {
    TaskButton* button = addButton(task);
  }
}

void TaskApplet::onTaskRemoved(TaskInfo* task) {
  TaskButton* button = findButton(task);
  if(button) {
    layout_->removeWidget(button);
    buttonGroup_->removeButton(button);
    delete button;
  }
}

void TaskApplet::onButtonClicked(QAbstractButton* abutton) {
  TaskButton* button = static_cast<TaskButton*>(abutton);
  TaskInfo* task = button->task();
  if(!task->active())
    task->setActive();
  else
    task->setMinimized();
}

void TaskApplet::onActiveChanged(TaskInfo* task) {
  TaskButton* button = findButton(task);
  if(button) {
    button->setChecked(true);
  }
  else {
    button = static_cast<TaskButton*>(buttonGroup_->checkedButton());
    if(button)
      button->setChecked(false);
  }
}

void TaskApplet::onTaskChanged(TaskInfo* task, TaskInfo::ChangeType changed) {
  TaskButton* button = findButton(task);
  if(button)
    button->update(changed);
}
