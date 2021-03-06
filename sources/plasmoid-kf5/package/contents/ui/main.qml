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

import QtQuick 2.4
import QtQuick.Controls 1.3 as QtControls
import QtQuick.Layouts 1.1
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.kde.plasma.private.netctl 1.0


Item {
    id: main
    // backend
    NetctlAdds {
        id: netctlAdds;
    }

    // variables
    // internal
    property bool debug: netctlAdds.isDebugEnabled()
    property variant fontWeight: {
        "light": Font.Light,
        "normal": Font.Normal,
        "demibold": Font.DemiBold,
        "bold": Font.Bold,
        "black": Font.Black
    }
    property variant align: {
        "left": Text.AlignLeft,
        "center": Text.AlignHCenter,
        "right": Text.AlignRight,
        "justify": Text.AlignJustify
    }
    // external
    property variant iconPath: {
        "true": plasmoid.configuration.activeIconPath,
        "false": plasmoid.configuration.inactiveIconPath,
        "N\\A": plasmoid.configuration.inactiveIconPath
    }
    property string sudoPath: plasmoid.configuration.useSudo ? plasmoid.configuration.sudoPath : ""
    // signals
    signal needIconUpdate(string newIcon)
    signal needTextUpdate(string newText, string newToolTip)
    signal needMenuUpdate(string current, string stringStatus, bool status)

    // init
    Plasmoid.icon: iconPath["false"]
    Plasmoid.backgroundHints: "DefaultBackground"
    Plasmoid.toolTipMainText: "Netctl"
    Plasmoid.associatedApplication: plasmoid.configuration.guiPath

    PlasmaCore.DataSource {
        id: mainData
        engine: "netctl"
        connectedSources: ["active", "current", "extip4", "extip6", "info",
                           "interfaces", "intip4", "intip6", "profiles", "status"]
        interval: plasmoid.configuration.autoUpdateInterval

        onNewData: {
            if (debug) console.log("[main::onNewData] : Update source " + sourceName)

            netctlAdds.setDataBySource(sourceName, data)
        }
    }

    // ui
    GridLayout {
        anchors.fill: parent
        columns: 2

        Image {
            id: icon
            source: iconPath["false"]
        }
        Text {
            id: text
            color: plasmoid.configuration.fontColor
            font.family: plasmoid.configuration.fontFamily
            font.italic: plasmoid.configuration.fontStyle == "italic" ? true : false
            font.pointSize: plasmoid.configuration.fontSize
            font.weight: fontWeight[plasmoid.configuration.fontWeight]
            horizontalAlignment: align[plasmoid.configuration.textAlign]
            renderType: Text.NativeRendering
            textFormat: Text.RichText
            text: "N\\A"
        }
    }

    Component.onCompleted: {
        if (debug) console.log("[main::onCompleted]")

        // actions
        plasmoid.setAction("titleAction", "netctl-gui", plasmoid.icon)
        plasmoid.setAction("startProfile", i18n("Start profile"), "dialog-apply")
        plasmoid.setAction("stopProfile", i18n("Stop profile"), "dialog-close")
        plasmoid.setAction("stopAllProfiles", i18n("Stop all profiles"), "dialog-close")
        plasmoid.setAction("switchToProfile", i18n("Switch to profile"))
        plasmoid.setAction("restartProfile", i18n("Restart profile"), "view-refresh")
        plasmoid.setAction("enableProfile", i18n("Enable profile"))
        plasmoid.setAction("startWifi", i18n("Show WiFi menu"), "netctl-gui-wifi")
        // init submodule
        Plasmoid.userConfiguringChanged(false)

        netctlAdds.needIconToBeUpdated.connect(needIconUpdate)
        netctlAdds.needMenuUpdate.connect(needMenuUpdate)
        netctlAdds.needTextToBeUpdated.connect(needTextUpdate)
    }

    onNeedIconUpdate: {
        if (debug) console.log("[main::onNeedIconUpdate]")

        icon.source = iconPath[newIcon]
        Plasmoid.icon = iconPath[newIcon]
    }

    onNeedTextUpdate: {
        if (debug) console.log("[main::onNeedTextUpdate]")

        text.text = newText
        Plasmoid.toolTipSubText = newToolTip
    }

    onNeedMenuUpdate: {
        if (debug) console.log("[main::onNeedMenuUpdate]")

        var titleAction = plasmoid.action("titleAction")
        var startAction = plasmoid.action("startProfile")
        var stopAction = plasmoid.action("stopProfile")
        var stopAllAction = plasmoid.action("stopAllProfiles")
        var switchToAction = plasmoid.action("switchToProfile")
        var restartAction = plasmoid.action("restartProfile")
        var enableAction = plasmoid.action("enableProfile")
        var wifiAction = plasmoid.action("startWifi")

        titleAction.iconSource = plasmoid.icon
        titleAction.text = current + " " + stringStatus

        if (stringStatus == "(netctl-auto)") {
            startAction.visible = false
            stopAction.visible = false
            stopAllAction.visible = false
            switchToAction.visible = true
            restartAction.visible = false
            enableAction.visible = false
        } else {
            if (current.indexOf("|") > -1) {
                startAction.visible = true
                stopAction.visible = false
                stopAllAction.visible = true
                switchToAction.visible = false
                restartAction.visible = false
                enableAction.visible = false
            } else {
                startAction.visible = true
                stopAction.visible = status
                stopAllAction.visible = false
                switchToAction.visible = false
                restartAction.visible = status
                enableAction.visible = status
            }
            if (status) {
                startAction.text = i18n("Start another profile")
                stopAction.text = i18n("Stop %1", current)
                restartAction.text = i18n("Restart %1", current)
                if (stringStatus.indexOf("enabled") > -1)
                    enableAction.text = i18n("Disable %1", current)
                else
                    enableAction.text = i18n("Enable %1", current)
            } else
                startAction.text = i18n("Start profile")
        }

        wifiAction.visible = plasmoid.configuration.useWifi
    }

    Plasmoid.onUserConfiguringChanged: {
        if (plasmoid.userConfiguring) return
        if (debug) console.log("[main::onUserConfiguringChanged]")

        // helper
        if (plasmoid.configuration.useHelper) {
            netctlAdds.runCmd(plasmoid.configuration.helperPath)
            plasmoid.configuration.useHelper = netctlAdds.checkHelperStatus()
        }
        // init submodule
        netctlAdds.setPattern(plasmoid.configuration.textPattern)
    }

    function action_titleAction() {
        if (debug) console.log("[main::action_titleAction]")

        netctlAdds.runCmd(plasmoid.configuration.guiPath)
    }

    function action_startProfile() {
        if (debug) console.log("[main::action_startProfile]")

        netctlAdds.startProfileSlot(plasmoid.configuration.useHelper,
                                    plasmoid.configuration.netctlPath,
                                    sudoPath)
    }

    function action_stopProfile() {
        if (debug) console.log("[main::action_stopProfile]")

        netctlAdds.stopProfileSlot(plasmoid.configuration.useHelper,
                                   plasmoid.configuration.netctlPath,
                                   sudoPath)
    }

    function action_stopAllProfiles() {
        if (debug) console.log("[main::action_stopAllProfiles]")

        netctlAdds.stopAllProfilesSlot(plasmoid.configuration.useHelper,
                                       plasmoid.configuration.netctlPath,
                                       sudoPath)
    }

    function action_switchToProfile() {
        if (debug) console.log("[main::action_switchToProfile]")

        netctlAdds.switchToProfileSlot(plasmoid.configuration.useHelper,
                                       plasmoid.configuration.netctlAutoPath)
    }

    function action_restartProfile() {
        if (debug) console.log("[main::action_restartProfile]")

        netctlAdds.restartProfileSlot(plasmoid.configuration.useHelper,
                                      plasmoid.configuration.netctlPath,
                                      sudoPath)
    }

    function action_enableProfile() {
        if (debug) console.log("[main::action_enableProfile]")

        netctlAdds.enableProfileSlot(plasmoid.configuration.useHelper,
                                     plasmoid.configuration.netctlPath,
                                     sudoPath)
    }

    function action_startWifi() {
        if (debug) console.log("[main::action_startWifi]")

        netctlAdds.runCmd(plasmoid.configuration.wifiPath)
    }
}
