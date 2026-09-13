#include "transferlistfilterswidgetitem.h"

#include <QCheckBox>
#include <QFont>
#include <QPainter>
#include <QString>
#include <QStyleOptionViewItem>
#include <QVBoxLayout>

#include "base/global.h"

namespace
{
    class ArrowCheckBox final : public QCheckBox
    {
    public:
        using QCheckBox::QCheckBox;

    private:
        void paintEvent(QPaintEvent *) override
        {
            QPainter painter {this};

            QStyleOptionViewItem indicatorOption;
            indicatorOption.initFrom(this);
            indicatorOption.rect = style()->subElementRect(QStyle::SE_CheckBoxIndicator, &indicatorOption, this);
            indicatorOption.state |= (QStyle::State_Children
                | (isChecked() ? QStyle::State_Open : QStyle::State_None));
            style()->drawPrimitive(QStyle::PE_IndicatorBranch, &indicatorOption, &painter, this);

            QStyleOptionButton labelOption;
            initStyleOption(&labelOption);
            labelOption.rect = style()->subElementRect(QStyle::SE_CheckBoxContents, &labelOption, this);
            style()->drawControl(QStyle::CE_CheckBoxLabel, &labelOption, &painter, this);
        }
    };
}

TransferListFiltersWidgetItem::TransferListFiltersWidgetItem(const QString &caption, QWidget *filterWidget, QWidget *parent)
    : QWidget(parent)
    , m_caption {new ArrowCheckBox(caption, this)}
    , m_filterWidget {filterWidget}
{
    setObjectName(u"modernSidebarSection"_s);
    setAttribute(Qt::WA_StyledBackground, true);

    QFont font;
    font.setBold(true);
    font.setPointSizeF(font.pointSizeF() + 0.8);
    m_caption->setFont(font);
    m_caption->setObjectName(u"modernSidebarHeader"_s);
    m_caption->setMinimumHeight(38);
    m_caption->setCursor(Qt::PointingHandCursor);

    m_filterWidget->setObjectName(u"modernSidebarBody"_s);
    m_filterWidget->setAttribute(Qt::WA_StyledBackground, true);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(12, 9, 12, 11);
    layout->setSpacing(7);
    layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    layout->addWidget(m_caption);
    layout->addWidget(m_filterWidget);

    m_filterWidget->setVisible(m_caption->isChecked());

    connect(m_caption, &QCheckBox::toggled, m_filterWidget, &QWidget::setVisible);
    connect(m_caption, &QCheckBox::toggled, this, &TransferListFiltersWidgetItem::toggled);
}

bool TransferListFiltersWidgetItem::isChecked() const
{
    return m_caption->isChecked();
}

void TransferListFiltersWidgetItem::setChecked(const bool value)
{
    m_caption->setChecked(value);
}
