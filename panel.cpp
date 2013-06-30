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


#include "panel.h"
#include "applet.h"
#include "appletfactory.h"
#include "application.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>
#include <QSpacerItem>

#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include "xfitman.h"

using namespace Lxpanel;

Panel::Panel():
  pendingGeometryChanged_(false),
  thickness_(26),
  length_(100),
  lengthMode_(SizeModeAuto),
  QWidget(0) {

  setWindowFlags(windowFlags()|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
  setAttribute(Qt::WA_X11NetWmWindowTypeDock);
  setAttribute(Qt::WA_AlwaysShowToolTips);

  // FIXME: this does not work at all
  setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

  layout_ = new QBoxLayout(QBoxLayout::LeftToRight, this);
  // layout_ = new QHBoxLayout(this);
  layout_->setMargin(0);
  layout_->setContentsMargins(0, 0, 0, 0);
  // layout_->setSizeConstraint(QLayout::SetNoConstraint);
  setLayout(layout_);
}
  
Panel::~Panel() {
  Q_FOREACH(Applet* applet, applets_) {
    // FIXME: how can we unload dynamic modules?
    delete applet;
  }
}

#if 0

// call this action signal to add a new applet
[Signal(action = true)]
public virtual signal void Panel::add_applet_action(Applet ? current_applet) {
  // show the "Add applet" dialog.
  int pos = get_applets().index(current_applet);

  if(pos != -1) {
    var applet = choose_new_applet(null, this);

    if(applet != null) {
      insert_applet(applet, pos);
    }
  }
}

// call this action signal to remove an existing applet
[Signal(action = true)]
public virtual signal void Panel::remove_applet_action(Applet ? current_applet) {
  remove_applet(current_applet);
}

// call this action signal to launch the preferences dialog
[Signal(action = true)]
public virtual signal void Panel::preferences() {
  edit_preferences(this);
}
#endif

/*
void Panel::get_preferred_height(out int min, out int natural) {
  if(preferred_height == -1)
    base.get_preferred_height(out min, out natural);
  else
    min = natural = preferred_height;
}

void Panel::get_preferred_width(out int min, out int natural) {
  if(preferred_width == -1)
    base.get_preferred_width(out min, out natural);
  else
    min = natural = preferred_width;
}
*/

void Panel::showEvent(QShowEvent* event) {
  QWidget::showEvent(event);
  // make the window visible on all desktops
  xfitMan().moveWindowToDesktop(winId(), -1);
}

void Panel::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);

  if(!pendingGeometryChanged_) {
    QTimer::singleShot(0, this, SLOT(onGeometryChanged()));
    pendingGeometryChanged_ = true;
  }

  if(lengthMode_ == SizeModeAuto) {
    updateGeometry(); // reposition the panel
  }
}

void Panel::moveEvent(QMoveEvent* event) {
  QWidget::moveEvent(event);
  if(!pendingGeometryChanged_) {
    QTimer::singleShot(0, this, SLOT(onGeometryChanged()));
    pendingGeometryChanged_ = true;
  }
}

void Panel::onGeometryChanged() {
  qDebug("onGeometryChanged: %s", qPrintable(objectName()));
  QRect newGeometry = geometry();
  if(newGeometry != oldGeometry_) {
    oldGeometry_ = newGeometry;
    if(reserveSpace_) {
      reserveScreenSpace(&oldGeometry_);
    }
  }
  pendingGeometryChanged_ = false;
}

// load an applet from a config file node.
bool Panel::loadApplet(QDomElement& element) {
  QString applet_type = element.attribute("type");
  Application* app = static_cast<Application*>(qApp);
  Applet* applet = app->appletManager().createApplet(applet_type);
  if(applet) {
    if(applet->loadSettings(element)) {
      insertApplet(applet, -1);
      qDebug("applet: %s is loaded", qPrintable(applet_type));
    }
    else {
      app->appletManager().destroyApplet(applet);
      applet = NULL;
    }
  }
  return (applet != NULL);
}

