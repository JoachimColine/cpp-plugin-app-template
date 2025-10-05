#include "JApp/Startup/SplashScreen.h"
#include <JApp/Plugins/PluginManager.h>
#include <JApp/Log.h>
#include <QPainter>


using namespace JApp;

SplashScreen::SplashScreen(PluginManager* pluginManager, QPixmap pixmap) : QSplashScreen(pixmap)
    , m_pluginManager(pluginManager)
{
    initialize();
}

void SplashScreen::drawContents(QPainter* painter)
{
    QSplashScreen::drawContents(painter);

    // Draw first progress bar
    QRect rect1(20, height() - 70, width() - 40, 20);
    painter->fillRect(rect1, QColor(200, 200, 200, 100));
    painter->fillRect(QRect(rect1.x(), rect1.y(), rect1.width() * m_pluginsLoadingProgress, rect1.height()),
                     QColor(0, 150, 136));

    // Draw second progress bar
    QRect rect2(20, height() - 40, width() - 40, 20);
    painter->fillRect(rect2, QColor(200, 200, 200, 100));
    painter->fillRect(QRect(rect2.x(), rect2.y(), rect2.width() * m_pluginsInitializationProgress, rect2.height()),
                     QColor(33, 150, 243));

}

void SplashScreen::onLoadingProgressChanged(qreal progress)
{
    m_pluginsLoadingProgress = progress;
    update();
}

void SplashScreen::onInitializationProgressChanged(qreal progress)
{
    m_pluginsInitializationProgress = progress;
    update();
}

void SplashScreen::initialize()
{
    connect(m_pluginManager, &PluginManager::loadingProgressChanged, this, &SplashScreen::onLoadingProgressChanged);
    connect(m_pluginManager, &PluginManager::initializationProgressChanged, this, &SplashScreen::onInitializationProgressChanged);
    connect(m_pluginManager, &PluginManager::pluginsLoaded, this, &SplashScreen::close);
}
