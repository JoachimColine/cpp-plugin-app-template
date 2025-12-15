#include <QApplication>

namespace JApp {

class Launcher
{

public:
    explicit Launcher(QApplication& application);

    void launch();
    void shutdown();

private:
    QString loadStyleSheet();

private:
    QApplication& m_application;
};

}