// load the panel from a config file node
bool Panel::load(QDomElement& element) {
  QString id = element.attribute("id");

  if(id.isEmpty())
    return false;

  setObjectName(id);

  for(QDomNode node = element.firstChild(); !node.isNull(); node = node.nextSibling()) {
    QDomElement childElement = node.toElement();

    if(childElement.isNull())
      continue;

    QString tagName = childElement.tagName();
    QString value = childElement.text();

    if(tagName == "applets") {
      for(QDomNode appletNode = childElement.firstChild(); !appletNode.isNull(); appletNode = appletNode.nextSibling()) {
        QDomElement appletElement = appletNode.toElement();

        if(!appletElement.isNull() && appletElement.tagName() == "applet")
          loadApplet(appletElement);
      }
    }
    else if(tagName == "position") {
      if(value == "left") {
        position_ = PositionLeft;
        orientation_ = Qt::Vertical;
      }
      else if(value == "top") {
        position_ = PositionTop;
        orientation_ = Qt::Horizontal;
      }
      else if(value == "right") {
        position_ = PositionRight;
        orientation_ = Qt::Vertical;
      }
      else {
        position_ = PositionBottom;
        orientation_ = Qt::Horizontal;
      }
    }
    else if(tagName == "reserve_space") {
      reserveSpace_ = value == "true";
    }
    else if(tagName == "auto_hide") {
      autoHide_ = value == "true";
    }
    else if(tagName == "xscreen") {
      // FIXME: is it possible to support this in Qt?
      // screenNum_ = value.toInt();
    }
    else if(tagName == "icon_size") {
      iconSize_ = value.toInt();
    }
    else if(tagName == "thickness") {
      thickness_ = value.toInt();
    }
    else if(tagName == "length") {
      length_ = value.toInt();
    }
    else if(tagName == "length_mode") {
      if(value == "percent")
        lengthMode_ = SizeModePercent;
      else if(value == "pixel")
        lengthMode_ = SizeModePixel;
      else
        lengthMode_ = SizeModeAuto;
    }
    else if(tagName == "alignment") {
      alignment_ = value.toDouble();
    }
    else if(tagName == "left_margin") {
      leftMargin_ = value.toInt();
    }
    else if(tagName == "top_margin") {
      topMargin_ = value.toInt();
    }
    else if(tagName == "right_margin") {
      rightMargin_ = value.toInt();
    }
    else if(tagName == "bottom_margin") {
      bottomMargin_ = value.toInt();
    }
    else if(tagName == "monitor") {
      monitor_ = value.toInt();
    }
    else if(tagName == "span_monitors") {
      spanMonitors_ = value == "true";
    }
  }

  // call proper Applet methods here
  Q_FOREACH(Applet * applet, applets_) {

  }

  //QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Preferred);
  //layout_->addSpacerItem(spacer);
  // update geometry
  recalculateGeometry();

  return true;
}

// save the panel to a config file node
bool Panel::save(QDomElement& element) {
  // TODO: save config
#if 0
  node.new_child("position", enum_to_nick<Position>(position));
  node.new_child("left_margin", left_margin.to_string());
  node.new_child("top_margin", top_margin.to_string());
  node.new_child("right_margin", right_margin.to_string());
  node.new_child("bottom_margin", bottom_margin.to_string());
  node.new_child("icon_size", icon_size.to_string());
  node.new_child("thickness", thickness.to_string());
  node.new_child("length", length.to_string());
  node.new_child("length_mode", enum_to_nick<SizeMode>(length_mode));
  node.new_child("alignment", alignment.to_string());
  node.new_child("reserve_space", reserve_space.to_string());
  node.new_child("auto_hide", auto_hide.to_string());
  node.new_child("monitor", monitor.to_string());
  node.new_child("xscreen", screen_num.to_string());
  node.new_child("span_monitors", span_monitors.to_string());
  node.new_child("auto_hide", auto_hide.to_string());
  unowned GMarkupDom.Node applets = node.new_child("applets");

  foreach(unowned Applet applet in get_applets()) {
    unowned AppletInfo info = applet.get_info();
    unowned GMarkupDom.Node applet_node;
    applet_node = applets.new_child("applet", null, {"type"}, {info.type_name});
    applet.save_config(applet_node);
  }

#endif

  return true;
}

// insert an applet to the panel at a specified index
void Panel::insertApplet(Applet* applet, int index) {
  QWidget* appletWidget = applet->widget();
  if(appletWidget) {
    applets_.insert(index, applet);
    // int stretch = 0;//applet->expand() ? 1 : 0;
    int stretch = applet->expand() ? 1 : 0;
    layout_->insertWidget(index, appletWidget, stretch, 0);
    layout_->setAlignment(appletWidget, Qt::AlignVCenter);
    appletWidget->show();
    QRect rc = appletWidget->geometry();
    qDebug("%d,%d,%d,%d", rc.x(), rc.y(), rc.width(), rc.height());
  }
  else {
    // FIXME: handle errors?
  }

  /*
  applet.set_panel_orientation(_orientation);
  applet.set_panel_position(position);
  applet.set_icon_size(icon_size);
  applet.show();

  applet_added(applet, index); // emit a signal
  */
}

