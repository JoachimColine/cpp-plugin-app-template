#pragma once

#include <JApp/Core/Models/LogModel.h>
#include <JApp/Logger.h>
#include <QWidget>
#include <QListView>
#include <QVBoxLayout>
#include <QLabel>

namespace JApp::Core::Gui {

class LogViewer : public QWidget
{
    Q_OBJECT

public:
    explicit LogViewer(QWidget *parent = nullptr,
                       JApp::Logger* logger = &JApp::Logger::instance());
    ~LogViewer() override;

private:
    void initialize();
    void buildWidget();

    QVBoxLayout* m_mainLayout;
    QLabel* m_headerLabel;
    QListView* m_logListView;
    JApp::Logger* m_logger;
    JApp::Core::Models::LogModel *m_logModel;
};

}
