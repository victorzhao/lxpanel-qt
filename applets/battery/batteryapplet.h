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


#ifndef LXPANEL_BATTERYAPPLET_H
#define LXPANEL_BATTERYAPPLET_H

#include "../../applet.h"
#include "../../appletplugin.h"
#include <QDBusInterface>
#include <QWidget>
#include <QProgressBar>
#include <QVector>

class QLabel;
class QHBoxLayout;

namespace Lxpanel {

class BatteryIndicatorBar: public QProgressBar {
  Q_OBJECT
public:
  BatteryIndicatorBar(QDBusInterface *deviceIface);
  virtual ~BatteryIndicatorBar();

  QDBusInterface* deviceIface() {
    return deviceIface_;
  }

private Q_SLOTS:
  void update();

private:
  QDBusInterface *deviceIface_;
};


class BatteryApplet: public Lxpanel::Applet {
  Q_OBJECT
public:
  BatteryApplet(QWidget* parent = 0);
  ~BatteryApplet();
  virtual QWidget* widget();

private Q_SLOTS:
  void onDeviceAdded(QString objPath);
  void onDeviceRemoved(QString objPath);

private:
  QWidget* widget_;
  QLabel* iconLabel_;
  QHBoxLayout* layout_;
  QDBusInterface upowerIface_;
  QVector<BatteryIndicatorBar*> batteryBars_;
};


class BatteryAppletPlugin: public QObject, public AppletPlugin {
  Q_OBJECT
  Q_INTERFACES(Lxpanel::AppletPlugin)

public:
  BatteryAppletPlugin();
  virtual ~BatteryAppletPlugin();
  virtual Applet* create(QWidget* parent);
};

}

#endif // LXPANEL_BATTERYAPPLET_H
