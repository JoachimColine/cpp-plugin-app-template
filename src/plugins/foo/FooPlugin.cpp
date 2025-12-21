#include "FooPlugin.h"
#include <JApp/Log.h>
#include <QThread>

using namespace Foo;

QString FooPlugin::name() const
{
    return "FooPlugin";
}

bool FooPlugin::initialize(const JApp::Core::Context& context)
{
    LOG_DEBUG() << "FooPlugin initializing...";
    QThread::sleep(1);
    LOG_DEBUG() << "FooPlugin initialized";
    return true;
}

void FooPlugin::shutdown()
{
    LOG_DEBUG() << "FooPlugin shutdown";
}
