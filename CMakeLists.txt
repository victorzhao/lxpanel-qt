project(lxpanel-qt)

cmake_minimum_required(VERSION 2.8.6)

include(GNUInstallDirs) # requires cmake 2.8.5

# Qt
find_package(Qt4 REQUIRED QtCore QtGui QtXml)

set(QT_USE_QTXML TRUE)

# glib using pkg-config
find_package(PkgConfig)
pkg_check_modules(GLIB REQUIRED
  glib-2.0>=2.30
  gio-2.0
  gio-unix-2.0
)

pkg_check_modules(X11 REQUIRED
  x11
  xdamage
  xcomposite
  xrender
)

pkg_check_modules(GTOP REQUIRED
  libgtop-2.0
)

pkg_check_modules(MENU_CACHE REQUIRED
  libmenu-cache>=0.3.3
)

pkg_check_modules(LIBFM_QT REQUIRED
  libfm
  libfm-qt
)

# FIXME: make this optional?
find_package(ALSA) # for volume control

set(CMAKE_CXX_FLAGS "-DQT_NO_KEYWORDS -fno-exceptions")

include_directories(
  ${QT_INCLUDES}
  ${GLIB_INCLUDE_DIRS}
  ${X11_INCLUDE_DIRS}
  ${GTOP_INCLUDE_DIRS}
  ${MENU_CACHE_INCLUDE_DIRS}
  ${LIBFM_QT_INCLUDE_DIRS}
  ${ALSA_INCLUDE_DIRS} # for volume control
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_AUTOMOC TRUE) # requires cmake 2.8.6

set(desktop_settings_SRCS
  desktopsettings.cpp
  xsettings/xsettings-client.c
  xsettings/xsettings-common.c
)

set(lxpanel_qt_SRCS
  ${desktop_settings_SRCS}
  application.cpp
  main.cpp
  panel.cpp
  applet.cpp
  appletmanager.cpp
  appletplugin.cpp
  appletinfo.cpp
  appletplugininfo.cpp
  popup.cpp
  # preferences
  preferencesdialog.cpp
  # add applet dialog
  addappletdialog.cpp
  # run dialog
  rundialog.cpp
  # xfitman taken from razor-qt
  xfitman.cpp
  # built-in applets
  applets/appmenu/appmenuapplet.cpp
  applets/clock/clockapplet.cpp
  applets/showdesktop/showdesktopapplet.cpp
  applets/launcher/launcherapplet.cpp
  applets/launcher/launchbutton.cpp
  applets/netstatus/netstatusapplet.cpp
  applets/blank/blankapplet.cpp
  applets/pager/pagerapplet.cpp
  applets/task/taskapplet.cpp
  applets/task/taskmanager.cpp
  applets/task/taskinfo.cpp
  applets/task/taskbutton.cpp
  applets/task/elidingbutton.cpp
  applets/systray/systrayapplet.cpp
  applets/systray/trayicon.cpp
  applets/systray/razortray.cpp
  applets/volume/volumeapplet.cpp
  applets/volume/volumecontrol.cpp
  applets/volume/volumecontrolalsa.cpp
)

qt4_add_dbus_adaptor(lxpanel_qt_SRCS
  org.lxde.LxPanel.Application.xml
  application.h
  Lxpanel::Application
  applicationadaptor
  ApplicationAdaptor
)

set(lxpanel_qt_UIS
  rundialog.ui
  preferences.ui
  addapplet.ui
)

qt4_wrap_ui(lxpanel_qt_UI_H
  ${lxpanel_qt_UIS}
)

add_definitions(
  -DLXPANEL_LIB_DIR="${CMAKE_INSTALL_FULL_LIBDIR}/lxpanel-qt"
  -DLXPANEL_DATA_DIR="${CMAKE_INSTALL_FULL_DATADIR}/lxpanel-qt"
)

add_executable(lxpanel-qt
  ${lxpanel_qt_SRCS}
  ${lxpanel_qt_UI_H}
)

target_link_libraries(lxpanel-qt
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTXML_LIBRARY}
  ${GLIB_LIBRARIES}
  ${X11_LIBRARIES}
  ${GTOP_LIBRARIES}
  ${MENU_CACHE_LIBRARIES}
  ${LIBFM_QT_LIBRARIES}
  ${ALSA_LIBRARIES}
)

install(TARGETS lxpanel-qt
  RUNTIME DESTINATION bin
)


# dynamically loadable applet plugins

set(battery_SRCS
  applets/battery/batteryapplet.cpp
)

add_library(battery MODULE
  ${battery_SRCS}
)

target_link_libraries(battery
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  ${QT_QTXML_LIBRARY}
)

set_target_properties(battery PROPERTIES PREFIX "")

# install dynamic applets
install(TARGETS battery
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/lxpanel-qt/applets
)

# install desktop entry files for the dynamic applets
install(DIRECTORY
  data/applets
  DESTINATION ${CMAKE_INSTALL_DATADIR}/lxpanel-qt
)

# install library headers
# TODO:

# default config file

install(DIRECTORY
  data/default
  data/2panel
  DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/xdg/lxpanel-qt
)


# building tarball with CPack -------------------------------------------------
# To create a source distribution, type:
# make package_source
include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
set (CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README")
set (CPACK_PACKAGE_VENDOR "")
set (CPACK_PACKAGE_VERSION_MAJOR "0")
set (CPACK_PACKAGE_VERSION_MINOR "1")
set (CPACK_PACKAGE_VERSION_PATCH "0")
set (CPACK_GENERATOR TBZ2)
set (CPACK_SOURCE_GENERATOR TBZ2)
set (CPACK_SOURCE_IGNORE_FILES /build/;.gitignore;.*~;.git;.kdev4;temp)
# set (CPACK_SOURCE_PACKAGE_FILE_NAME lxpanel-qt.0.1.0.tar.gz)
include (CPack)
