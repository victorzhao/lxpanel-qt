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


#include "netstatusapplet.h"
#include <glibtop.h>
#include <glibtop/netload.h>

using namespace Lxpanel;

NetStatusApplet::NetStatusApplet(QWidget* parent):
  Applet(),
  timer_(),
  last_rx(0),
  last_tx(0),
  iface("eth0") {

  loadIcons();
  iconLabel_ = new QLabel();
  iconLabel_->setPixmap(icon_idle.pixmap(QSize(24, 24)));
  timer_.start(500);
  connect(&timer_, SIGNAL(timeout()), SLOT(onTimeout()));
}

NetStatusApplet::~NetStatusApplet() {
  delete iconLabel_;
  timer_.stop();
}

QIcon NetStatusApplet::loadIcon(const char** names, int num) {
  QIcon icon;
  for(int i = 0; i < num; ++i) {
    icon = QIcon::fromTheme(names[i]);
    if(!icon.isNull())
      break;
  }
  return icon;
}

void NetStatusApplet::loadIcons() {
  const char* icon_error_names[] = {"network-error", "gnome-netstatus-error"};
  icon_error = loadIcon(icon_error_names, 2);

  const char* icon_idle_names[] = {"network-idle", "gnome-netstatus-idle"};
  icon_idle = loadIcon(icon_idle_names, 2);
  
  const char* icon_offline_names[] = {"network-offline", "gnome-netstatus-disconn"};
  icon_offline = loadIcon(icon_offline_names, 2);
  
  const char* icon_rx_names[] = {"network-receive", "gnome-netstatus-rx"};
  icon_rx = loadIcon(icon_rx_names, 2);
  
  const char* icon_tx_names[] = {"network-transmit", "gnome-netstatus-tx"};
  icon_tx = loadIcon(icon_tx_names, 2);
  
  const char* icon_tx_rx_names[] = {"network-transmit-receive", "gnome-netstatus-txrx"};
  icon_tx_rx = loadIcon(icon_tx_rx_names, 2);
}

bool NetStatusApplet::loadSettings(QDomElement& element) {
  Lxpanel::Applet::loadSettings(element);
  for(QDomNode node = element.firstChild(); !node.isNull(); node = node.nextSibling()) {
    QDomElement childElement = node.toElement();
    if(childElement.isNull())
      continue;
    QString tagName = childElement.tagName();
    QString value = childElement.text();
    if(tagName == "iface") {
      iface = value;
    }
    else if(tagName == "command") {
    }
  }
  return true;
}

bool NetStatusApplet::saveSettings(QDomElement& element) {
  Lxpanel::Applet::saveSettings(element);
  // TODO: save config here
}

void NetStatusApplet::preferences() {
  Lxpanel::Applet::preferences();
}

void NetStatusApplet::onTimeout() {
  glibtop_netload netload;
  glibtop_get_netload(&netload, qPrintable(iface));
  bool tx = false, rx = false;

  if(netload.packets_in > last_rx) {
    rx = true;
    last_rx = netload.packets_in;
  }

  if(netload.packets_out > last_tx) {
    tx = true;
    last_tx = netload.packets_out;
  }

  QIcon icon;
  if(tx && rx)
    icon = icon_tx_rx;
  else if(tx)
    icon = icon_tx;
  else if(rx)
    icon = icon_rx;
  else
    icon = icon_idle;
  iconLabel_->setPixmap(icon.pixmap(QSize(24, 24)));
}
