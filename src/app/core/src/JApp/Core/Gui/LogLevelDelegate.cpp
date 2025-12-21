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
    LogLevel logLevel = LogFormatter::logLevelFromString(level);
    QColor bgColor = getColorForLogLevel(logLevel);
    painter->save();
    painter->fillRect(opt.rect, bgColor);

    QFont font = opt.font;
    applyFontStyleForLogLevel(font, logLevel);
    painter->setFont(font);
    painter->setPen(Qt::black);
    painter->drawText(opt.rect.adjusted(4, 0, -4, 0), Qt::AlignVCenter, level);

    painter->restore();
}

QColor LogLevelDelegate::getColorForLogLevel(const LogLevel &level) const {
    QColor c;
    switch (level) {
    case LogLevel::Critical:
        c =  QColor(255, 200, 200);
        break;
    case LogLevel::Warning:
        c =  QColor(255, 255, 200);
        break;
    case LogLevel::Info:
        c =  QColor(200, 255, 200);
        break;
    case LogLevel::Fatal:
        c =  QColor(200, 200, 255);
        break;
    case LogLevel::Debug:
        c =  QColor(200, 255, 255);
        break;
    default:
        c = QColor(240, 240, 240);
        break;
    }

    return c;
}

void LogLevelDelegate::applyFontStyleForLogLevel(QFont &font, const LogLevel &level) const {
    switch (level) {
    case LogLevel::Fatal:
    case LogLevel::Critical:
    case LogLevel::Warning:
        font.setBold(true);
        font.setItalic(false);
        break;
    case LogLevel::Debug:
        font.setBold(false);
        font.setItalic(true);
        break;
    default:
        font.setBold(false);
        font.setItalic(false);
        break;
    }
}
