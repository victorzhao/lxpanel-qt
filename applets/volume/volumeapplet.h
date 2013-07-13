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


#ifndef LXPANEL_VOLUMEAPPLET_H
#define LXPANEL_VOLUMEAPPLET_H

#include "../../applet.h"
#include "../../popup.h"
#include <QToolButton>

class QSlider;
class QCheckBox;
class QVBoxLayout;

namespace Lxpanel {

class VolumeControl;

class VolumeApplet : public Lxpanel::Applet {
  Q_OBJECT
public:

  enum IconType {
    None = -1,
    Muted = 0,
    Low = 1,
    Medium = 2,
    High = 3
  };

  virtual QWidget* widget() {
    return button_;
  }
  explicit VolumeApplet(AppletInfo* info, QWidget* parent = 0);
  virtual void setPanelIconSize(int size);
  virtual bool loadSettings(QDomElement& element);
  virtual bool saveSettings(QDomElement& element);
  virtual void preferences();
  virtual ~VolumeApplet();

private:
  void updateIcon();
  void createPopup();
  
private Q_SLOTS:
  void onVolumeChanged();
  void onClicked();
  void destroyPopup();

private:
  QToolButton* button_;
  VolumeControl* volumeControl_;
  bool muted_;
  int volume_;
  IconType iconType_;

  Popup* popup_;
  QSlider* slider_;
  QCheckBox* mutedButton_;
  QVBoxLayout* popupLayout_;
};

}

#endif // LXPANEL_VOLUMEAPPLET_H
