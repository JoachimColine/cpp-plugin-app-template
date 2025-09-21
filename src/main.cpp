#include <QApplication>
#include <QDebug>
#include <JApp/Logger.h>
#include <JApp/Log.h>
#include <JApp/Core/Gui/LogViewer.h>
#include <JApp/Core/Models/FakeLogModel.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    JApp::Logger& logger = JApp::Logger::instance();
    logger.initialize();

    JApp::Core::Gui::LogViewer* v = new JApp::Core::Gui::LogViewer(nullptr, new JApp::Core::Models::FakeLogModel());
    v->show();

    return app.exec();
}

