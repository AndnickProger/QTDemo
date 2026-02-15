import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 600
    height: 400
    title: "Курсы ЦБ РФ (Boost + QML)"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        Button {
            text: worker.isLoading ? "Загрузка..." : "Получить курсы валют"
            Layout.fillWidth: true
            enabled: !worker.isLoading
            onClicked: worker.fetch()
        }

        // Вместо ZStack используем Item
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ScrollView {
                anchors.fill: parent
                clip: true

                TextArea {
                    text: worker.displayText
                    readOnly: true
                    font.pixelSize: 14
                    wrapMode: TextEdit.Wrap
                    placeholderText: "Нажмите кнопку для получения данных..."
                    background: Rectangle {
                        color: "#f9f9f9"
                        border.color: "#ddd"
                    }
                }
            }

            // Индикатор будет поверх ScrollView, так как объявлен ниже
            BusyIndicator {
                anchors.centerIn: parent
                running: worker.isLoading
                visible: worker.isLoading
            }
        }
    }
}