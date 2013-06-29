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


#include "popup.h"
#include <QDesktopWidget>
#include <QApplication>

using namespace Lxpanel;

Popup::Popup(QWidget* parent): QWidget(parent, Qt::Popup) {
  // setAttribute();
  
}

Popup::~Popup() {

}

void Popup::closeEvent(QCloseEvent* event) {
  Q_EMIT aboutToHide();
  QWidget::closeEvent(event);
}

void Popup::popup(const QPoint& pos) {
  Q_EMIT aboutToShow();

  QPoint adjustedPos = pos;
  QDesktopWidget* desktop = qApp->desktop();
  int screenNum = desktop->screenNumber(this);
  QRect screenRect = desktop->availableGeometry(screenNum);

  adjustSize();
  QSize _size = size();
  qDebug("screen: %d, %d, popup: %d, %d", screenRect.width(), screenRect.height(), _size.width(), _size.height());

  if((pos.y() + _size.height()) > screenRect.bottom())
    adjustedPos.setY(screenRect.bottom() - _size.height());
  if((pos.x() + _size.width()) > screenRect.right())
    adjustedPos.setX(screenRect.right() - _size.width());
  move(adjustedPos);
  show();
}
