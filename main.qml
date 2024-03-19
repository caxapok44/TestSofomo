import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: "Geolocation Data Manager"

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        TextField {
            id: inputField
            Layout.fillWidth: true
            placeholderText: "Enter IP Address or URL"
        }

        RowLayout {
            Layout.fillWidth: true
            Button {
                text: "Add"
                onClicked: geoManager.addEntry(inputField.text)
            }
            Button {
                text: "Delete"
                onClicked: geoManager.deleteEntry(inputField.text)
            }
            Button {
                text: "Retrieve"
                onClicked: geoManager.retrieveEntry(inputField.text)
            }
            Button {
                text: "Show All"
                onClicked: geoManager.getAllEntries()
            }
        }
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOn
            clip: true
            contentItem: Flickable {
                    boundsBehavior: Flickable.StopAtBounds
            }

            ColumnLayout {
                width: 2000
                RowLayout {
                    spacing: 10

                    Text {
                        text: "Flag"
                        Layout.preferredWidth: 100
                    }
                    Text {
                        text: "IP"
                        Layout.preferredWidth: 100
                    }
                    Text {
                        text: "URL"
                        Layout.preferredWidth: 100
                    }
                    Text {
                        text: "City"
                        Layout.preferredWidth: 100
                    }
                    Text {
                        text: "Continent"
                        Layout.preferredWidth: 100
                    }
                    Text {
                        text: "Continent Code"
                        Layout.preferredWidth: 100
                    }

                    Text {
                        text: "Country Code"
                        Layout.preferredWidth: 100
                    }
                    Text {
                        text: "Country Name"
                        Layout.preferredWidth: 100
                    }
                    Text {
                        text: "Region Code"
                        Layout.preferredWidth: 100
                    }
                    Text {
                        text: "Region Name"
                        Layout.preferredWidth: 100
                    }
                    Text {
                        text: "ZIP"
                        Layout.preferredWidth: 100
                    }
                    Text {
                        text: "Latitude"
                        Layout.preferredWidth: 150
                    }
                    Text {
                        text: "Longitude"
                        Layout.preferredWidth: 150
                    }
                    Text {
                        text: "capital"
                        Layout.preferredWidth: 100
                    }
                    Text {
                        text: "calling_code"
                        Layout.preferredWidth: 100
                    }
                    Text {
                        text: "is_eu"
                        Layout.preferredWidth: 100
                    }
                }
                ListView {
                    id: listView
                    width: parent.width
                    height: 600
                    model: ListModel {
                        id: geolocationModel
                    }

                    delegate: Rectangle {
                        color: "white"
                        width: listView.width
                        height: 60
                        RowLayout {
                            spacing: 10

                            Image {
                                source: model.country_flag
                                Layout.preferredWidth: 100
                                Layout.preferredHeight: 50
                                fillMode: Image.PreserveAspectFit
                            }

                            Text {
                                text: model.ip
                                Layout.preferredWidth: 100
                            }
                            Text {
                                text: model.url
                                Layout.preferredWidth: 100
                            }
                            Text {
                                text: model.city
                                Layout.preferredWidth: 100
                            }
                            Text {
                                text: model.continent_name
                                Layout.preferredWidth: 100
                            }
                            Text {
                                text: model.continent_code
                                Layout.preferredWidth: 100
                            }
                            Text {
                                text: model.country_code
                                Layout.preferredWidth: 100
                            }
                            Text {
                                text: model.country_name
                                Layout.preferredWidth: 100
                            }
                            Text {
                                text: model.region_code
                                Layout.preferredWidth: 100
                            }
                            Text {
                                text: model.region_name
                                Layout.preferredWidth: 100
                            }

                            Text {
                                text: model.zip
                                Layout.preferredWidth: 100
                            }
                            Text {
                                text: String(model.latitude)
                                Layout.preferredWidth: 150
                            }
                            Text {
                                text: String(model.longitude)
                                Layout.preferredWidth: 150
                            }
                            Text {
                                text: model.capital
                                Layout.preferredWidth: 100
                            }
                            Text {
                                text: model.calling_code
                                Layout.preferredWidth: 100
                            }
                            Text {
                                text: model.is_eu ? "Yes" : "No"
                                Layout.preferredWidth: 100
                            }
                        }
                    }
                }
            }
        }
    }
    Popup {
        id: errorPopup
        x: parent.width / 2 - width / 2
        y: parent.height / 2 - height / 2
        width: parent.width * 0.6
        height: parent.height * 0.6

        background: Rectangle {
            color: "red"
            radius: 10
        }
        Label {
            id: errorMessage
            text: ""
            anchors.centerIn: parent
        }
    }

    Timer {
        id: errorTimer
        interval: 3000 // 3 seconds
        repeat: false
        onTriggered: errorPopup.close()
    }
    Connections {
        target: geoManager

        function onDataReceived(data) {
            if(data === "")
            {
               geolocationModel.clear()
               return;
            }

            var json = JSON.parse(data)
            geolocationModel.clear()
            if (Array.isArray(json)) {
                json.forEach(function (item) {

                    var latitude = parseFloat(item.latitude)
                    var longitude = parseFloat(item.longitude)
                    var geonameId = parseInt(item.geoname_id)
                    var isEu = item.is_eu === 1
                    geolocationModel.append({
                                                "ip": item.ip,
                                                "url": item.url,
                                                "type": item.type,
                                                "continent_code": item.continent_code,
                                                "continent_name": item.continent_name,
                                                "country_code": item.country_code,
                                                "country_name": item.country_name,
                                                "region_code": item.region_code,
                                                "region_name": item.region_name,
                                                "city": item.city,
                                                "zip": item.zip,
                                                "latitude": latitude,
                                                "longitude": longitude,
                                                "geoname_id": geonameId,
                                                "capital": item.capital,
                                                "country_flag": item.country_flag,
                                                "calling_code": item.calling_code,
                                                "is_eu": isEu
                                            })
                })
            } else {
                var latitude = parseFloat(json.latitude)
                var longitude = parseFloat(json.longitude)
                var geonameId = parseInt(json.geoname_id)
                var isEu = json.is_eu === 1
                geolocationModel.append({
                                            "ip": json.ip,
                                            "url": item.url,
                                            "type": json.type,
                                            "continent_code": json.continent_code,
                                            "continent_name": json.continent_name,
                                            "country_code": json.country_code,
                                            "country_name": json.country_name,
                                            "region_code": json.region_code,
                                            "region_name": json.region_name,
                                            "city": json.city,
                                            "zip": json.zip,
                                            "latitude": latitude,
                                            "longitude": longitude,
                                            "geoname_id": geonameId,
                                            "capital": json.capital,
                                            "country_flag": json.country_flag,
                                            "calling_code": json.calling_code,
                                            "is_eu": isEu
                                        })
            }
        }

        function onErrorOccurred(error) {
            console.log("Error:", error)
            errorMessage.text = error
            errorPopup.open()
            errorTimer.start()
        }
    }

    Component.onCompleted: {
        geoManager.getAllEntries()
    }
}
