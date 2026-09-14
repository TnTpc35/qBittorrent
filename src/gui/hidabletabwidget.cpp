/*
 * Bittorrent Client using Qt and libtorrent.
 * Copyright (C) 2006  Christophe Dumez <chris@qbittorrent.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "hidabletabwidget.h"

#include <QAction>
#include <QColor>
#include <QIcon>
#include <QPainter>
#include <QPainterPath>
#include <QPalette>
#include <QPixmap>
#include <QTabBar>
#include <QToolBar>

#ifdef Q_OS_MACOS
#include <QPaintEvent>
#include <QStyle>
#endif

namespace
{
    const char UTORRENT_STYLE[] = R"QSS(
QMainWindow { background:#f3f3f3; color:#222; }
QWidget { color:#222; }

QMenuBar {
    background:#f7f7f7; color:#202020; border:0;
    border-bottom:1px solid #d3d3d3; padding:1px 4px;
}
QMenuBar::item { background:transparent; padding:5px 9px; margin:0 1px; }
QMenuBar::item:selected { background:#e5e5e5; }
QMenu { background:#fff; color:#202020; border:1px solid #bdbdbd; padding:3px; }
QMenu::item { padding:6px 28px 6px 10px; margin:1px; }
QMenu::item:selected { background:#dceecb; color:#111; }
QMenu::separator { height:1px; background:#ddd; margin:4px 7px; }

QToolBar {
    background:#f8f8f8; border:0; border-bottom:1px solid #cecece;
    spacing:2px; padding:5px 7px;
}
QToolBar::separator { width:1px; background:#d0d0d0; margin:7px 5px; }
QToolButton {
    background:transparent; color:#222; border:1px solid transparent;
    border-radius:3px; padding:4px; margin:0; min-width:34px; min-height:34px;
}
QToolButton:hover { background:#eee; border-color:#d0d0d0; }
QToolButton:pressed, QToolButton:checked { background:#dceccf; border-color:#a8c98b; }

QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox {
    background:#fff; color:#222; border:1px solid #bfc3c6;
    border-radius:3px; padding:4px 7px; selection-background-color:#cfe5ff;
}
QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDoubleSpinBox:focus { border-color:#78b52f; }
QToolBar QLineEdit { min-width:245px; min-height:23px; margin:4px 3px; }
QComboBox::drop-down { width:22px; border:0; }

QTabWidget::pane { background:#fff; border:1px solid #c9c9c9; border-top:1px solid #bdbdbd; }
QTabBar { background:#ededed; }
QTabBar::tab {
    background:#e9e9e9; color:#353535; border:1px solid #c9c9c9;
    border-bottom:0; padding:6px 12px; margin:2px 1px 0 0; min-width:66px;
}
QTabBar::tab:hover { background:#f5f5f5; }
QTabBar::tab:selected { background:#fff; color:#202020; border-top:2px solid #79b530; padding-top:5px; }

QTreeView, QTableView, QListView, QListWidget, QTableWidget {
    background:#fff; alternate-background-color:#fafafa; color:#202020;
    border:0; outline:0; selection-background-color:#dcecff; selection-color:#111;
}
QTreeView::item, QTableView::item, QListView::item, QListWidget::item { border:0; padding:3px 5px; }
QTreeView::item:hover, QTableView::item:hover, QListView::item:hover, QListWidget::item:hover { background:#f1f6ec; }
QTreeView::item:selected, QTableView::item:selected, QListView::item:selected, QListWidget::item:selected { background:#dcecff; color:#111; }

QHeaderView { background:#f1f1f1; }
QHeaderView::section {
    background:#f1f1f1; color:#4b4b4b; border:0; border-right:1px solid #dedede;
    border-bottom:1px solid #c9c9c9; padding:6px 7px; min-height:20px; font-weight:500;
}
QHeaderView::section:hover { background:#e8e8e8; color:#222; }

QScrollBar:vertical { background:#f1f1f1; width:12px; margin:0; }
QScrollBar::handle:vertical { background:#b8b8b8; min-height:28px; border:2px solid #f1f1f1; border-radius:5px; }
QScrollBar::handle:vertical:hover { background:#9d9d9d; }
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background:transparent; border:0; height:0; }
QScrollBar:horizontal { background:#f1f1f1; height:12px; margin:0; }
QScrollBar::handle:horizontal { background:#b8b8b8; min-width:28px; border:2px solid #f1f1f1; border-radius:5px; }
QScrollBar::handle:horizontal:hover { background:#9d9d9d; }
QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal, QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal { background:transparent; border:0; width:0; }

QSplitter::handle { background:#cfcfcf; }
QSplitter::handle:horizontal { width:3px; }
QSplitter::handle:vertical { height:3px; }
QStatusBar { background:#f2f2f2; color:#444; border-top:1px solid #cfcfcf; min-height:24px; padding:1px 5px; }
QToolTip { background:#ffffe1; color:#202020; border:1px solid #8f8f8f; padding:4px 6px; }

QPushButton {
    background:#ececec; color:#292929; border:1px solid #c5c5c5;
    border-radius:2px; padding:4px 8px; min-height:20px;
}
QPushButton:hover { background:#f7f7f7; border-color:#aaa; }
QPushButton:pressed { background:#fff; border-color:#b9b9b9; border-top:2px solid #79b530; padding-top:3px; }
QGroupBox { background:#fff; color:#333; border:1px solid #d1d1d1; border-radius:2px; margin-top:10px; padding-top:8px; }
QGroupBox::title { subcontrol-origin:margin; left:8px; padding:0 4px; background:#fff; color:#444; font-weight:600; }
QLabel { color:#303030; }
QTextEdit, QPlainTextEdit { background:#fff; color:#202020; border:1px solid #c7c7c7; selection-background-color:#dcecff; }

QWidget#modernFiltersSidebar { background:#ededed; border-right:1px solid #c9c9c9; }
QWidget#modernFiltersSidebarContent { background:#ededed; }
QWidget#modernSidebarSection { background:transparent; border:0; }
QCheckBox#modernSidebarHeader { color:#484848; spacing:6px; padding:3px 5px; font-weight:600; }
QWidget#modernSidebarBody { background:transparent; }
QWidget#modernSidebarBody QAbstractItemView { background:transparent; border:0; color:#333; selection-background-color:#78b52f; selection-color:#fff; }
QWidget#modernSidebarBody QAbstractItemView::item { padding:5px 7px; }
QWidget#modernSidebarBody QAbstractItemView::item:hover { background:#dfe9d6; color:#202020; }
QWidget#modernSidebarBody QAbstractItemView::item:selected { background:#78b52f; color:#fff; }
)QSS";

    enum class Glyph { Add, Link, Remove, Start, Stop, Top, Up, Down, Bottom, Create, Settings, Lock };

    QIcon makeIcon(const Glyph glyph)
    {
        QPixmap px(32, 32);
        px.fill(Qt::transparent);
        QPainter p(&px);
        p.setRenderHint(QPainter::Antialiasing, true);
        const QColor green("#58aa2a");
        const QColor darkGreen("#3f8f1b");
        const QColor red("#c93632");
        const QColor dark("#4a4a4a");
        const QColor mid("#777777");
        const auto pen = [](const QColor &c, qreal w) { return QPen(c, w, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin); };

        switch (glyph)
        {
        case Glyph::Add:
            p.setPen(pen(darkGreen, 2)); p.setBrush(QColor("#eef7e8")); p.drawRect(QRectF(8, 5, 16, 22));
            p.setPen(pen(green, 3)); p.drawLine(QPointF(5, 21), QPointF(15, 21)); p.drawLine(QPointF(10, 16), QPointF(10, 26));
            break;
        case Glyph::Link:
            p.setBrush(Qt::NoBrush); p.setPen(pen(darkGreen, 3));
            p.drawRoundedRect(QRectF(4, 10, 12, 9), 4, 4); p.drawRoundedRect(QRectF(16, 10, 12, 9), 4, 4); p.drawLine(QPointF(12, 14.5), QPointF(20, 14.5));
            break;
        case Glyph::Remove:
            p.setPen(pen(red, 3.4)); p.drawLine(QPointF(7, 7), QPointF(25, 25)); p.drawLine(QPointF(25, 7), QPointF(7, 25));
            break;
        case Glyph::Start:
        {
            p.setPen(Qt::NoPen); p.setBrush(green); QPainterPath path; path.moveTo(8, 5); path.lineTo(26, 16); path.lineTo(8, 27); path.closeSubpath(); p.drawPath(path);
            break;
        }
        case Glyph::Stop:
            p.setPen(Qt::NoPen); p.setBrush(dark); p.drawRect(QRectF(8, 8, 16, 16));
            break;
        case Glyph::Top:
            p.setPen(pen(mid, 2.8)); p.drawLine(QPointF(7,7), QPointF(25,7)); p.drawLine(QPointF(16,23), QPointF(16,10)); p.drawLine(QPointF(10,16), QPointF(16,10)); p.drawLine(QPointF(22,16), QPointF(16,10));
            break;
        case Glyph::Up:
            p.setPen(pen(mid, 3)); p.drawLine(QPointF(8,20), QPointF(16,11)); p.drawLine(QPointF(16,11), QPointF(24,20));
            break;
        case Glyph::Down:
            p.setPen(pen(mid, 3)); p.drawLine(QPointF(8,12), QPointF(16,21)); p.drawLine(QPointF(16,21), QPointF(24,12));
            break;
        case Glyph::Bottom:
            p.setPen(pen(mid, 2.8)); p.drawLine(QPointF(7,25), QPointF(25,25)); p.drawLine(QPointF(16,9), QPointF(16,22)); p.drawLine(QPointF(10,16), QPointF(16,22)); p.drawLine(QPointF(22,16), QPointF(16,22));
            break;
        case Glyph::Create:
            p.setPen(pen(darkGreen, 2)); p.setBrush(QColor("#eef7e8")); p.drawRect(QRectF(6,5,17,22)); p.setPen(pen(green, 3)); p.drawLine(QPointF(10,21), QPointF(22,9));
            break;
        case Glyph::Settings:
            p.setBrush(Qt::NoBrush); p.setPen(pen(dark, 2.5)); p.drawEllipse(QRectF(9,9,14,14)); p.drawEllipse(QRectF(13,13,6,6));
            p.drawLine(QPointF(16,4), QPointF(16,9)); p.drawLine(QPointF(16,23), QPointF(16,28)); p.drawLine(QPointF(4,16), QPointF(9,16)); p.drawLine(QPointF(23,16), QPointF(28,16));
            break;
        case Glyph::Lock:
            p.setBrush(QColor("#efb32d")); p.setPen(pen(dark, 2)); p.drawRoundedRect(QRectF(8,14,16,13), 2, 2); p.setBrush(Qt::NoBrush); p.drawArc(QRectF(10,5,12,16), 0, 180 * 16);
            break;
        }
        return QIcon(px);
    }

    void configure(QWidget *parent)
    {
        if (!parent) return;

        QPalette pal = parent->palette();
        pal.setColor(QPalette::Window, QColor("#f3f3f3"));
        pal.setColor(QPalette::WindowText, QColor("#242424"));
        pal.setColor(QPalette::Base, QColor("#ffffff"));
        pal.setColor(QPalette::AlternateBase, QColor("#fafafa"));
        pal.setColor(QPalette::Text, QColor("#202020"));
        pal.setColor(QPalette::Button, QColor("#ececec"));
        pal.setColor(QPalette::ButtonText, QColor("#292929"));
        pal.setColor(QPalette::Highlight, QColor("#dcecff"));
        pal.setColor(QPalette::HighlightedText, QColor("#111111"));
        parent->setPalette(pal);
        parent->setStyleSheet(QString::fromLatin1(UTORRENT_STYLE));

        auto *toolbar = parent->findChild<QToolBar *>(u"toolBar"_s);
        if (!toolbar) return;
        toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);
        toolbar->setIconSize(QSize(26, 26));
        toolbar->setMinimumHeight(52);

        for (QAction *action : toolbar->actions())
        {
            const QString name = action->objectName();
            if (name == u"actionOpen") action->setIcon(makeIcon(Glyph::Add));
            else if (name == u"actionDownloadFromURL") action->setIcon(makeIcon(Glyph::Link));
            else if (name == u"actionDelete") action->setIcon(makeIcon(Glyph::Remove));
            else if (name == u"actionStart") action->setIcon(makeIcon(Glyph::Start));
            else if (name == u"actionStop") action->setIcon(makeIcon(Glyph::Stop));
            else if (name == u"actionTopQueuePos") action->setIcon(makeIcon(Glyph::Top));
            else if (name == u"actionIncreaseQueuePos") action->setIcon(makeIcon(Glyph::Up));
            else if (name == u"actionDecreaseQueuePos") action->setIcon(makeIcon(Glyph::Down));
            else if (name == u"actionBottomQueuePos") action->setIcon(makeIcon(Glyph::Bottom));
            else if (name == u"actionCreateTorrent") action->setIcon(makeIcon(Glyph::Create));
            else if (name == u"actionOptions") action->setIcon(makeIcon(Glyph::Settings));
            else if (name == u"actionLock") action->setIcon(makeIcon(Glyph::Lock));
        }
    }
}

HidableTabWidget::HidableTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    configure(parent);
    setDocumentMode(true);
    tabBar()->setExpanding(false);
    tabBar()->setUsesScrollButtons(true);
    tabBar()->setIconSize(QSize(16, 16));
    tabBar()->setFocusPolicy(Qt::NoFocus);
}

void HidableTabWidget::tabInserted(const int index)
{
    QTabWidget::tabInserted(index);
    tabsCountChanged();
}

void HidableTabWidget::tabRemoved(const int index)
{
    QTabWidget::tabRemoved(index);
    tabsCountChanged();
}

void HidableTabWidget::tabsCountChanged()
{
    const qsizetype tabsCount = count();
    tabBar()->setVisible(tabsCount != 1);
    tabBar()->setFocusPolicy((tabsCount > 1) ? Qt::StrongFocus : Qt::NoFocus);
}

#ifdef Q_OS_MACOS
void HidableTabWidget::paintEvent(QPaintEvent *event)
{
    if (!style()->inherits("QMacStyle"))
        QTabWidget::paintEvent(event);
}
#endif
