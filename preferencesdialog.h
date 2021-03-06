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


#ifndef LXPANEL_PREFERENCESDIALOG_H
#define LXPANEL_PREFERENCESDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "ui_preferences.h"

namespace Lxpanel {
  
class Panel;
class Applet;

class PreferencesDialog : public QDialog {
  Q_OBJECT

public:
  explicit PreferencesDialog(Panel* panel);
  virtual void done(int);
  virtual ~PreferencesDialog();
  
private:
  void initGeometryPage();
  void initAppearancePage();
  void initAppletsPage();
  void initAdvancedPage();
  void initGlobalPage();
  
  Applet* selectedApplet() const;

private Q_SLOTS:
  void onScreenEdgeChanged(int value);
  void onAlignmentChanged(int value);
  void onMarginChanged(int value);
  void onLengthChanged(int value);
  void onThicknessChanged(int value);
  
  void onAddApplet();
  void onRemoveApplet();
  void onAppletPref();
  void onMoveUp();
  void onMoveDown();

private:
  Ui::PreferencesDialog ui;
  Panel* panel_;
  QStandardItemModel* appletsModel_;
};

}

#endif // LXPANEL_PREFERENCESDIALOG_H
