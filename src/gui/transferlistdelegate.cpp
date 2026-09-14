/*
 * Bittorrent Client using Qt and libtorrent.
 * Copyright (C) 2025  Vladimir Golovnev <glassez@yandex.ru>
 * Copyright (C) 2006  Christophe Dumez <chris@qbittorrent.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "transferlistdelegate.h"

#include <QModelIndex>

#include "base/preferences.h"
#include "transferlistmodel.h"

namespace
{
    constexpr int UTORRENT_ROW_HEIGHT = 28;
}

TransferListDelegate::TransferListDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QSize TransferListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (m_nameColHeight == -1)
    {
        const QModelIndex nameColumn = index.sibling(index.row(), TransferListModel::TR_NAME);
        m_nameColHeight = QStyledItemDelegate::sizeHint(option, nameColumn).height();
    }

    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(std::max({m_nameColHeight, size.height(), UTORRENT_ROW_HEIGHT}));
    return size;
}

void TransferListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch (index.column())
    {
    case TransferListModel::TR_PROGRESS:
        {
            using namespace BitTorrent;
            const auto isEnableState = [](const TorrentState state) -> bool
            {
                switch (state)
                {
                case TorrentState::Error:
                case TorrentState::StoppedDownloading:
                case TorrentState::Unknown:
                    return false;
                default:
                    return true;
                }
            };

            const int progress = static_cast<int>(index.data(TransferListModel::UnderlyingDataRole).toReal());
            const QModelIndex statusIndex = index.siblingAtColumn(TransferListModel::TR_STATUS);
            const auto torrentState = statusIndex.data(TransferListModel::UnderlyingDataRole).value<TorrentState>();

            QStyleOptionViewItem customOption {option};
            customOption.state.setFlag(QStyle::State_Enabled, isEnableState(torrentState));
            const QColor color = Preferences::instance()->getProgressBarFollowsTextColor()
                ? index.data(Qt::ForegroundRole).value<QColor>() : QColor();
            m_progressBarPainter.paint(painter, customOption, index.data().toString(), progress, color);
        }
        break;
    default:
        {
            QStyleOptionViewItem customOption {option};
            customOption.rect = option.rect.adjusted(3, 0, -3, 0);
            QStyledItemDelegate::paint(painter, customOption, index);
        }
        break;
    }
}
