#include <QSplashScreen>

namespace JApp {

class PluginsInitializer;

class SplashScreen : public QSplashScreen
{
    Q_OBJECT

public:
    explicit SplashScreen(PluginsInitializer& pluginsInitializer, QPixmap pixmap);

protected:
    void drawContents(QPainter* painter) override;

private slots:
    void onLoadProgressChanged(qreal progress);
    void onLoadMessageChanged(const QString& loadMessage);
    void onInitializationProgressChanged(qreal progress);
    void onInitializationMessageChanged(const QString& initializationMessage);

private:
    void initialize();

    PluginsInitializer& m_pluginsInitializer;
    qreal m_pluginsLoadingProgress;
    qreal m_pluginsInitializationProgress;
    QString m_pluginsLoadingMessage;
    QString m_pluginsInitializationMessage;
};

}
