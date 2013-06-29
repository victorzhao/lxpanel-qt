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


#include "volumecontrolalsa.h"

using namespace Lxpanel;

struct AlsaSource {
  GSource source;
  GPollFD* fds;
  int n_fds;
  VolumeControlAlsa* volumeControl;
};

GSourceFuncs VolumeControlAlsa::alsaSourceFuncs = {
  VolumeControlAlsa::prepareAlsaSource,
  VolumeControlAlsa::checkAlsaSource,
  VolumeControlAlsa::dispatchAlsaSource,
  VolumeControlAlsa::finalizeAlsaSource
};

VolumeControlAlsa::VolumeControlAlsa(QObject* parent):
  VolumeControl(parent),
  mixer_(NULL),
  alsaSource_(NULL),
  masterElement_(NULL) {

  initAlsa();
}

VolumeControlAlsa::~VolumeControlAlsa() {
  if(mixer_) {
    snd_mixer_free(mixer_); // Unload all mixer elements and free all related resources.
    snd_mixer_close(mixer_); // Close a mixer and free all related resources.
  }
  if(alsaSource_) {
    g_source_destroy(alsaSource_);
    g_source_unref(alsaSource_);
  }
}

void VolumeControlAlsa::setMuted(bool muted) {
  snd_mixer_selem_set_playback_switch_all(masterElement_, muted ? 0 : 1);
  Q_EMIT changed();
}

// Get the condition of the mute control from the sound system.
bool VolumeControlAlsa::muted() {
  /* The switch is on if sound is not muted, and off if the sound is muted.
   * Initialize so that the sound appears unmuted if the control does not exist. */
  int value = 1;
  if(masterElement_ != NULL)
    snd_mixer_selem_get_playback_switch(masterElement_, SND_MIXER_SCHN_FRONT_LEFT, &value);
  return (value == 0);
}

// Get the presence of the mute control from the sound system.
bool VolumeControlAlsa::hasMute() {
  return ((masterElement_ != NULL) ? snd_mixer_selem_has_playback_switch(masterElement_) : false);
}

void VolumeControlAlsa::setLevel(int level) {
  qDebug("setLevel(%d)", level);
  if(masterElement_ != NULL) {
    snd_mixer_selem_set_playback_volume_all(masterElement_, level);
    // snd_mixer_selem_set_playback_volume(masterElement_, SND_MIXER_SCHN_FRONT_LEFT, level);
    // snd_mixer_selem_set_playback_volume(masterElement_, SND_MIXER_SCHN_FRONT_RIGHT, level);
    Q_EMIT changed();
  }
}

// Get the volume from the sound system.
// This implementation returns the average of the Front Left and Front Right channels.
int VolumeControlAlsa::level() {
  long aleft = 0;
  long aright = 0;
  if(masterElement_ != NULL) {
    snd_mixer_selem_get_playback_volume(masterElement_, SND_MIXER_SCHN_FRONT_LEFT, &aleft);
    snd_mixer_selem_get_playback_volume(masterElement_, SND_MIXER_SCHN_FRONT_RIGHT, &aright);
  }
  return (aleft + aright) >> 1;
}

void VolumeControlAlsa::setDevice(QString name) {

}

QString VolumeControlAlsa::device() {

}

