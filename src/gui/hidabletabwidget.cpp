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
#include <QApplication>
#include <QIcon>
#include <QPainter>
#include <QPainterPath>
#include <QPalette>
#include <QPixmap>
#include <QTabBar>
#include <QTimer>
#include <QToolBar>

#ifdef Q_OS_MACOS
#include <QPaintEvent>
#include <QStyle>
#endif

namespace
{
    const char UTORRENT_STYLE[] = R"QSS(
QMainWindow, QDialog { background:#f4f4f4; color:#202020; }
QMenuBar { background:#f8f8f8; color:#202020; border:0; border-bottom:1px solid #cfcfcf; padding:1px 4px; }
QMenuBar::item { background:transparent; padding:5px 9px; }
QMenuBar::item:selected { background:#e7e7e7; }
QMenu { background:#fff; color:#202020; border:1px solid #b9b9b9; padding:3px; }
QMenu::item { padding:6px 28px 6px 10px; }
QMenu::item:selected { background:#dceecb; color:#111; }
QMenu::separator { height:1px; background:#ddd; margin:4px 7px; }

QToolBar { background:#f8f8f8; border:0; border-bottom:1px solid #c9c9c9; spacing:2px; padding:4px 7px; }
QToolBar::separator { width:1px; background:#d0d0d0; margin:7px 5px; }
QToolButton { background:transparent; border:1px solid transparent; border-radius:2px; padding:3px; min-width:34px; min-height:34px; }
QToolButton:hover { background:#ededed; border-color:#cecece; }
QToolButton:pressed, QToolButton:checked { background:#dceecf; border-color:#9fbe82; }

QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox { background:#fff; color:#202020; border:1px solid #b8bdc1; border-radius:2px; padding:4px 6px; selection-background-color:#cfe5ff; }
QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDoubleSpinBox:focus { border-color:#78b52f; }
QToolBar QLineEdit { min-width:240px; min-height:22px; margin:3px; }

QTreeView, QTableView, QListView, QListWidget, QTableWidget { background:#fff; alternate-background-color:#fafafa; color:#202020; border:0; outline:0; selection-background-color:#dcecff; selection-color:#111; }
QTreeView::item, QTableView::item, QListView::item, QListWidget::item { padding:2px 4px; }
QTreeView::item:hover, QTableView::item:hover, QListView::item:hover, QListWidget::item:hover { background:#eef5e8; }
QTreeView::item:selected, QTableView::item:selected, QListView::item:selected, QListWidget::item:selected { background:#dcecff; color:#111; }

QHeaderView { background:#f0f0f0; }
QHeaderView::section { background:#f0f0f0; color:#404040; border:0; border-right:1px solid #ddd; border-bottom:1px solid #c8c8c8; padding:5px 6px; min-height:19px; }
QHeaderView::section:hover { background:#e7e7e7; }

QTabWidget::pane { background:#fff; border:1px solid #c8c8c8; }
QTabBar { background:#ededed; }
QTabBar::tab { background:#e9e9e9; color:#333; border:1px solid #c8c8c8; border-bottom:0; padding:5px 11px; margin:2px 1px 0 0; min-width:62px; }
QTabBar::tab:selected { background:#fff; color:#202020; border-top:2px solid #79b530; padding-top:4px; }

QPushButton { background:#ececec; color:#292929; border:1px solid #c4c4c4; border-radius:2px; padding:4px 8px; min-height:20px; }
QPushButton:hover { background:#f7f7f7; }
QPushButton:pressed { background:#fff; border-top:2px solid #79b530; padding-top:3px; }
QGroupBox { background:#fff; color:#333; border:1px solid #d0d0d0; border-radius:2px; margin-top:10px; padding-top:8px; }
QGroupBox::title { subcontrol-origin:margin; left:8px; padding:0 4px; background:#fff; }
QLabel { color:#303030; }
QTextEdit, QPlainTextEdit { background:#fff; color:#202020; border:1px solid #c7c7c7; }

QStatusBar { background:#f1f1f1; color:#444; border-top:1px solid #c9c9c9; min-height:23px; }
QSplitter::handle { background:#cfcfcf; }
QSplitter::handle:horizontal { width:3px; }
QSplitter::handle:vertical { height:3px; }
QScrollBar:vertical { background:#f0f0f0; width:12px; }
QScrollBar::handle:vertical { background:#b5b5b5; min-height:28px; border:2px solid #f0f0f0; border-radius:5px; }
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background:transparent; border:0; height:0; }
QScrollBar:horizontal { background:#f0f0f0; height:12px; }
QScrollBar::handle:horizontal { background:#b5b5b5; min-width:28px; border:2px solid #f0f0f0; border-radius:5px; }
QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal, QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal { background:transparent; border:0; width:0; }

QWidget#modernFiltersSidebar, QWidget#modernFiltersSidebarContent { background:#eeeeee; }
QWidget#modernFiltersSidebar { border-right:1px solid #c8c8c8; }
QWidget#modernSidebarSection { background:transparent; border:0; }
QCheckBox#modernSidebarHeader { color:#444; spacing:5px; padding:3px 4px; font-weight:600; }
QWidget#modernSidebarBody { background:transparent; }
QWidget#modernSidebarBody QAbstractItemView { background:transparent; border:0; color:#303030; selection-background-color:#79b530; selection-color:#fff; }
QWidget#modernSidebarBody QAbstractItemView::item { padding:4px 6px; }
QWidget#modernSidebarBody QAbstractItemView::item:selected { background:#79b530; color:#fff; }
)QSS";

    enum class Glyph { Add, Link, Remove, Start, Stop, Up, Down, Settings };

    QIcon makeIcon(const Glyph glyph)
    {
        QPixmap px(30, 30);
        px.fill(Qt::transparent);
        QPainter p(&px);
        p.setRenderHint(QPainter::Antialiasing, true);
        const QColor green("#62ad2f");
        const QColor darkGreen("#3f8c1d");
        const QColor red("#c63c34");
        const QColor gray("#555555");
        const auto pen = [](const QColor &c, const qreal w) { return QPen(c, w, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin); };

        switch (glyph)
        {
        case Glyph::Add:
            p.setPen(pen(darkGreen, 2)); p.setBrush(QColor("#f4faef")); p.drawRect(QRectF(8,4,15,21));
            p.setPen(pen(green, 3)); p.drawLine(QPointF(5,20), QPointF(15,20)); p.drawLine(QPointF(10,15), QPointF(10,25));
            break;
        case Glyph::Link:
            p.setBrush(Qt::NoBrush); p.setPen(pen(darkGreen, 2.8));
            p.drawRoundedRect(QRectF(3,10,12,8), 4, 4); p.drawRoundedRect(QRectF(15,10,12,8), 4, 4); p.drawLine(QPointF(11,14), QPointF(19,14));
            break;
        case Glyph::Remove:
            p.setPen(pen(red, 3.2)); p.drawLine(QPointF(7,7), QPointF(23,23)); p.drawLine(QPointF(23,7), QPointF(7,23));
            break;
        case Glyph::Start:
        {
            p.setPen(Qt::NoPen); p.setBrush(green); QPainterPath path; path.moveTo(8,5); path.lineTo(24,15); path.lineTo(8,25); path.closeSubpath(); p.drawPath(path);
            break;
        }
        case Glyph::Stop:
            p.setPen(Qt::NoPen); p.setBrush(gray); p.drawRect(QRectF(8,8,14,14));
            break;
        case Glyph::Up:
            p.setPen(pen(gray, 2.8)); p.drawLine(QPointF(7,19), QPointF(15,11)); p.drawLine(QPointF(15,11), QPointF(23,19));
            break;
        case Glyph::Down:
            p.setPen(pen(gray, 2.8)); p.drawLine(QPointF(7,11), QPointF(15,19)); p.drawLine(QPointF(15,19), QPointF(23,11));
            break;
        case Glyph::Settings:
            p.setBrush(Qt::NoBrush); p.setPen(pen(gray, 2.4)); p.drawEllipse(QRectF(8,8,14,14)); p.drawEllipse(QRectF(12,12,6,6));
            p.drawLine(QPointF(15,3), QPointF(15,8)); p.drawLine(QPointF(15,22), QPointF(15,27)); p.drawLine(QPointF(3,15), QPointF(8,15)); p.drawLine(QPointF(22,15), QPointF(27,15));
            break;
        }
        return QIcon(px);
    }

    void configure(QWidget *window)
    {
        if (!window)
            return;

        QApplication::setStyle(u"Fusion"_s);

        QPalette pal = qApp->palette();
        pal.setColor(QPalette::Window, QColor("#f4f4f4"));
        pal.setColor(QPalette::WindowText, QColor("#202020"));
        pal.setColor(QPalette::Base, QColor("#ffffff"));
        pal.setColor(QPalette::AlternateBase, QColor("#fafafa"));
        pal.setColor(QPalette::Text, QColor("#202020"));
        pal.setColor(QPalette::Button, QColor("#ececec"));
        pal.setColor(QPalette::ButtonText, QColor("#292929"));
        pal.setColor(QPalette::Highlight, QColor("#dcecff"));
        pal.setColor(QPalette::HighlightedText, QColor("#111111"));
        qApp->setPalette(pal);
        qApp->setStyleSheet(QString::fromLatin1(UTORRENT_STYLE));
        window->setPalette(pal);
        window->setStyleSheet(QString::fromLatin1(UTORRENT_STYLE));

        auto *toolbar = window->findChild<QToolBar *>(u"toolBar"_s);
        if (!toolbar)
            return;

        toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);
        toolbar->setIconSize(QSize(25, 25));
        toolbar->setMinimumHeight(48);

        for (QAction *action : toolbar->actions())
        {
            const QString name = action->objectName();
            if (name == u"actionOpen") action->setIcon(makeIcon(Glyph::Add));
            else if (name == u"actionDownloadFromURL") action->setIcon(makeIcon(Glyph::Link));
            else if (name == u"actionDelete") action->setIcon(makeIcon(Glyph::Remove));
            else if (name == u"actionStart") action->setIcon(makeIcon(Glyph::Start));
            else if (name == u"actionStop") action->setIcon(makeIcon(Glyph::Stop));
            else if ((name == u"actionTopQueuePos") || (name == u"actionIncreaseQueuePos")) action->setIcon(makeIcon(Glyph::Up));
            else if ((name == u"actionBottomQueuePos") || (name == u"actionDecreaseQueuePos")) action->setIcon(makeIcon(Glyph::Down));
            else if (name == u"actionCreateTorrent") action->setIcon(makeIcon(Glyph::Add));
            else if (name == u"actionOptions") action->setIcon(makeIcon(Glyph::Settings));
        }
    }
}

HidableTabWidget::HidableTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    configure(parent);
    QTimer::singleShot(0, this, [parent] { configure(parent); });

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
