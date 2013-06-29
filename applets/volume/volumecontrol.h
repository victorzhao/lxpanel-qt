/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2013  <copyright holder> <email>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef LXPANEL_VOLUMECONTROL_H
#define LXPANEL_VOLUMECONTROL_H

#include <QObject>

namespace Lxpanel {

class VolumeControl : public QObject {
  Q_OBJECT
public:
  explicit VolumeControl(QObject* parent = 0);
  virtual ~VolumeControl();

  virtual QString device() = 0;
  virtual void setDevice(QString name) = 0;
  virtual int level() = 0;
  virtual bool muted() = 0;
  virtual bool hasMute() = 0;

public Q_SLOTS:
  virtual void setLevel(int level) = 0;
  virtual void setMuted(bool muted) = 0;

Q_SIGNALS:
  void changed();
};

}

#endif // LXPANEL_VOLUMECONTROL_H
