#include <QApplication>
#include <QFile>
#include <QDebug>
#include <JApp/Logger.h>
#include <JApp/Log.h>
#include <JApp/Core/Gui/LogViewer.h>
#include <JApp/Core/Models/FakeLogModel.h>

QString loadStyleSheet()
{
    QFile styleFile(":/style.qss");
    if (!styleFile.open(QFile::ReadOnly)) {
        LOG_WARN() << "Failed to open stylesheet: " << styleFile.errorString();
        return QString();
    }
    return QLatin1String(styleFile.readAll());
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    JApp::Logger& logger = JApp::Logger::instance();
    logger.initialize();

    app.setStyleSheet(loadStyleSheet());

    JApp::Core::Gui::LogViewer* v = new JApp::Core::Gui::LogViewer(nullptr, new JApp::Core::Models::FakeLogModel());
    v->show();

    return app.exec();
}

