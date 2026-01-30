#include "CameraWidget.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setApplicationName("qt_opencv_camera");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Qt6 + OpenCV webcam demo");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption deviceOpt(QStringList() << "d" << "device", "Camera device index (default 0).", "index", "0");
    parser.addOption(deviceOpt);
    parser.process(app);

    int deviceIndex = parser.value(deviceOpt).toInt();

    CameraWidget w;
    w.resize(800, 600);
    if (!w.startCamera(deviceIndex)) {
        qWarning() << "Failed to start camera. Exiting.";
        return 1;
    }
    w.show();

    return app.exec();
}