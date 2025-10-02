#include "QuxPlugin.h"
#include <JApp/Log.h>
#include <QThread>

using namespace Qux;

QString QuxPlugin::name() const
{
    return "QuxPlugin";
}

bool QuxPlugin::initialize()
{
    LOG_DEBUG() << "QuxPlugin initializing...";
    QThread::sleep(1);
    LOG_DEBUG() << "QuxPlugin initialized";
    return true;
}

void QuxPlugin::shutdown()
{
    LOG_DEBUG() << "QuxPlugin shutdown";
}
