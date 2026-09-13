#include "progressbarpainter.h"

#include <algorithm>

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
    const QRect outer = option.rect.adjusted(10, 15, -10, -15);
    const qreal radius = outer.height() / 2.0;

    QColor track = option.palette.color(QPalette::Mid);
    track.setAlpha(150);

    QColor chunk = color;
    if (!chunk.isValid())
        chunk = m_chunkColor.isValid() ? m_chunkColor : QColor(63, 170, 75);

    if (!option.state.testFlag(QStyle::State_Enabled))
        chunk = option.palette.color(QPalette::Disabled, QPalette::Highlight);

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::NoPen);
    painter->setBrush(track);
    painter->drawRoundedRect(outer, radius, radius);

    const qreal fraction = qBound(0, progress, 100) / 100.0;
    if (fraction > 0.0)
    {
        QRectF fill = outer;
        fill.setWidth(std::max<qreal>(outer.height(), outer.width() * fraction));
        painter->setBrush(chunk);
        painter->drawRoundedRect(fill, radius, radius);
    }

    painter->setPen(option.palette.color(QPalette::Text));
    QFont font = option.font;
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(option.rect, Qt::AlignCenter, text);
    painter->restore();
}

void ProgressBarPainter::applyUITheme()
{
    m_chunkColor = UIThemeManager::instance()->getColor(u"ProgressBar"_s);
}
