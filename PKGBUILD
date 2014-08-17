# Maintainer: Evgeniy "arcanis" Alexeev <arcanis.arch at gmail dot com>

pkgbase=netctl-gui
pkgname=('libnetctlgui' 'netctlgui-helper' 'netctl-gui'
         'libnetctlgui-qt4' 'netctlgui-helper-qt4' 'netctl-gui-qt4'
         'kdeplasma-applets-netctl-gui')
pkgver=1.3.0
pkgrel=1
pkgdesc="Qt4/Qt5 GUI for netctl. Also provides a widget for KDE"
arch=('i686' 'x86_64')
url="http://arcanis.name/projects/netctl-gui"
license=('GPL3')
makedepends=('automoc4' 'cmake' 'kdelibs' 'qt5-base' 'qt5-tools')
source=("https://github.com/arcan1s/netctl-gui/releases/download/V.${pkgver}/${pkgbase}-${pkgver}-src.tar.xz")
install="${pkgbase}.install"
md5sums=('d99eba4101fee42ebbfa5735aa706bc9')


prepare() {
  rm -rf "${srcdir}/build-"{plasmoid,qt4,qt5}
  mkdir "${srcdir}/build-"{plasmoid,qt4,qt5}
}


build() {
  cd "${srcdir}/build-plasmoid"
  cmake -DCMAKE_INSTALL_PREFIX=/usr \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_GUI:BOOL=0 \
        -DBUILD_LIBRARY:BOOL=0 \
        -DBUILD_HELPER:BOOL=0 \
        "../${pkgbase}"
  make

  cd "${srcdir}/build-qt4"
  cmake -DCMAKE_INSTALL_PREFIX=/usr \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_DATAENGINE:BOOL=0 \
        -DBUILD_PLASMOID:BOOL=0 \
        -DUSE_QT5:BOOL=0 \
        "../${pkgbase}"
  make

  cd "${srcdir}/build-qt5"
  cmake -DCMAKE_INSTALL_PREFIX=/usr \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_DATAENGINE:BOOL=0 \
        -DBUILD_PLASMOID:BOOL=0 \
        "../${pkgbase}"
  make
}


package_kdeplasma-applets-netctl-gui() {
  pkgdesc="A plasmoid, which interacts with netctl. A part of netctl-gui"
  depends=('netctl' 'kdebase-workspace')
  optdepends=('kdebase-runtime: sudo support'
              'netctlgui-helper: DBus helper daemon'
              'netctlgui-helper-qt4: DBus helper daemon'
              'netctl-gui: graphical front-end'
              'netctl-gui-qt4: graphical front-end'
              'sudo: sudo support')
  install="${pkgbase}.install"

  cd "${srcdir}/build-plasmoid"
  make DESTDIR="${pkgdir}" install
}


package_libnetctlgui() {
  pkgdesc="Qt5 library which interacts with netctl. A part of netctl-gui"
  depends=('netctl' 'qt5-base')
  optdepends=('netctlgui-helper: DBus helper daemon'
              'sudo: sudo support'
              'wpa_supplicant: wifi support')
  provides=('libnetctlgui-qt4')
  conflicts=('libnetctlgui-qt4')

  cd "${srcdir}/build-qt5/netctlgui"
  make DESTDIR="${pkgdir}" install
}


package_netctlgui-helper() {
  pkgdesc="Helper daemon for netctl-gui. A part of netctl-gui"
  depends=('libnetctlgui')
  provides=('netctlgui-helper-qt4')
  conflicts=('netctlgui-helper-qt4')
  backup=('etc/netctlgui-helper.conf')

  cd "${srcdir}/build-qt5/helper"
  make DESTDIR="${pkgdir}" install
}


package_netctl-gui() {
  pkgdesc="Qt5 graphical front-end for netctl. A part of netctl-gui"
  depends=('libnetctlgui' 'xdg-utils')
  optdepends=('kdeplasma-applets-netctl-gui: KDE widget'
              'netctlgui-helper: DBus helper daemon')
  provides=('netctl-gui-qt4')
  conflicts=('netctl-gui-qt4')
  install="${pkgbase}.install"

  cd "${srcdir}/build-qt5/gui"
  make DESTDIR="${pkgdir}" install
}


package_libnetctlgui-qt4() {
  pkgdesc="Qt4 library which interacts with netctl. A part of netctl-gui"
  depends=('netctl' 'qt4')
  optdepends=('netctlgui-helper-qt4: DBus helper daemon'
              'sudo: sudo support'
              'wpa_supplicant: wifi support')
  provides=('libnetctlgui')
  conflicts=('libnetctlgui')

  cd "${srcdir}/build-qt4/netctlgui"
  make DESTDIR="${pkgdir}" install
}


package_netctlgui-helper-qt4() {
  pkgdesc="Helper daemon for netctl-gui. A part of netctl-gui"
  depends=('libnetctlgui-qt4')
  provides=('netctlgui-helper')
  conflicts=('netctlgui-helper')
  backup=('etc/netctlgui-helper.conf')

  cd "${srcdir}/build-qt4/helper"
  make DESTDIR="${pkgdir}" install
}


package_netctl-gui-qt4() {
  pkgdesc="Qt4 graphical front-end for netctl. A part of netctl-gui"
  depends=('libnetctlgui-qt4')
  optdepends=('kdeplasma-applets-netctl-gui: KDE widget'
              'netctlgui-helper-qt4: DBus helper daemon')
  provides=('netctl-gui')
  conflicts=('netctl-gui')
  install="${pkgbase}.install"

  cd "${srcdir}/build-qt4/gui"
  make DESTDIR="${pkgdir}" install
}
