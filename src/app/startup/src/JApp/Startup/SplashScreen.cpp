#include "JApp/Startup/SplashScreen.h"
#include <JApp/Plugins/PluginManager.h>
#include <JApp/Log.h>
#include <QPainter>

using namespace JApp;

SplashScreen::SplashScreen(PluginManager* pluginManager, QPixmap pixmap) : QSplashScreen(pixmap)
    , m_pluginManager(pluginManager)
    , m_pluginsLoadingProgress(0)
    , m_pluginsInitializationProgress(0)
    , m_pluginsLoadingMessage("")
    , m_pluginsInitializationMessage("")
{
    initialize();
}

void SplashScreen::drawContents(QPainter* painter)
{
    QSplashScreen::drawContents(painter);

    QRect rect1(20, height() - 70, width() - 40, 20);
    painter->fillRect(rect1, QColor(200, 200, 200, 100));
    painter->fillRect(QRect(rect1.x(), rect1.y(), rect1.width() * m_pluginsLoadingProgress, rect1.height()),
                      QColor(0, 150, 136));

    QString loadingText = m_pluginsLoadingMessage;
    QFont font = painter->font();
    painter->setFont(font);

    QRect textRect1(rect1.x() + 5, rect1.y(), rect1.width(), 20);
    painter->setPen(QColor(0, 0, 0, 160));
    painter->drawText(textRect1.translated(1, 1), Qt::AlignLeft | Qt::AlignVCenter, loadingText);
    painter->setPen(Qt::white);
    painter->drawText(textRect1, Qt::AlignLeft | Qt::AlignVCenter, loadingText);

    QRect rect2(20, height() - 40, width() - 40, 20);
    painter->fillRect(rect2, QColor(200, 200, 200, 100));
    painter->fillRect(QRect(rect2.x(), rect2.y(), rect2.width() * m_pluginsInitializationProgress, rect2.height()),
                      QColor(33, 150, 243));

    QString initText = m_pluginsInitializationMessage;
    QRect textRect2(rect2.x() + 5, rect2.y(), rect2.width(), 20);
    painter->setPen(QColor(0, 0, 0, 160));
    painter->drawText(textRect2.translated(1, 1), Qt::AlignLeft | Qt::AlignVCenter, initText);
    painter->setPen(Qt::white);
    painter->drawText(textRect2, Qt::AlignLeft | Qt::AlignVCenter, initText);
}

void SplashScreen::onLoadProgressChanged(qreal progress)
{
    m_pluginsLoadingProgress = progress;
    update();
}

void SplashScreen::onLoadMessageChanged(const QString &loadMessage)
{
    m_pluginsLoadingMessage = loadMessage;
    update();
}

void SplashScreen::onInitializationProgressChanged(qreal progress)
{
    m_pluginsInitializationProgress = progress;
    update();
}

void SplashScreen::onInitializationMessageChanged(const QString &initializationMessage)
{
    m_pluginsInitializationMessage = initializationMessage;
    update();
}

void SplashScreen::initialize()
{
    connect(m_pluginManager, &PluginManager::loadProgressChanged, this, &SplashScreen::onLoadProgressChanged);
    connect(m_pluginManager, &PluginManager::initializationProgressChanged, this, &SplashScreen::onInitializationProgressChanged);
    connect(m_pluginManager, &PluginManager::loadMessageChanged, this, &SplashScreen::onLoadMessageChanged);
    connect(m_pluginManager, &PluginManager::initializationMessageChanged, this, &SplashScreen::onInitializationMessageChanged);

}
