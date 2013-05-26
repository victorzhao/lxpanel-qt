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

#include "taskinfo.h"
#include "../../netwm/netwm.h"
#include <QImage>

using namespace Lxpanel;

static unsigned long windows_properties[2] = {
  NET::WMDesktop|
  NET::WMIcon|
  NET::WMIconName|
  NET::WMName|
  NET::WMPid|
  NET::WMState|
  NET::WMVisibleIconName|
  NET::WMVisibleName|
  NET::WMWindowType,
  NET::WM2AllowedActions|
  NET::WM2GroupLeader|
  NET::WM2WindowClass|
  NET::WM2WindowRole
};

TaskInfo::TaskInfo(TaskManager* manager, Window window):
  NETWinInfo(QX11Info::display(), window, QX11Info::appRootWindow(), windows_properties, 2),
  manager_(manager),
  window_(window) {

}

TaskInfo::~TaskInfo() {

}

QString TaskInfo::title() {
  const char* _title = visibleName();
  if(!_title)
    _title = name();
  return QString::fromUtf8(_title);
}

QPixmap TaskInfo::iconPixmap(int size) {
  NETIcon iconData = icon(size, size);
  if(iconData.data && iconData.size.width > 0 && iconData.size.height > 0) {
    QImage image(iconData.data, iconData.size.width, iconData.size.height, QImage::Format_ARGB32);
    image = image.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    if(!image.isNull())
      return QPixmap::fromImage(image);
  }
  return QPixmap();
}

// taken from KDE kdeui kwindowsystem.cpp
static void sendClientMessageToRoot(Window w, Atom a, long x, long y = 0, long z = 0) {
  XEvent ev;
  long mask;

  memset(&ev, 0, sizeof(ev));
  ev.xclient.type = ClientMessage;
  ev.xclient.window = w;
  ev.xclient.message_type = a;
  ev.xclient.format = 32;
  ev.xclient.data.l[0] = x;
  ev.xclient.data.l[1] = y;
  ev.xclient.data.l[2] = z;
  mask = SubstructureRedirectMask;
  XSendEvent(QX11Info::display(), QX11Info::appRootWindow(), False, mask, &ev);
}

void TaskInfo::setKeepAbove() {

}

void TaskInfo::setKeepBelow() {

}

void TaskInfo::setMaximized() {

}

void TaskInfo::setMinimized() {
  // sendClientMessageToRoot(window_, kde_wm_change_state, IconicState, 1);

  QX11Info inf;
  XIconifyWindow(QX11Info::display(), window_, inf.screen());
}

void TaskInfo::setShaded() {

}
