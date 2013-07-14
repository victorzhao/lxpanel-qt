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


#include "preferencesdialog.h"
#include "panel.h"
#include "applet.h"
#include "appletinfo.h"
#include <QIcon>

using namespace Lxpanel;

PreferencesDialog::PreferencesDialog(Panel* panel):
  QDialog(panel),
  panel_(panel),
  appletsModel_(new QStandardItemModel()) {

  ui.setupUi(this);

  initGeometryPage();
  initAppearancePage();
  initAppletsPage();
  initAdvancedPage();
  initGlobalPage();
  
}

void PreferencesDialog::done(int code) {
  QDialog::done(code);
}

PreferencesDialog::~PreferencesDialog() {
  delete appletsModel_;
}

void PreferencesDialog::initGeometryPage() {
  ui.screenEdge->setCurrentIndex(int(panel_->position()));
  connect(ui.screenEdge, SIGNAL(currentIndexChanged(int)), SLOT(onScreenEdgeChanged(int)));

  ui.alignment->setValue(panel_->alignment() * 100);
  connect(ui.alignment, SIGNAL(valueChanged(int)), SLOT(onAlignmentChanged(int)));
  
  ui.leftMargin->setValue(panel_->leftMargin());
  ui.topMargin->setValue(panel_->topMargin());
  ui.rightMargin->setValue(panel_->rightMargin());
  ui.bottomMargin->setValue(panel_->bottomMargin());
  connect(ui.leftMargin, SIGNAL(valueChanged(int)), SLOT(onMarginChanged(int)));
  connect(ui.topMargin, SIGNAL(valueChanged(int)), SLOT(onMarginChanged(int)));
  connect(ui.rightMargin, SIGNAL(valueChanged(int)), SLOT(onMarginChanged(int)));
  connect(ui.bottomMargin, SIGNAL(valueChanged(int)), SLOT(onMarginChanged(int)));

  ui.length->setValue(panel_->length());
  connect(ui.length, SIGNAL(valueChanged(int)), SLOT(onLengthChanged(int)));

  ui.thickness->setValue(panel_->thickness());
  connect(ui.thickness, SIGNAL(valueChanged(int)), SLOT(onThicknessChanged(int)));
}

void PreferencesDialog::initAppearancePage() {
  ui.iconSize->setValue(panel_->iconSize());

}

void PreferencesDialog::initAppletsPage() {
  Q_FOREACH(Applet* applet, panel_->applets()) {
    AppletInfo* info = applet->info();
    QIcon icon;
    if(!info->iconName().isEmpty())
      icon = QIcon::fromTheme(info->iconName());
    QStandardItem* item = new QStandardItem(icon, info->name());
    item->setEditable(false);
    item->setData(qVariantFromValue<void*>(applet));
    appletsModel_->appendRow(item);
  }
  ui.appletsView->setModel(appletsModel_);
  
  connect(ui.addApplet, SIGNAL(clicked(bool)), SLOT(onAddApplet()));
  connect(ui.removeApplet, SIGNAL(clicked(bool)), SLOT(onRemoveApplet()));
  connect(ui.appletPref, SIGNAL(clicked(bool)), SLOT(onAppletPref()));
  connect(ui.moveUp, SIGNAL(clicked(bool)), SLOT(onMoveUp()));
  connect(ui.moveDown, SIGNAL(clicked(bool)), SLOT(onMoveDown()));
}

void PreferencesDialog::initAdvancedPage() {

}

void PreferencesDialog::initGlobalPage() {

}

void PreferencesDialog::onScreenEdgeChanged(int value) {
  panel_->setPosition(Position(value));
}

void PreferencesDialog::onAlignmentChanged(int value) {
  panel_->setAlignment(double(value)/100);
}

void PreferencesDialog::onMarginChanged(int value) {
  QSpinBox* margin = static_cast<QSpinBox*>(sender());
  if(margin == ui.leftMargin)
    panel_->setLeftMargin(value);
  else if(margin == ui.topMargin)
    panel_->setTopMargin(value);
  else if(margin == ui.rightMargin)
    panel_->setRightMargin(value);
  else if(margin == ui.bottomMargin)
    panel_->setBottomMargin(value);
}

void PreferencesDialog::onLengthChanged(int value) {
  panel_->setLength(value);
}

void PreferencesDialog::onThicknessChanged(int value) {
  panel_->setThickness(value);
}

void PreferencesDialog::onAddApplet() {
  Applet* applet = panel_->addApplet();
  if(applet) {
    
  }
}

void PreferencesDialog::onRemoveApplet() {
  Applet* applet = selectedApplet();
  if(applet) {
    panel_->removeApplet(applet);
  }
}

Applet* PreferencesDialog::selectedApplet() const {
  Applet* applet = NULL;
  QModelIndex index = ui.appletsView->selectionModel()->selectedRows().first();
  if(index.isValid()) {
    QStandardItem* item = appletsModel_->itemFromIndex(index);
    applet = reinterpret_cast<Applet*>(qVariantValue<void*>(item->data()));
  }
  return applet;
}

void PreferencesDialog::onAppletPref() {
  Applet* applet = selectedApplet();
  if(applet) {
    applet->preferences();
  }
}

void PreferencesDialog::onMoveUp() {
  Applet* applet = selectedApplet();
  if(applet) {
  }
}

void PreferencesDialog::onMoveDown() {
  Applet* applet = selectedApplet();
  if(applet) {
  }
}
