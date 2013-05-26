project(lxpanel-qt)

cmake_minimum_required(VERSION 2.8.6)

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

set(CMAKE_CXX_FLAGS "-DQT_NO_KEYWORDS -fno-exceptions")

include_directories(
  ${QT_INCLUDES}
  ${GLIB_INCLUDE_DIRS}
  ${X11_INCLUDE_DIRS}
  ${GTOP_INCLUDE_DIRS}
  ${MENU_CACHE_INCLUDE_DIRS}
  ${LIBFM_QT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_AUTOMOC TRUE)

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
  appletfactory.cpp
  appletpluginfactory.cpp
  # xfitman taken from razor-qt
  xfitman.cpp
  # built-in applets
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
)

qt4_add_dbus_adaptor(lxpanel_qt_SRCS
  org.lxde.LxPanel.Application.xml
  application.h
  Lxpanel::Application
  applicationadaptor
  ApplicationAdaptor
)

add_definitions(
  -DLXPANEL_LIB_DIR="${CMAKE_INSTALL_PREFIX}/lib/lxpanel-qt"
  -DLXPANEL_DATA_DIR="${CMAKE_INSTALL_PREFIX}/share/lxpanel-qt"
  -DLIBFM_QT_API=Q_DECL_IMPORT # fix the problem of libfm-qt
)

add_executable(lxpanel-qt
  ${lxpanel_qt_SRCS}
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
)

install(TARGETS lxpanel-qt
  RUNTIME DESTINATION bin
)


# dynamically loadable applet plugins

set(appmenu_SRCS
  applets/appmenu/appmenuapplet.cpp
)

add_library(appmenu MODULE
  ${appmenu_SRCS}
)

target_link_libraries(appmenu
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTXML_LIBRARY}
  ${GLIB_LIBRARIES}
)

set_target_properties(appmenu PROPERTIES PREFIX "")

install(TARGETS appmenu
  LIBRARY DESTINATION lib/lxpanel-qt/applets
)
