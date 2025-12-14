#include <QApplication>
#include <QFile>
#include <QScreen>
#include <QSysInfo>
#include <JApp/Logger.h>
#include <JApp/Log.h>
#include <JApp/Core/Gui/LogViewer.h>
#include <JApp/Plugins/PluginManager.h>
#include <JApp/Startup/SplashScreen.h>

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

    // Launch the app. Later, this will be wrapped inside a dedicated JApp::Launcher class.

    // Instantiate plugin manager
    JApp::PluginManager* pluginManager = new JApp::PluginManager(QCoreApplication::applicationDirPath() + "/plugins");

    // Instantiate splashscreen to monitor plugins loading
    QPixmap pic(QString(":/splashscreen.png"));
    JApp::SplashScreen* splashScreen = new JApp::SplashScreen(pluginManager, pic);

    // When plugins have loaded, display log viewer
    JApp::Core::Gui::LogViewer* v = new JApp::Core::Gui::LogViewer();
    QObject::connect(pluginManager, &JApp::PluginManager::loadFinished, splashScreen, [splashScreen, v]{
        splashScreen->finish(v);
        v->show();
    });

    // Let's go
    splashScreen->show();
    pluginManager->load();

    // Execute the application
    int appExitCode = app.exec();
    LOG_INFO() << "Application exited with code: " << appExitCode;

    // Gracefully stop logger
    logger.shutdown();

    return appExitCode;
}

