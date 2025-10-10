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
    void onLoadingMessageChanged(const QString& loadingMessage);
    void onInitializationProgressChanged(qreal progress);
    void onInitializationMessageChanged(const QString& initializationMessage);

private:
    void initialize();

    PluginManager* m_pluginManager;
    qreal m_pluginsLoadingProgress;
    qreal m_pluginsInitializationProgress;
    QString m_pluginsLoadingMessage;
    QString m_pluginsInitializationMessage;
};

}
