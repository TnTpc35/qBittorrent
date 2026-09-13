#include "transferlistfilterswidget.h"

#include <QIcon>
#include <QListWidgetItem>
#include <QMenu>
#include <QScrollArea>
#include <QUrl>
#include <QVBoxLayout>

#include "base/bittorrent/session.h"
#include "base/global.h"
#include "base/preferences.h"
#include "base/utils/compare.h"
#include "transferlistfilters/categoryfilterwidget.h"
#include "transferlistfilters/statusfilterwidget.h"
#include "transferlistfilters/tagfilterwidget.h"
#include "transferlistfilters/trackersfilterwidget.h"
#include "transferlistfilters/trackerstatusfilterwidget.h"
#include "transferlistfilterswidgetitem.h"
#include "transferlistwidget.h"
#include "utils.h"

TransferListFiltersWidget::TransferListFiltersWidget(QWidget *parent, TransferListWidget *transferList, const bool downloadFavicon)
    : QWidget(parent)
    , m_transferList {transferList}
{
    setBackgroundRole(QPalette::Base);
    setMinimumWidth(245);
    setObjectName(u"modernFiltersSidebar"_s);
    setAttribute(Qt::WA_StyledBackground, true);

    Preferences *const pref = Preferences::instance();

    auto *mainWidget = new QWidget;
    mainWidget->setObjectName(u"modernFiltersSidebarContent"_s);
    mainWidget->setAttribute(Qt::WA_StyledBackground, true);
    auto *mainWidgetLayout = new QVBoxLayout(mainWidget);
    mainWidgetLayout->setContentsMargins(8, 10, 8, 10);
    mainWidgetLayout->setSpacing(10);
    mainWidgetLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    {
        auto *statusFilterWidget = new StatusFilterWidget(this, transferList);
        auto *item = new TransferListFiltersWidgetItem(tr("Status"), statusFilterWidget, this);
        item->setChecked(pref->getStatusFilterState());
        connect(item, &TransferListFiltersWidgetItem::toggled, statusFilterWidget, &StatusFilterWidget::toggleFilter);
        connect(item, &TransferListFiltersWidgetItem::toggled, pref, &Preferences::setStatusFilterState);
        statusFilterWidget->toggleFilter(item->isChecked());
        mainWidgetLayout->addWidget(item);
    }

    {
        auto *categoryFilterWidget = new CategoryFilterWidget(this);
        connect(categoryFilterWidget, &CategoryFilterWidget::actionDeleteTorrentsTriggered
                , transferList, &TransferListWidget::deleteVisibleTorrents);
        connect(categoryFilterWidget, &CategoryFilterWidget::actionStopTorrentsTriggered
                , transferList, &TransferListWidget::stopVisibleTorrents);
        connect(categoryFilterWidget, &CategoryFilterWidget::actionStartTorrentsTriggered
                , transferList, &TransferListWidget::startVisibleTorrents);
        connect(categoryFilterWidget, &CategoryFilterWidget::categoryChanged
                , transferList, &TransferListWidget::applyCategoryFilter);

        auto *item = new TransferListFiltersWidgetItem(tr("Categories"), categoryFilterWidget, this);
        item->setChecked(pref->getCategoryFilterState());
        connect(item, &TransferListFiltersWidgetItem::toggled, this, [this, categoryFilterWidget](const bool enabled)
        {
            m_transferList->applyCategoryFilter(enabled ? categoryFilterWidget->currentCategory() : QString());
        });
        connect(item, &TransferListFiltersWidgetItem::toggled, pref, &Preferences::setCategoryFilterState);
        mainWidgetLayout->addWidget(item);
    }

    {
        auto *tagFilterWidget = new TagFilterWidget(this);
        connect(tagFilterWidget, &TagFilterWidget::actionDeleteTorrentsTriggered
                , transferList, &TransferListWidget::deleteVisibleTorrents);
        connect(tagFilterWidget, &TagFilterWidget::actionStopTorrentsTriggered
                , transferList, &TransferListWidget::stopVisibleTorrents);
        connect(tagFilterWidget, &TagFilterWidget::actionStartTorrentsTriggered
                , transferList, &TransferListWidget::startVisibleTorrents);
        connect(tagFilterWidget, &TagFilterWidget::tagChanged
                , transferList, &TransferListWidget::applyTagFilter);

        auto *item = new TransferListFiltersWidgetItem(tr("Tags"), tagFilterWidget, this);
        item->setChecked(pref->getTagFilterState());
        connect(item, &TransferListFiltersWidgetItem::toggled, this, [this, tagFilterWidget](const bool enabled)
        {
            m_transferList->applyTagFilter(enabled ? tagFilterWidget->currentTag() : std::nullopt);
        });
        connect(item, &TransferListFiltersWidgetItem::toggled, pref, &Preferences::setTagFilterState);
        mainWidgetLayout->addWidget(item);
    }

    const int trackerStatusItemPos = mainWidgetLayout->count();

    {
        m_trackersFilterWidget = new TrackersFilterWidget(this, transferList, downloadFavicon);

        auto *item = new TransferListFiltersWidgetItem(tr("Trackers"), m_trackersFilterWidget, this);
        item->setChecked(pref->getTrackerFilterState());
        connect(item, &TransferListFiltersWidgetItem::toggled, m_trackersFilterWidget, &TrackersFilterWidget::toggleFilter);
        connect(item, &TransferListFiltersWidgetItem::toggled, pref, &Preferences::setTrackerFilterState);
        m_trackersFilterWidget->toggleFilter(item->isChecked());
        mainWidgetLayout->addWidget(item);
    }

    auto *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setWidget(mainWidget);

    auto *vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addWidget(scroll);

    const auto createTrackerStatusItem = [this, mainWidgetLayout, trackerStatusItemPos, pref]
    {
        auto *trackerStatusFilterWidget = new TrackerStatusFilterWidget(this, m_transferList);
        auto *item = new TransferListFiltersWidgetItem(tr("Tracker status"), trackerStatusFilterWidget, this);
        item->setChecked(pref->getTrackerStatusFilterState());
        connect(item, &TransferListFiltersWidgetItem::toggled, trackerStatusFilterWidget, &TrackerStatusFilterWidget::toggleFilter);
        connect(item, &TransferListFiltersWidgetItem::toggled, pref, &Preferences::setTrackerStatusFilterState);
        trackerStatusFilterWidget->toggleFilter(item->isChecked());
        mainWidgetLayout->insertWidget(trackerStatusItemPos, item);
    };

    const auto removeTrackerStatusItem = [mainWidgetLayout, trackerStatusItemPos]
    {
        QLayoutItem *layoutItem = mainWidgetLayout->takeAt(trackerStatusItemPos);
        delete layoutItem->widget();
        delete layoutItem;
    };

    m_useSeparateTrackerStatusFilter = pref->useSeparateTrackerStatusFilter();
    if (m_useSeparateTrackerStatusFilter)
        createTrackerStatusItem();

    connect(pref, &Preferences::changed, this, [this, pref, createTrackerStatusItem, removeTrackerStatusItem]
    {
        if (m_useSeparateTrackerStatusFilter == pref->useSeparateTrackerStatusFilter())
            return;

        m_useSeparateTrackerStatusFilter = !m_useSeparateTrackerStatusFilter;
        if (m_useSeparateTrackerStatusFilter)
            createTrackerStatusItem();
        else
            removeTrackerStatusItem();
    });
}

void TransferListFiltersWidget::setDownloadTrackerFavicon(bool value)
{
    m_trackersFilterWidget->setDownloadTrackerFavicon(value);
}
