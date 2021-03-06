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


#include "clockapplet.h"
#include <QTimer>
#include <QDateTime>
#include <QVBoxLayout>
#include <QCalendarWidget>
#include <QMouseEvent>
#include "../../popup.h"

using namespace Lxpanel;

ClockLabel::ClockLabel(QWidget* parent, Qt::WindowFlags f):
  QLabel(parent, f) {
}

ClockLabel::~ClockLabel() {
}

void ClockLabel::mousePressEvent(QMouseEvent* event) {
  QLabel::mousePressEvent(event);
  if(event->button() == Qt::LeftButton) {
    Popup* popup = new Popup();
    QCalendarWidget* calendar = new QCalendarWidget(popup);
    QVBoxLayout* layout = new QVBoxLayout(popup);
    popup->setLayout(layout);
    layout->setMargin(0);
    layout->addWidget(calendar);
    connect(popup, SIGNAL(aboutToHide()), popup, SLOT(deleteLater()));

    QPoint _pos = parentWidget()->mapToGlobal(pos());
    popup->popup(_pos);
  }
}


ClockApplet::ClockApplet(AppletInfo* info, QWidget* parent):
  Applet(info, parent) {

  label_ = new ClockLabel(parent);
  label_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  timer_ = new QTimer();
  connect(timer_, SIGNAL(timeout()), SLOT(onTimeout()));
  timer_->start(1000);
  onTimeout();
}

ClockApplet::~ClockApplet() {
  timer_->stop();
  delete timer_;
  delete label_;
}

void ClockApplet::onTimeout() {
  QDateTime current = QDateTime::currentDateTime();
  label_->setText(current.toString("hh:mm:ss"));
}
