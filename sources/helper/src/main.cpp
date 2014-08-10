/***************************************************************************
 *   This file is part of netctl-gui                                       *
 *                                                                         *
 *   netctl-gui is free software: you can redistribute it and/or           *
 *   modify it under the terms of the GNU General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   netctl-gui is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with netctl-gui. If not, see http://www.gnu.org/licenses/       *
 ***************************************************************************/


#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDir>
#include <QDebug>
#include <QTranslator>
#include <iostream>
#include <unistd.h>

#include "language.h"
#include "messages.h"
#include "netctlhelper.h"
#include "version.h"


using namespace std;


bool checkExistSession()
{
    QDBusConnection bus = QDBusConnection::systemBus();
    QDBusMessage request = QDBusMessage::createMethodCall(DBUS_HELPER_SERVICE,
                                                          DBUS_CTRL_PATH,
                                                          DBUS_HELPER_INTERFACE,
                                                          QString("Active"));
    QDBusMessage response = bus.call(request);
    QList<QVariant> arguments = response.arguments();
    return !arguments.isEmpty();
}


int main(int argc, char *argv[])
{
    QMap<QString, QVariant> args = getArgs();
    // reading
    for (int i=1; i<argc; i++) {
        // config path
        if ((QString(argv[i]) == QString("-c")) || (QString(argv[i]) == QString("--config"))) {
            args[QString("config")] = QDir().absoluteFilePath(argv[i+1]);
            i++;
        }
        // debug
        else if ((QString(argv[i]) == QString("-d")) || (QString(argv[i]) == QString("--debug"))) {
            args[QString("debug")] = true;
        }
        // daemonized
        else if (QString(argv[i]) == QString("--nodaemon")) {
            args[QString("nodaemon")] = true;
        }
        // messages
        // help message
        else if ((QString(argv[i]) == QString("-h")) || (QString(argv[i]) == QString("--help"))) {
            args[QString("help")] = true;
        }
        // info message
        else if ((QString(argv[i]) == QString("-i")) || (QString(argv[i]) == QString("--info"))) {
            args[QString("info")] = true;
        }
        // version message
        else if ((QString(argv[i]) == QString("-v")) || (QString(argv[i]) == QString("--version"))) {
            args[QString("version")] = true;
        }
        else {
            args[QString("error")] = true;
        }
    }
    if ((args[QString("debug")].toBool()) ||
            (args[QString("help")].toBool()) ||
            (args[QString("info")].toBool()) ||
            (args[QString("version")].toBool()) ||
            (args[QString("error")].toBool()))
        args[QString("nodaemon")] = true;

    // detach from console
    if (!args[QString("nodaemon")].toBool())
        daemon(0, 0);
#if QT_VERSION >= 0x050000
    QCoreApplication::setSetuidAllowed(true);
#endif
    QCoreApplication a(argc, argv);
    // reread translations according to flags
    QString language = Language::defineLanguage(args[QString("config")].toString(),
            args[QString("options")].toString());
    QTranslator translator;
    translator.load(QString(":/translations-helper/") + language);
    a.installTranslator(&translator);

    // running
    if (args[QString("error")].toBool()) {
        cout << errorMessage().toUtf8().data() << endl;
        cout << helpMessage().toUtf8().data();
        return 127;
    }
    if (args[QString("help")].toBool()) {
        cout << helpMessage().toUtf8().data();
        return 0;
    }
    if (args[QString("info")].toBool()) {
        cout << versionMessage().toUtf8().data() << endl;
        cout << infoMessage().toUtf8().data();
        return 0;
    }
    if (args[QString("version")].toBool()) {
        cout << versionMessage().toUtf8().data();
        return 0;
    }

    // check if exists
    if (checkExistSession())
        return 0;
    NetctlHelper w(0, args);
    return a.exec();
}