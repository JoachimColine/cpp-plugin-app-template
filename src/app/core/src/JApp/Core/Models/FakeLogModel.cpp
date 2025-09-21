#include "JApp/Core/Models/FakeLogModel.h"
#include <QTimer>

namespace JApp::Core::Models {

FakeLogModel::FakeLogModel(QObject *parent)
    : LogModel(parent)
{
    m_timer = new QTimer(this);
    m_timer->setSingleShot(false);
    m_timer->setInterval(1000);
    connect(m_timer, &QTimer::timeout, this, [this](){
        addLog(Log{QDateTime::currentDateTime(),
                    "Fake category",
                    LogLevel::Info,
                    "FakeFile.cpp",
                    "fakeFunction",
                    0,
                    "A fake message",
                   "Fake thread id" }
        );
    });
    m_timer->start();
}

FakeLogModel::~FakeLogModel()
{
}

}
