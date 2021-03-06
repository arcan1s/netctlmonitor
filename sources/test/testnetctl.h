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

#ifndef TESTNETCTL_H
#define TESTNETCTL_H

#include <QObject>
#include <QVariant>


class Netctl;
class NetctlProfile;

class TestNetctl : public QObject
{
    Q_OBJECT

private slots:
    // initialization
    void initTestCase();
    void cleanupTestCase();
    // netctl
    void test_getRecommendedConfiguration();
    void test_getActiveProfile();
    void test_getProfileStatus();
    void test_isProfileActive();
    void test_isProfileEnabled();
    void test_reenableProfile();
    void test_restartProfile();

private:
    bool helper;
    Netctl *createNetctlObj();
    NetctlProfile *createNetctlProfileObj();
    void createTestProfile();
    void removeTestProfile();
    QList<QVariant> sendDBusRequest(const QString path,
                                    const QString cmd,
                                    const QList<QVariant> args = QList<QVariant>());
};


#endif /* TESTNETCTL_H */
