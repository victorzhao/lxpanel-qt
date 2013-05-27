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


#ifndef LXPANEL_PANEL_H
#define LXPANEL_PANEL_H

#include <QWidget>
#include <QList>
#include <QDomElement>
#include <QBoxLayout>

namespace Lxpanel {

enum SizeMode {
  SizeModeAuto,
  SizeModePercent,
  SizeModePixel
};

enum Position {
  PositionLeft,
  PositionTop,
  PositionRight,
  PositionBottom
};

class Applet;

class Panel : public QWidget {
  Q_OBJECT

public:
  Panel();
  virtual ~Panel();

  bool load(QDomElement& element);
  bool save(QDomElement& element);

  const QList<Applet*>& applets() const {
    return applets_;
  }

  void recalculateGeometry();

  Position position() {
    return position_;
  }
  void setPosition(Position position, bool force = false);

  Qt::Orientation orientation() {
    return orientation_;
  }
  void setOrientation(Qt::Orientation orientation);
  
  // properties
  int leftMargin() {
    return leftMargin_;
  }
  void setLeftMargin(int left_margin);

  int topMargin() {
    return topMargin_;
  }
  void setTopMargin(int top_margin);
  
  int rightMargin() {
    return rightMargin_;
  }
  void setRightMargin(int right_margin);

  int bottomMargin() {
    return bottomMargin_;
  }
  void setBottomMargin(int bottom_margin);
  
  bool spanMonitors() {
    return spanMonitors_;
  }
  void setSpanMonitors(bool span_monitors);

  int thickness() {
    return thickness_;
  }
  void setThickness(int thickness);

  int length() {
    return length_;
  }
  void setLength(int length) {
    if(length_ != length) {
      length_ = length;
      recalculateGeometry();
    }
  }

  SizeMode lengthMode() {
    return lengthMode_;
  }
  void setLengthMode(SizeMode length_mode);

  double alignment() {
    return alignment_;
  }
  void setAlignment(double alignment) {
    if(alignment_ != alignment) {
      alignment_ = alignment;
      recalculateGeometry();
    }
  }

  int screenNum() {
    return screenNum_;
  }
  void setScreenNum(int screen_num) {
    screenNum_ = screen_num;
  }

  int monitor() {
    return monitor_;
  }
  void setMonitor(int monitor) {
    if(monitor_ != monitor) {
      monitor_ = monitor;
      recalculateGeometry();
    }
  }

  bool autoHide() {
    return autoHide_;
  }
  void setAutoHide(bool auto_hide) {
    if(autoHide_ != auto_hide) {
      autoHide_ = auto_hide;
    }
  }

  bool reserveSpace() {
    return reserveSpace_;
  }
  void setReserveSpace(bool reserve_space);

  int iconSize() {
    return iconSize_;
  }
  void setIconSize(int size);

protected:
  bool loadApplet(QDomElement& element);
  void insertApplet(Lxpanel::Applet* applet, int index = -1);
  void reorderApplet(Applet* applet, int new_pos);
  void removeApplet(Applet* applet);
  void reserveScreenSpace(QRect* rect);

  virtual void resizeEvent(QResizeEvent* event);
  virtual void moveEvent(QMoveEvent* event);
  virtual void showEvent(QShowEvent* event);

Q_SIGNALS:
  // an applet is added to the panel
  void appletAdded(Applet* applet, int pos);
  // an applet is removed from the panel
  void appletRemoved(Applet* applet, int pos);
  // an applet is reordered
  void appletReordered(Applet* applet, int new_pos);

private Q_SLOTS:

  void onGeometryChanged();

private:

  Qt::Orientation orientation_; // orientation of the panel
  Position position_; // left, top, right, bottom
  int leftMargin_; // reserved margin for left of screen
  int topMargin_; // reserved margin for top of screen
  int rightMargin_; // reserved margin for right of screen
  int bottomMargin_; // reserved margin for bottom of screen
  bool spanMonitors_; // span across monitors
  int thickness_; // size of the panel
  int length_; // length of the panel
  SizeMode lengthMode_; // mode of length;
  double alignment_; // alignment of the panel, 0.0 - 1.0
  int monitor_; // index of the monitor
  int screenNum_;
  bool autoHide_; // automatically hide the panel
  bool reserveSpace_; // set partial struct
  int iconSize_; // size of icons
  int rows_; // row count

  QRect oldGeometry_;
  bool pendingGeometryChanged_;
  QBoxLayout* layout_;
  QList<Applet*> applets_;
};

}

#endif // LXPANEL_PANEL_H