// move an applet to a new position specified by index
void Panel::reorderApplet(Applet* applet, int new_pos) {
  QWidget* appletWidget = applet->widget();
  if(appletWidget) {
    layout_->removeWidget(appletWidget);
    layout_->insertWidget(new_pos, appletWidget);

    applets_.removeOne(applet);
    applets_.insert(new_pos, applet);

    Q_EMIT appletReordered(applet, new_pos); // emit a signal
  }
}

// remove an applet from the panel. this caused destruction
// of the applet widget, unless it's referenced by others.
void Panel::removeApplet(Applet* applet) {
  int pos = applets_.indexOf(applet);

  if(pos != -1) {
    applets_.removeOne(applet);
    QWidget* appletWidget = applet->widget();
    layout_->removeWidget(appletWidget);
    delete applet; // FIXME: should we do this?
    Q_EMIT appletRemoved(applet, pos); // emit a signal
  }
}

// resize and reposition the panel according to current monitor size.
void Panel::recalculateGeometry() {
  // qDebug("recalculateGeometry(%s)", qPrintable(objectName()));

  // If length_mode is SizeModeAuto, the length of the panel is
  // determined by Qt so we don't touch it here. We only calculate
  // a new position for it and reposition the panel.
  // Otherwise, if the length of the panel is determined by monitor
  // size, such as SizeModePercent, or it's set to fixed pixel size,
  // we use gtk_set_size_request() here to update its size.
  QDesktopWidget* desktop = QApplication::desktop();
  QRect monitor_rect;

  // FIXME: how to handle the case when the specified monitor does not exist?
  if(monitor_ >= desktop->numScreens()) {
    monitor_ = desktop->primaryScreen();
  }
  // get the size of the monitor we're on.
  if(spanMonitors_) {
    monitor_rect = desktop->geometry();
  }
  else {
    monitor_rect = desktop->screenGeometry(monitor_);
  }

  // add margins
  monitor_rect.adjust(leftMargin_, topMargin_, -rightMargin_, -bottomMargin_);

  int newX = 0, newY = 0, newWidth = 0, newHeight = 0;
  if(orientation_ == Qt::Horizontal) {
    newHeight = thickness_;
    if(lengthMode_ == SizeModePercent)
      newWidth = monitor_rect.width() * length_ / 100;
    else if(lengthMode_ == SizeModePixel)
      newWidth = length_;
    else
      newWidth = width();
    newX = monitor_rect.x() + (int)((monitor_rect.width() - newWidth) * alignment_);
    if(position_ == PositionBottom)
      newY = (monitor_rect.y() + monitor_rect.height()) - newHeight;
    else
      newY = monitor_rect.y();
  }
  else { // orientation == Qt::Vertical
    newWidth = thickness_;
    if(lengthMode_ == SizeModePercent)
      newHeight = monitor_rect.height() * length_ / 100;
    else if(lengthMode_ == SizeModePixel)
      newHeight = length_;
    else
      newHeight = height();
    newY = monitor_rect.y() + (int)((monitor_rect.height() - newHeight) * alignment_);
    if(position_ == PositionRight)
      newX = (monitor_rect.x() + monitor_rect.width()) - newWidth;
    else
      newX = monitor_rect.x();
  }
  // NOTE: setGeometry() excludes window frame and will place the panel
  // at the wrong position. Use move() here.
  // FIXME: we should make the panel totaqlly frameless so the geometry calculation
  // is not affected by window managers.
  move(newX, newY);
  resize(newWidth, newHeight);

  // qDebug("%s: %d, %d, %d, %d, %d, %d\n", qPrintable(objectName()),
  //        newX, newY, newWidth, newHeight, length_, lengthMode_);
}

void Panel::setOrientation(Qt::Orientation orientation) {
  if(orientation_ != orientation) {
    orientation_ = orientation;
    // TODO: orientation
    // layout_->setDirection();

    // call set_panel_orientation() on all applets
    Q_FOREACH(Applet * applet, applets_) {
      // applet.set_panel_orientation(orientation);
    }
  }
}

