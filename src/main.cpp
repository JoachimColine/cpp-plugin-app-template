#include <QApplication>
#include <JApp/Log.h>
#include <JApp/Startup/Launcher.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    JApp::Launcher launcher(app);
    launcher.launch();

    LOG_INFO() << "Application enters main event loop";
    int appExitCode = app.exec();

    LOG_INFO() << "Application exited with code: " << appExitCode;
    launcher.shutdown();

    return appExitCode;
}

