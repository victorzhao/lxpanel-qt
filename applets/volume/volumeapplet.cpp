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

#include "volumeapplet.h"
#include "volumecontrol.h"
#include "volumecontrolalsa.h"
#include <QIcon>
#include <QSlider>
#include <QCheckBox>
#include <QVBoxLayout>

using namespace Lxpanel;

VolumeApplet::VolumeApplet(QWidget* parent):
  Applet(parent),
  volume_(0),
  muted_(false),
  popup_(NULL),
  slider_(NULL),
  mutedButton_(NULL),
  popupLayout_(NULL),
  iconType_(None),
  button_(new QToolButton()) {

  button_->setAutoRaise(true);
  connect(button_, SIGNAL(clicked(bool)), SLOT(onClicked()));

  // FIXME: should we do multiple backends here?
  volumeControl_ = new VolumeControlAlsa(); // ALSA backend
  connect(volumeControl_, SIGNAL(changed()), SLOT(onVolumeChanged()));
  onVolumeChanged();
}

VolumeApplet::~VolumeApplet() {
  delete button_;
  delete volumeControl_;

  if(popup_)
    destroyPopup();
}

void VolumeApplet::setPanelIconSize(int size) {
  Applet::setPanelIconSize(size);
}

bool VolumeApplet::loadSettings(QDomElement& element) {
  return Applet::loadSettings(element);
}

bool VolumeApplet::saveSettings(QDomElement& element) {
  return Applet::saveSettings(element);
}

void VolumeApplet::preferences() {
  Applet::preferences();
}

void VolumeApplet::updateIcon() {
  static const char* iconNames[] = {
    "audio-volume-muted",
    "audio-volume-low",
    "audio-volume-medium",
    "audio-volume-high"
  };
  const char* iconName = iconNames[int(iconType_)];
  QIcon icon = QIcon::fromTheme(iconName);
  button_->setIcon(icon);
}

void VolumeApplet::onVolumeChanged() {
  int new_volume = volumeControl_->level();
  bool new_muted_ = volumeControl_->muted();

  if(volume_ != new_volume) {
    volume_ = new_volume;
    if(slider_)
      slider_->setValue(volume_);
  }
  if(new_muted_ != muted_) {
    muted_ = new_muted_;
    if(mutedButton_)
      mutedButton_->setChecked(muted_);
  }

  IconType newIconType = iconType_;
  if(muted_) {
    newIconType = Muted;
    button_->setToolTip(tr("Muted"));
  }
  else {
    button_->setToolTip(tr("Volume: %1 %").arg(volume_));
    if(volume_ > 66)
      newIconType = High;
    else if(volume_ < 33)
      newIconType = Low;
    else
      newIconType = Medium;
  }
  if(iconType_ != newIconType) {
    iconType_ = newIconType;
    updateIcon();
  }
}

void VolumeApplet::onClicked() {
  createPopup();

  QRect rc = button_->geometry();
  QPoint pos = button_->parentWidget()->mapToGlobal(rc.topLeft());
  popup_->popup(pos);
}

void VolumeApplet::createPopup() {
  popup_ = new Popup();
  popupLayout_ = new QVBoxLayout();
  popup_->setLayout(popupLayout_);

  slider_ = new QSlider();
  slider_->setRange(0, 100);
  slider_->setValue(volume_);
  connect(slider_, SIGNAL(valueChanged(int)), volumeControl_, SLOT(setLevel(int)));

  popupLayout_->addWidget(slider_);
  popupLayout_->setAlignment(slider_, Qt::AlignCenter);

  if(volumeControl_->hasMute()) {
    mutedButton_ = new QCheckBox(tr("Mute"));
    mutedButton_->setChecked(muted_);
    connect(mutedButton_, SIGNAL(toggled(bool)), volumeControl_, SLOT(setMuted(bool)));
    popupLayout_->addWidget(mutedButton_);
  }

  connect(popup_, SIGNAL(aboutToHide()), SLOT(destroyPopup()));
}

void VolumeApplet::destroyPopup() {
  delete slider_;
  slider_ = NULL;
  delete mutedButton_;
  mutedButton_ = NULL;
  delete popupLayout_;
  popupLayout_ = NULL;
  popup_->deleteLater();
  popup_ = NULL;
}
