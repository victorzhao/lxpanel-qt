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


#include "launchbutton.h"
#include <gio/gdesktopappinfo.h>
#include <libfm-qt/icontheme.h>

using namespace Lxpanel;

LaunchButton::LaunchButton(ButtonType type, QWidget* parent):
  QToolButton(parent),
  appInfo_(NULL),
  terminal_(false),
  type_(type) {

  setAutoRaise(true);
  connect(this, SIGNAL(clicked(bool)), SLOT(onClicked()));
}

void LaunchButton::setType(ButtonType type) {

}

LaunchButton::~LaunchButton() {
  if(appInfo_)
    g_object_unref(appInfo_);
}

void LaunchButton::setDesktopId(QString desktop_id) {
  desktopId_ = desktop_id;
  appInfo_ = (GAppInfo*)g_desktop_app_info_new(qPrintable(desktopId_));
  GIcon* gicon = g_app_info_get_icon(appInfo_);
  QString dispName = QString::fromUtf8(g_app_info_get_display_name(appInfo_));
  qDebug("disp: %s", qPrintable(dispName));
  setText(dispName);
  setToolTip(dispName);
  // FIXME: should we rely on libfm here?
  QIcon icon = Fm::IconTheme::icon(gicon);
  setIcon(icon);
}

void LaunchButton::onClicked() {
  // launch the button
  if(appInfo_) {
    g_app_info_launch(appInfo_, NULL, NULL, NULL);
  }
  else if(commandExec_.isEmpty()) {
    // flags = NEEDS_TERMINAL;
    QByteArray exec = commandExec_.toUtf8();
    appInfo_ = g_app_info_create_from_commandline(exec.constData(), exec.constData(), GAppInfoCreateFlags(0), NULL);
    g_app_info_launch(appInfo_, NULL, NULL, NULL);
    g_object_unref(appInfo_);
    appInfo_ = NULL;
  }
}
