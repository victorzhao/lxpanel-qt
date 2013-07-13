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


#ifndef LXPANEL_APPLETFACTORY_H
#define LXPANEL_APPLETFACTORY_H

#include <QString>

class QWidget;

namespace Lxpanel {

class Applet;
class AppletInfo;

// factory class used to generate new instances of applets
class AppletFactory {

public:
  AppletFactory(QString id):
    id_(id) {
  }

  virtual Applet* create(QWidget* parent) = 0;
  virtual ~AppletFactory();

  const QString id() const {
    return id_;
  }
  void setId(const QString value) {
    id_ = value;
  }

  const QString name() const {
    return name_;
  }
  void setName(const QString value) {
    name_ = value;
  }

  const QString description() const {
    return description_;
  }
  void setDescription(const QString value) {
    description_ = value;
  }

  const QString iconName() const {
    return iconName_;
  }
  
  void setIconName(const QString value) {
    iconName_ = value;
  }
  
  const QString author() const {
    return author_;
  }
  void setAuthor(const QString value) {
    author_ = value;
  }

private:
  QString id_;
  QString name_;
  QString description_;
  QString iconName_;
  QString author_;
};

}

#define LXPANEL_DECLARE_BUILTIN_APPLET(appletClass, id, name, desc) \
namespace Lxpanel { \
class appletClass##Factory: public AppletFactory { \
public: \
  appletClass##Factory(): \
    AppletFactory(id) { \
    setName(name); \
    setDescription(desc); \
  } \
  virtual ~appletClass##Factory() {} \
  virtual Applet* create(QWidget* parent) { \
    return new appletClass(this, parent); \
  } \
}; \
};

#endif // LXPANEL_APPLETFACTORY_H
