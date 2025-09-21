#include "JApp/Core/Gui/LogLevelDelegate.h"
#include <JApp/LogFormatter.h>
#include <QPainter>

using namespace JApp::Core::Gui;

LogLevelDelegate::LogLevelDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

void LogLevelDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const {
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    
    QString level = index.data().toString();
    QColor bgColor = getColorForLogLevel(LogFormatter::logLevelFromString(level));
    
    painter->save();
    painter->fillRect(opt.rect, bgColor);
    
    painter->setPen(Qt::black);
    painter->drawText(opt.rect.adjusted(4, 0, -4, 0), 
                      Qt::AlignVCenter, level);
    painter->restore();
}

QColor LogLevelDelegate::getColorForLogLevel(const LogLevel &level) const {
    if (level == LogLevel::Critical)
        return QColor(255, 200, 200);
    else if (level == LogLevel::Warning)
        return QColor(255, 255, 200);
    else if (level == LogLevel::Info)
        return QColor(200, 255, 200);
    else if (level == LogLevel::Fatal)
        return QColor(200, 200, 255);
    else if (level == LogLevel::Debug)
        return QColor(200, 255, 255);
    else
        return QColor(240, 240, 240);
}
