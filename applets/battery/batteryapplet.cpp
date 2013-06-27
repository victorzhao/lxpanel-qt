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


#include "batteryapplet.h"
#include <QDBusConnection>
#include <QDBusReply>
#include <QDBusObjectPath>
#include <QDBusContext>
#include <QIcon>
#include <QHBoxLayout>
#include <QLabel>

#include <QMetaMethod>

using namespace Lxpanel;

BatteryIndicatorBar::BatteryIndicatorBar(QDBusInterface* deviceIface):
  QProgressBar(),
  deviceIface_(deviceIface) {

  connect(deviceIface_, SIGNAL(Changed()), SLOT(update()));
  setOrientation(Qt::Vertical);
  setTextVisible(false);
  setMaximumWidth(6);
  //bar->setMaximumHeight(22);
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  setMinimumHeight(10);

  update();
}

BatteryIndicatorBar::~BatteryIndicatorBar() {
  delete deviceIface_;
}

void BatteryIndicatorBar::update() {
  qDebug("device changed!!: %s", qPrintable(deviceIface_->path()));
  int percent = deviceIface_->property("Percentage").toInt();
  setToolTip(tr("%1 %").arg(percent));
  setValue(percent);
}



BatteryApplet::BatteryApplet(QWidget* parent):
  Applet(parent),
  upowerIface_("org.freedesktop.UPower",
               "/org/freedesktop/UPower",
               "org.freedesktop.UPower",
               QDBusConnection::systemBus()),
  widget_(new QWidget()),
  layout_(new QHBoxLayout()),
  iconLabel_(new QLabel()) {

  widget_->setLayout(layout_);
  layout_->addWidget(iconLabel_);
  layout_->setContentsMargins(0, 0, 0, 0);
  layout_->setSpacing(1);

  QIcon icon = QIcon::fromTheme("battery");
  iconLabel_->setPixmap(icon.pixmap(QSize(24, 24)));

  // set style sheet for progress bars
  widget_->setStyleSheet(
    "QProgressBar{"
    "border:1px solid black;" // TODO: make the colors configurable
    "background-color:black;"
    "}QProgressBar::chunk{"
    "background-color:#00ff00;"
    "}"
  );

  QDBusReply<QList<QDBusObjectPath> > devices = upowerIface_.call("EnumerateDevices");
  Q_FOREACH(QDBusObjectPath path, devices.value()) {
    onDeviceAdded(path.path());
  }
  connect(&upowerIface_, SIGNAL(DeviceAdded(QString)), SLOT(onDeviceAdded(QString)));
  connect(&upowerIface_, SIGNAL(DeviceRemoved(QString)), SLOT(onDeviceRemoved(QString)));
}

BatteryApplet::~BatteryApplet() {
  Q_FOREACH(BatteryIndicatorBar* bar, batteryBars_) {
    delete bar;
  }
  delete iconLabel_;
  delete layout_;
  delete widget_;
}

QWidget* BatteryApplet::widget() {
  return widget_;
}

void BatteryApplet::onDeviceAdded(QString objPath) {
  QDBusInterface *deviceIface = new QDBusInterface("org.freedesktop.UPower",
              objPath,
              "org.freedesktop.UPower.Device",
              QDBusConnection::systemBus());
  if(deviceIface->property("Type") != 2) {
    delete deviceIface;
    return;
  }
  BatteryIndicatorBar* bar = new BatteryIndicatorBar(deviceIface);
  batteryBars_.push_back(bar);
  layout_->addWidget(bar);
}

void BatteryApplet::onDeviceRemoved(QString objPath) {
  for(int i = 0; i < batteryBars_.count(); ++i) {
    BatteryIndicatorBar* bar = batteryBars_[i];
    if(bar->deviceIface()->path() == objPath) {
      bar->deleteLater();
      batteryBars_.remove(i);
      break;
    }
  }
}
