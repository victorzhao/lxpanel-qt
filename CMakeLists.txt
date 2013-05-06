project(lxpanel-qt)

cmake_minimum_required(VERSION 2.8.6)

# Qt
find_package(Qt4 REQUIRED QtCore QtGui QtXml)

set(QT_USE_QTXML TRUE)

# glib using pkg-config
find_package(PkgConfig)
pkg_check_modules(GLIB REQUIRED
  glib-2.0
)

find_package(PkgConfig)
pkg_check_modules(X11 REQUIRED
  x11
)

set(CMAKE_CXX_FLAGS "-DQT_NO_KEYWORDS -fno-exceptions")

include_directories(
  ${QT_INCLUDES}
  ${GLIB_INCLUDE_DIRS}
  ${X11_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(CMAKE_AUTOMOC TRUE)

set(lxpanel_qt_SRCS
  application.cpp
  main.cpp
  panel.cpp
  applet.cpp
  appletplugin.cpp
  appletfactory.cpp
  appletpluginfactory.cpp
  # built-in applets
  applets/clock/clockapplet.cpp
  applets/showdesktop/showdesktopapplet.cpp
)

add_definitions(
  -DLXPANEL_LIB_DIR="${CMAKE_INSTALL_PREFIX}/lib/lxpanel-qt"
  -DLXPANEL_DATA_DIR="${CMAKE_INSTALL_PREFIX}/share/lxpanel-qt"
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
