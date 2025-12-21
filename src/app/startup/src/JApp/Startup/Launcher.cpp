#include "JApp/Startup/Launcher.h"
#include <JApp/Logger.h>
#include <JApp/Log.h>
#include <JApp/Core/Gui/LogViewer.h>
#include <JApp/Plugins/PluginsInitializer.h>
#include <JApp/Startup/SplashScreen.h>
#include <QApplication>
#include <QScreen>

using namespace JApp;

Launcher::Launcher(QApplication& application) :
    m_application(application)
{

}

QString Launcher::loadStyleSheet()
{
    QFile styleFile(":/style.qss");
    if (!styleFile.open(QFile::ReadOnly)) {
        LOG_WARN() << "Failed to open stylesheet: " << styleFile.errorString();
        return QString();
    }
    return QLatin1String(styleFile.readAll());
}

void Launcher::launch()
{
    // Initialize logger
    JApp::Logger& logger = JApp::Logger::instance();
    logger.initialize();

    // Log some useful information
    LOG_INFO() << "Build: " << __DATE__ << __TIME__ << (sizeof(void*) == 8 ? "64-bit" : "32-bit");
    LOG_INFO() << "Platform: " << QSysInfo::prettyProductName() << "(" << QSysInfo::kernelVersion() << ")";
    LOG_INFO() << "Qt version: " << QT_VERSION_STR;
    LOG_INFO() << "Application version: " << APP_VERSION_FULL;
    LOG_INFO() << "Screen: " << QGuiApplication::primaryScreen()->geometry().width() << "x" << QGuiApplication::primaryScreen()->geometry().height();

    // Style the application
    m_application.setStyleSheet(loadStyleSheet());

    // Prepare plugins initialization
    JApp::PluginsInitializer& pluginsInitializer = JApp::PluginsInitializer::instance();
    pluginsInitializer.setDirectory(QCoreApplication::applicationDirPath() + "/plugins");

    // Instantiate splashscreen to monitor plugins loading
    QPixmap pic(QString(":/splashscreen.png"));
    JApp::SplashScreen* splashScreen = new JApp::SplashScreen(pluginsInitializer, pic);

    // When plugins have loaded, display log viewer
    JApp::Core::Gui::LogViewer* v = new JApp::Core::Gui::LogViewer();
    QObject::connect(&pluginsInitializer, &JApp::PluginsInitializer::initialized, splashScreen, [splashScreen, v]{
        splashScreen->finish(v);
        v->show();
    });

    // Let's go
    splashScreen->show();
    pluginsInitializer.initialize();
}

void Launcher::shutdown()
{
    JApp::Logger& logger = JApp::Logger::instance();
    logger.shutdown();
}
