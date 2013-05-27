/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2013  <copyright holder> <email>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#include "batterywidget.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QIcon>
#include <QPainter>

using namespace Lxpanel;

BatteryWidget::BatteryWidget(QWidget* parent):
  QWidget(parent),
  layout_(new QHBoxLayout()),
  iconLabel_(new QLabel()) {

  setLayout(layout_);
  layout_->addWidget(iconLabel_);
  layout_->setContentsMargins(0, 0, 4, 0);

  QIcon icon = QIcon::fromTheme("battery");
  iconLabel_->setPixmap(icon.pixmap(QSize(24, 24)));
}

BatteryWidget::~BatteryWidget() {
  delete iconLabel_;
  delete layout_;
}

void BatteryWidget::paintEvent(QPaintEvent* event) {
  // QWidget::paintEvent(event);
  QPainter painter(this);
  QBrush brush(QColor(0, 255, 0));
  QRect contentRect = rect();
  contentRect.setLeft(contentRect.right() - 4);
  contentRect.adjust(0, 4, -1, -4);
  painter.save();
  painter.setBrush(brush);
  painter.setPen(QPen(QColor(0, 0, 0)));
  painter.drawRect(contentRect);
  painter.restore();
}

