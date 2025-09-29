#include <QApplication>
#include <QFile>
#include <QScreen>
#include <QSysInfo>
#include <JApp/Logger.h>
#include <JApp/Log.h>
#include <JApp/Core/Gui/LogViewer.h>

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

    // Initialize logger
    JApp::Logger& logger = JApp::Logger::instance();
    logger.initialize();

    // Log some useful information
    LOG_INFO() << "Build: " << __DATE__ << __TIME__ << (sizeof(void*) == 8 ? "64-bit" : "32-bit");
    LOG_INFO() << "Platform: " << QSysInfo::prettyProductName() << "(" << QSysInfo::kernelVersion() << ")";
    LOG_INFO() << "Qt version: " << QT_VERSION_STR;
    LOG_INFO() << "Screen: " << QGuiApplication::primaryScreen()->geometry().width() << "x" << QGuiApplication::primaryScreen()->geometry().height();

    // Style the application
    app.setStyleSheet(loadStyleSheet());

    // Temporary code to show log viewer
    JApp::Core::Gui::LogViewer* v = new JApp::Core::Gui::LogViewer();
    v->show();
    QTimer* t = new QTimer();
    t->setInterval(500);
    t->setSingleShot(false);
    QObject::connect(t, &QTimer::timeout, t, [](){
        LOG_WARN() << "Hi from main";
        LOG_INFO() << "Hi from main";
        LOG_CRITICAL() << "Hi from main";
    });
    t->start();

    // Execute the application
    int appExitCode = app.exec();
    LOG_INFO() << "Application exited with code: " << appExitCode;

    // Gracefully stop logger
    logger.shutdown();

    return appExitCode;
}

