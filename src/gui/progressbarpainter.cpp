#include "progressbarpainter.h"

#include <QPainter>
#include <QPalette>
#include <QStyleOptionViewItem>

#include "base/global.h"
#include "gui/uithememanager.h"

ProgressBarPainter::ProgressBarPainter(QObject *parent)
    : QObject(parent)
{
    applyUITheme();
    connect(UIThemeManager::instance(), &UIThemeManager::themeChanged, this, &ProgressBarPainter::applyUITheme);
}

void ProgressBarPainter::paint(QPainter *painter, const QStyleOptionViewItem &option, const QString &text, const int progress, const QColor &color) const
{
    const QRect outer = option.rect.adjusted(6, 7, -6, -7);

    QColor chunk = color;
    if (!chunk.isValid())
        chunk = QColor("#79b530");
    if (!option.state.testFlag(QStyle::State_Enabled))
        chunk = QColor("#a9a9a9");

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);

    painter->setPen(QColor("#b8b8b8"));
    painter->setBrush(QColor("#eeeeee"));
    painter->drawRoundedRect(outer, 2, 2);

    const int bounded = qBound(0, progress, 100);
    if (bounded > 0)
    {
        QRect fill = outer.adjusted(1, 1, -1, -1);
        fill.setWidth((fill.width() * bounded) / 100);
        painter->setPen(Qt::NoPen);
        painter->setBrush(chunk);
        painter->drawRect(fill);
    }

    painter->setPen(QColor("#202020"));
    painter->setFont(option.font);
    painter->drawText(option.rect, Qt::AlignCenter, text);
    painter->restore();
}

void ProgressBarPainter::applyUITheme()
{
    m_chunkColor = UIThemeManager::instance()->getColor(u"ProgressBar"_s);
}
