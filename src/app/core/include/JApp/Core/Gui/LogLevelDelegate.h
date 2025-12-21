#pragma once
#include <QStyledItemDelegate>
#include <JApp/LogDefinitions.h>

namespace JApp::Core::Gui {

class LogLevelDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit LogLevelDelegate(QObject *parent = nullptr);
    
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
               
private:
    QColor getColorForLogLevel(const LogLevel &level) const;
    void applyFontStyleForLogLevel(QFont& font, const LogLevel &level) const;
};

}
