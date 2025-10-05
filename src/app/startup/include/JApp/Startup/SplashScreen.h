#include <QSplashScreen>

namespace JApp {

class PluginManager;

class SplashScreen : public QSplashScreen
{
    Q_OBJECT

public:
    explicit SplashScreen(PluginManager* pluginManager, QPixmap pixmap);

protected:
    void drawContents(QPainter* painter) override;

private slots:
    void onLoadingProgressChanged(qreal progress);
    void onInitializationProgressChanged(qreal progress);

private:
    void initialize();

    PluginManager* m_pluginManager;
    qreal m_pluginsLoadingProgress;
    qreal m_pluginsInitializationProgress;
};

}
