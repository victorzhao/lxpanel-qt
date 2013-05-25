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

TaskInfo::TaskInfo(Window window):
  NETWinInfo(QX11Info::display(), window, QX11Info::appRootWindow(), windows_properties, 2),
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
