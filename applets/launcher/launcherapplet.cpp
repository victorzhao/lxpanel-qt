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


#include "launcherapplet.h"
#include "launchbutton.h"
#include <libfm-qt/icontheme.h>

using namespace Lxpanel;

LauncherApplet::LauncherApplet(QWidget* parent): Applet() {
  frame_ = new QFrame(parent);
  layout_ = new QBoxLayout(QBoxLayout::LeftToRight, frame_);
  frame_->setLayout(layout_);
}

LauncherApplet::~LauncherApplet() {
  delete frame_;
}

void LauncherApplet::loadButton(QDomElement& element) {
  LaunchButton* btn = NULL;
  QString type = element.attribute("type");
  if(type == "app") { // application button
    btn = new LaunchButton(LaunchButton::App);
    for(QDomNode node = element.firstChild(); !node.isNull(); node = node.nextSibling()) {
      if(!node.isElement())
        continue;
      QDomElement childElement = node.toElement();
      if(childElement.tagName() == "desktop_id") {
        btn->setDesktopId(childElement.text());
      }
      else // error!!
        return;
    }
  }
  else if(type == "custom") { // custom command button
    btn = new LaunchButton(LaunchButton::Custom);
    for(QDomNode node = element.firstChild(); !node.isNull(); node = node.nextSibling()) {
      if(!node.isElement())
        continue;
      QDomElement childElement = node.toElement();
      QString tagName = childElement.tagName();
      if(tagName == "command") {
        btn->setCommandExec(childElement.text());
      }
      else if(tagName == "icon") {
        FmIcon* fmicon = fm_icon_from_name(qPrintable(childElement.text()));
        if(fmicon) {
          btn->setIcon(Fm::IconTheme::icon(fmicon));
          fm_icon_unref(fmicon);
        }
      }
      else if(tagName == "terminal") {
        btn->setTerminal(childElement.text() == "true");
      }
      else if(tagName == "label") {
        btn->setText(childElement.text());
        btn->setToolTip(childElement.text());
      }
      else // error!!
        return;
    }
  }
  else if(type == "command") {
  }
  if(btn != NULL) {
    layout_->addWidget(btn);
  }
}

bool LauncherApplet::loadSettings(QDomElement& element) {
  Lxpanel::Applet::loadSettings(element);
  for(QDomNode node = element.firstChild(); !node.isNull(); node = node.nextSibling()) {
    QDomElement childElement = node.toElement();
    if(childElement.isNull())
      continue;
    if(childElement.tagName() == "button") {
      loadButton(childElement);
    }
  }
  return true;
}

bool LauncherApplet::saveSettings(QDomElement& element) {
  Lxpanel::Applet::saveSettings(element);

#if 0
 
  foreach(weak Gtk.Widget child in get_children()) {
    var btn = (LaunchButton)child;
    unowned GMarkupDom.Node btn_node;
    LaunchButton.Type type = btn.get_button_type();
    btn_node = config_node.new_child("button", NULL,
    {"type"},
    {enum_to_nick<LaunchButton.Type>(type)});

    switch(type) {
      case LaunchButton.Type.APP:
        btn_node.new_child("desktop_id", btn.get_desktop_id());
        break;
      case LaunchButton.Type.CUSTOM:
        btn_node.new_child("command", btn.get_command_exec());
        btn_node.new_child("icon", btn.get_gicon().to_string());
        btn_node.new_child("label", btn.get_label());
        btn_node.new_child("terminal", btn.get_terminal().to_string());
        break;
      case LaunchButton.Type.COMMAND:
        break;
    }
  }

#endif
  return true;
}
