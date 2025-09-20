#include <QApplication>
#include <QDebug>
#include <JApp/Logger.h>
#include <JApp/Log.h>
#include <JApp/Core/Gui/LogViewer.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    JApp::Logger& logger = JApp::Logger::instance();
    logger.initialize();

    JApp::Core::Gui::LogViewer* v = new JApp::Core::Gui::LogViewer();
    v->show();
    QTimer::singleShot(1000, [](){
        LOG_DEBUG() << "Debug from main!";
        LOG_INFO() <<  "Info from main!";
        LOG_WARN() <<  "Warn from main!";
    });

    return app.exec();
}