// Initialize the ALSA interface
bool VolumeControlAlsa::initAlsa() {
  /* Access the "default" device. */
  // snd_mixer_selem_id_alloca(&elementId_);
  if(snd_mixer_open(&mixer_, 0) != 0)
    return false;
  // FIXME: should we use default or hw:0 here?
  if(snd_mixer_attach(mixer_, "default") != 0 ||
    snd_mixer_selem_register(mixer_, NULL, NULL) != 0 ||
    snd_mixer_load(mixer_))
    return false;

  // Find Master element, or Front element, or PCM element, or LineOut element.
  // If one of these succeeds, master_element is valid.
  const char* elem_names[] = {"Master", "Front", "PCM", "LineOut"};
  int i;
  for(i = 0; i < 4; ++i) {
    const char* master_name = elem_names[i];
    for(snd_mixer_elem_t* elem = snd_mixer_first_elem(mixer_); elem; elem = snd_mixer_elem_next(elem)) {
      if(snd_mixer_selem_is_active(elem)) {
        const char* name = snd_mixer_selem_get_name(elem);
        if(name && strcmp(name, master_name) == 0) {
          masterElement_ = elem;
          break;
        }
      }
    }
  }
  if(masterElement_ == NULL) // failed to find a master element
    return false;
  // Set the playback volume range as we wish it.
  snd_mixer_selem_set_playback_volume_range(masterElement_, 0, 100);

  // Listen to events from ALSA.
  // In this case, glib mainloop integration works much better
  // then QSocketNotifier. Let's use it here!
  alsaSource_ = g_source_new(&alsaSourceFuncs, sizeof(AlsaSource));
  AlsaSource* asource = reinterpret_cast<AlsaSource*>(alsaSource_);
  asource->volumeControl = this;
  asource->n_fds = 0;
  asource->fds = NULL;
  g_source_attach(alsaSource_, NULL);

  return true;
}

// called by glib after it has done the polling
gboolean VolumeControlAlsa::checkAlsaSource(GSource* source) {
  AlsaSource* asource = reinterpret_cast<AlsaSource*>(source);
  VolumeControlAlsa* pThis = asource->volumeControl;
  unsigned short revents;
  int err = snd_mixer_poll_descriptors_revents(pThis->mixer_, (struct pollfd*)asource->fds, asource->n_fds, &revents);
  return (err == 0) && (revents & POLLIN);
}

// called by glib just before polling all of the fds
gboolean VolumeControlAlsa::prepareAlsaSource(GSource* source, gint* timeout_) {
  AlsaSource* asource = reinterpret_cast<AlsaSource*>(source);
  VolumeControlAlsa* pThis = asource->volumeControl;
  int i;
  if(pThis->mixer_) {
    int n_fds = snd_mixer_poll_descriptors_count(pThis->mixer_);
    bool fds_changed = false;
    if(n_fds != asource->n_fds) { // number of fds to monitor changes (this rarely happens)
      // remove old pollfds and add new ones
      if(asource->fds) {
        for(i = 0; i < asource->n_fds; ++i)
          g_source_remove_poll(source, &asource->fds[i]);
        delete []asource->fds;
      }
      asource->fds = new GPollFD[n_fds]; // allocate memory for new poll fds
      asource->n_fds = n_fds;
      fds_changed = true;
    }

    // GPollFD should be totally the same as struct pollfd. Otherwise, it's an error.
    Q_ASSERT(sizeof(GPollFD) == sizeof(struct pollfd));
    snd_mixer_poll_descriptors(pThis->mixer_, (struct pollfd*)asource->fds, n_fds);

    if(fds_changed) { // if fds changes, we need to add the new ones to the source
      for(i = 0; i < n_fds; ++i) {
        g_source_add_poll(source, &asource->fds[i]);
      }
    }
  }

  // tell glib that we cannot determine if there are data available and need poll() later.
  return FALSE;
}

// called by glib to dispatch events after polling
gboolean VolumeControlAlsa::dispatchAlsaSource(GSource* source, GSourceFunc callback, gpointer user_data) {
  AlsaSource* asource = reinterpret_cast<AlsaSource*>(source);
  VolumeControlAlsa* pThis = asource->volumeControl;
  snd_mixer_handle_events(pThis->mixer_);

  // tell the world that the state of alsa mixer changes
  Q_EMIT pThis->changed();

  return TRUE;
}

void VolumeControlAlsa::finalizeAlsaSource(GSource* source) {
  AlsaSource* asource = reinterpret_cast<AlsaSource*>(source);
  if(asource->fds)
    delete []asource->fds;
}
