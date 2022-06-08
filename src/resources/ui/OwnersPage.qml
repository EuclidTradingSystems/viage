import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import People

ListView {
    leftMargin: 6
    rightMargin: 6
    topMargin: 3
    bottomMargin: 3
    spacing: 6
    clip: true
    boundsBehavior: Flickable.StopAtBounds

    model : OwnersModel { list: owners }

    onCountChanged: busyDialog.close()

    property bool completed: owners.completed

    delegate: BackgroundRect {
        id: owner

        required property var model
        required property int index
        property bool editing: true

        InfantDelegate {
            title: qsTr("Partenaire")
            model: owner.model

            ColumnLayout {
                spacing: 0
                Layout.leftMargin: 12
                Layout.rightMargin: 12

                LabeledTextField {
                    name: qsTr("AVS")
                    textOf: model.avs
                    canEdit: owner.editing
                    onEdit: function(txt) { model.avs = txt }
                    inputHint: Qt.ImhFormattedNumbersOnly
                    validator: RegularExpressionValidator {
                        regularExpression: /\d{13}?$/
                    }
                }

                DateChooser {
                    dateOf: model
                    editing: owner.editing
                }

                AddressChooser {
                    addressOf: model
                    canEdit: owner.editing
                }

                GroupBox {
                    label: Label {
                        text: qsTr("État civil")
                        font.italic: true
                    }
                    Layout.topMargin: 12
                    Layout.bottomMargin: 12
                    Layout.fillWidth: true

                    ButtonGroup {
                        id: civilStates
                        buttons: civilStateColumn.children
                        onCheckedButtonChanged: model.civilStatus = checkedButton.index
                    }

                    ColumnLayout {
                        width: parent.width

                        ColumnLayout {
                            id: civilStateColumn
                            spacing: 0
                            visible: !civilStateDisplay.visible

                            RadioButton {
                                text: qsTr("Célibataire")
                                readonly property int index: 1
                                checked: model.civilStatus === index
                            }
                            RadioButton {
                                text: qsTr("Marié(e)")
                                readonly property int index: 2
                                checked: model.civilStatus === index
                            }
                            RadioButton {
                                text: qsTr("Partenariat enregistré")
                                readonly property int index: 3
                                checked: model.civilStatus === index
                            }
                            RadioButton {
                                text: qsTr("Séparé(e)")
                                readonly property int index: 4
                                checked: model.civilStatus === index
                            }
                            RadioButton {
                                text: qsTr("Divorcé(e)")
                                readonly property int index: 5
                                checked: model.civilStatus === index
                            }
                            RadioButton {
                                text: qsTr("Partenariat dissous")
                                readonly property int index: 6
                                checked: model.civilStatus === index
                            }
                            RadioButton {
                                text: qsTr("Veuf")
                                readonly property int index: 7
                                checked: model.civilStatus === index
                            }
                        }

                        TextField {
                            id: civilStateDisplay
                            text: civilStates.checkedButton.text
                            readOnly: true
                            visible: !owner.editing
                            Layout.fillWidth: true
                        }
                    }
                }

                RowLayout {
                    spacing: 0

                    Button {
                        id: deleteButton
                        icon.source: "qrc:/icons/trash-alt.svg"
                        visible: owner.editing && model.index > 0
                        onClicked: owners.remove(model.id)
                    }
                }
            }
        }
    }
}