void Panel::setPosition(Position position, bool force) {
  if(position_ != position || force) {
    position_ = position;
    // change orientation according to position
    switch(position) {
      case PositionTop:
      case PositionBottom:
        orientation_ = Qt::Horizontal;
        break;
      case PositionLeft:
      case PositionRight:
        orientation_ = Qt::Vertical;
        break;
    }

    /*
    // call set_panel_position() on all applets
    Q_FOREACH(Applet* applet, applets_) {
      applet.set_panel_position(position);
    }
    */
    recalculateGeometry();
  }
}

void Panel::setLeftMargin(int left_margin) {
  if(leftMargin_ != left_margin) {
    leftMargin_ = left_margin;
    recalculateGeometry();
  }
}

void Panel::setTopMargin(int top_margin) {
  if(topMargin_ != top_margin) {
    topMargin_ = top_margin;
    recalculateGeometry();
  }
}

void Panel::setRightMargin(int right_margin) {
  if(rightMargin_ != right_margin) {
    rightMargin_ = right_margin;
    recalculateGeometry();
  }
}

void Panel::setBottomMargin(int bottom_margin) {
  if(bottomMargin_ != bottom_margin) {
    bottomMargin_ = bottom_margin;
    recalculateGeometry();
  }
}

void Panel::setSpanMonitors(bool span_monitors) {
  if(spanMonitors_ != span_monitors) {
    spanMonitors_ = span_monitors;
    recalculateGeometry();
  }
}

void Panel::setThickness(int thickness) {
  if(thickness_ != thickness) {
    thickness_ = thickness;
    recalculateGeometry();
  }
}

void Panel::setLengthMode(SizeMode length_mode) {
  if(lengthMode_ != length_mode) {
    lengthMode_ = length_mode;
    recalculateGeometry(); // FIXME: is this needed?
    if(length_mode == SizeModeAuto) {
      // Notifies the layout system that this widget has changed and may need to change geometry.
      updateGeometry();
    }
  }
}

void Panel::setReserveSpace(bool reserve_space) {
  if(reserveSpace_ != reserve_space) {
    reserveSpace_ = reserve_space;
    if(reserve_space) {
      QRect rect = geometry();
      reserveScreenSpace(&rect);
    }
    else {
      reserveScreenSpace(NULL);
    }
  }
}


void Panel::setIconSize(int size) {
  if(iconSize_ != size) {
    Q_FOREACH(Applet * applet, applets_) {
      // applet.set_icon_size(size);
    }
  }
}

// ask the window manager to reserve the specified space for our panel window.
// FIXME: how can we support this in XCB or even Wayland?
void Panel::reserveScreenSpace(QRect* rect) {
  // reserve space for the panel
  // See: http://standards.freedesktop.org/wm-spec/1.3/ar01s05.html#NETWMSTRUTPARTIAL
  int left = 0, right = 0, top = 0, bottom = 0;
  int left_start_y = 0, left_end_y = 0, right_start_y = 0, right_end_y = 0;
  int top_start_x = 0, top_end_x = 0, bottom_start_x = 0, bottom_end_x = 0;
  if(rect != NULL) {
    switch(position_) {
      case PositionTop:
        top = rect->height();
        top_start_x = rect->x();
        top_end_x = rect->x() + rect->width() - 1;
        break;
      case PositionBottom:
        bottom = rect->height();
        bottom_start_x = rect->x();
        bottom_end_x = rect->x() + rect->width() - 1;
        break;
      case PositionLeft:
        left = rect->width();
        left_start_y = rect->y();
        left_end_y = rect->y() + rect->height() - 1;
        break;
      case PositionRight:
        right = rect->width();
        right_start_y = rect->y();
        right_end_y = rect->y() + rect->height() - 1;
        break;
    }
    xfitMan().setStrut(winId(),
                       left, right, top, bottom,
                       left_start_y, left_end_y,
                       right_start_y, right_end_y,
                       top_start_x, top_end_x,
                       bottom_start_x, bottom_end_x);
  }
  else { // remove it
    XDeleteProperty(QX11Info::display(), winId(), xfitMan().atom("_NET_WM_STRUT"));
    XDeleteProperty(QX11Info::display(), winId(), xfitMan().atom("_NET_WM_STRUT_PARTIAL"));
  }
}
