/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2013  PCMan <email>

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


#ifndef LXPANEL_NETSTATUSAPPLET_H
#define LXPANEL_NETSTATUSAPPLET_H

#include "../../applet.h"
#include <QLabel>
#include <QTimer>
#include <QIcon>
#include <glib.h>

namespace Lxpanel {

class NetStatusApplet : public Lxpanel::Applet {
  Q_OBJECT
public:
  virtual QWidget* widget() {
    return iconLabel_;
  }
  explicit NetStatusApplet(QWidget* parent = 0);
  virtual ~NetStatusApplet();
  virtual bool loadSettings(QDomElement& element);
  virtual bool saveSettings(QDomElement& element);
  virtual void preferences();

private:
  void loadIcons();
  QIcon loadIcon(const char** names, int num);

private Q_SLOTS:
  void onTimeout();

private:
  QLabel* iconLabel_;
  QTimer timer_;
  QString iface; // network interface
  guint64 last_rx;
  guint64 last_tx;

  QIcon icon_error;
  QIcon icon_idle;
  QIcon icon_offline;
  QIcon icon_rx;
  QIcon icon_tx;
  QIcon icon_tx_rx;
};

}

#endif // LXPANEL_NETSTATUSAPPLET_H
