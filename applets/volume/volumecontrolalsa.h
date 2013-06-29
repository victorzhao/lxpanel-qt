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


#ifndef LXPANEL_VOLUMECONTROLALSA_H
#define LXPANEL_VOLUMECONTROLALSA_H

#include "volumecontrol.h"
#include <alsa/asoundlib.h>
#include <glib.h> // we use glib mainloop integration for polling ALSA

namespace Lxpanel {

class VolumeControlAlsa : public VolumeControl {
  Q_OBJECT
private:
  
public:
  explicit VolumeControlAlsa(QObject* parent = 0);
  virtual ~VolumeControlAlsa();

  virtual bool hasMute();
  virtual void setMuted(bool muted);
  virtual bool muted();
  virtual void setLevel(int level);
  virtual int level();
  virtual void setDevice(QString name);
  virtual QString device();
  
private:
  bool initAlsa();

  static gboolean prepareAlsaSource(GSource* source, gint* timeout_);
  static gboolean checkAlsaSource(GSource* source);
  static gboolean dispatchAlsaSource(GSource *source, GSourceFunc callback, gpointer user_data);
  static void finalizeAlsaSource(GSource* source);
  
private:
  snd_mixer_t * mixer_;
  snd_mixer_selem_id_t * elementId_;
  snd_mixer_elem_t * masterElement_;
  // QSocketNotifier** socketNotifiers_;
  // int socketNotifierCount_;
  GSource* alsaSource_;
  static GSourceFuncs alsaSourceFuncs;
};

}

#endif // LXPANEL_VOLUMECONTROLALSA_H
