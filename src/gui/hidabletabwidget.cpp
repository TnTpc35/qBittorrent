/*
 * Bittorrent Client using Qt and libtorrent.
 * Copyright (C) 2006  Christophe Dumez <chris@qbittorrent.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * In addition, as a special exception, the copyright holders give permission to
 * link this program with the OpenSSL project's "OpenSSL" library (or with
 * modified versions of it that use the same license as the "OpenSSL" library),
 * and distribute the linked executables. You must obey the GNU General Public
 * License in all respects for all of the code used other than "OpenSSL".  If you
 * modify file(s), you may extend this exception to your version of the file(s),
 * but you are not obligated to do so. If you do not wish to do so, delete this
 * exception statement from your version.
 */

#include "hidabletabwidget.h"

#include <QAction>
#include <QTabBar>
#include <QToolBar>

#ifdef Q_OS_MACOS
#include <QPaintEvent>
#include <QStyle>
#endif

namespace
{
    const char MODERN_MAIN_STYLE[] = R"QSS(
QMainWindow#modernMainWindow {
    background: #0f1412;
    color: #e6eee9;
}

QMainWindow#modernMainWindow QMenuBar {
    background: #111815;
    color: #cdd8d2;
    border: none;
    border-bottom: 1px solid #26342d;
    padding: 3px 8px;
}

QMainWindow#modernMainWindow QMenuBar::item {
    background: transparent;
    border-radius: 7px;
    padding: 7px 11px;
    margin: 1px 2px;
}

QMainWindow#modernMainWindow QMenuBar::item:selected {
    background: #203129;
    color: #ffffff;
}

QMainWindow#modernMainWindow QMenu {
    background: #17201c;
    color: #e6eee9;
    border: 1px solid #314239;
    border-radius: 10px;
    padding: 6px;
}

QMainWindow#modernMainWindow QMenu::item {
    border-radius: 7px;
    padding: 8px 28px 8px 12px;
    margin: 2px;
}

QMainWindow#modernMainWindow QMenu::item:selected {
    background: #244435;
    color: #ffffff;
}

QMainWindow#modernMainWindow QMenu::separator {
    height: 1px;
    background: #314239;
    margin: 5px 8px;
}

QMainWindow#modernMainWindow QToolBar {
    background: #151d19;
    border: none;
    border-bottom: 1px solid #2a3931;
    spacing: 6px;
    padding: 7px 10px;
}

QMainWindow#modernMainWindow QToolBar::separator {
    width: 1px;
    background: #314239;
    margin: 10px 6px;
}

QMainWindow#modernMainWindow QToolButton {
    background: transparent;
    color: #dce6e0;
    border: 1px solid transparent;
    border-radius: 11px;
    padding: 7px 10px;
    margin: 1px;
    min-width: 58px;
    min-height: 50px;
    font-weight: 600;
}

QMainWindow#modernMainWindow QToolButton:hover {
    background: #20332a;
    border-color: #315543;
    color: #ffffff;
}

QMainWindow#modernMainWindow QToolButton:pressed,
QMainWindow#modernMainWindow QToolButton:checked {
    background: #25553f;
    border-color: #34c77b;
    color: #ffffff;
}

QMainWindow#modernMainWindow QLineEdit,
QMainWindow#modernMainWindow QComboBox,
QMainWindow#modernMainWindow QSpinBox,
QMainWindow#modernMainWindow QDoubleSpinBox {
    background: #19221e;
    color: #e8f0eb;
    border: 1px solid #314239;
    border-radius: 10px;
    padding: 7px 10px;
    selection-background-color: #2b7653;
}

QMainWindow#modernMainWindow QLineEdit:hover,
QMainWindow#modernMainWindow QComboBox:hover,
QMainWindow#modernMainWindow QSpinBox:hover,
QMainWindow#modernMainWindow QDoubleSpinBox:hover {
    border-color: #456352;
}

QMainWindow#modernMainWindow QLineEdit:focus,
QMainWindow#modernMainWindow QComboBox:focus,
QMainWindow#modernMainWindow QSpinBox:focus,
QMainWindow#modernMainWindow QDoubleSpinBox:focus {
    border: 1px solid #35c97c;
    background: #1b2721;
}

QMainWindow#modernMainWindow QToolBar QLineEdit {
    min-width: 260px;
    min-height: 24px;
    margin: 7px 4px;
}

QMainWindow#modernMainWindow QComboBox::drop-down {
    width: 24px;
    border: none;
}

QMainWindow#modernMainWindow QTabWidget::pane {
    background: #101613;
    border: none;
    border-top: 1px solid #27362e;
}

QMainWindow#modernMainWindow QTabBar {
    background: #101613;
}

QMainWindow#modernMainWindow QTabBar::tab {
    background: transparent;
    color: #93a39a;
    border: none;
    border-radius: 10px;
    padding: 10px 17px;
    margin: 5px 3px;
    min-width: 72px;
    font-weight: 600;
}

QMainWindow#modernMainWindow QTabBar::tab:hover {
    background: #1c2822;
    color: #dbe7e0;
}

QMainWindow#modernMainWindow QTabBar::tab:selected {
    background: #214b38;
    color: #f4fff8;
}

QMainWindow#modernMainWindow QTreeView,
QMainWindow#modernMainWindow QTableView,
QMainWindow#modernMainWindow QListView,
QMainWindow#modernMainWindow QListWidget,
QMainWindow#modernMainWindow QTableWidget {
    background: #101613;
    alternate-background-color: #121a16;
    color: #dce6e0;
    border: none;
    outline: none;
    selection-background-color: #23513c;
    selection-color: #ffffff;
}

QMainWindow#modernMainWindow QTreeView::item,
QMainWindow#modernMainWindow QTableView::item,
QMainWindow#modernMainWindow QListView::item,
QMainWindow#modernMainWindow QListWidget::item {
    border: none;
    padding: 7px 8px;
}

QMainWindow#modernMainWindow QTreeView::item:hover,
QMainWindow#modernMainWindow QTableView::item:hover,
QMainWindow#modernMainWindow QListView::item:hover,
QMainWindow#modernMainWindow QListWidget::item:hover {
    background: #19251f;
}

QMainWindow#modernMainWindow QTreeView::item:selected,
QMainWindow#modernMainWindow QTableView::item:selected,
QMainWindow#modernMainWindow QListView::item:selected,
QMainWindow#modernMainWindow QListWidget::item:selected {
    background: #23513c;
    color: #ffffff;
}

QMainWindow#modernMainWindow QHeaderView {
    background: #151d19;
}

QMainWindow#modernMainWindow QHeaderView::section {
    background: #151d19;
    color: #8fa198;
    border: none;
    border-bottom: 1px solid #2a3931;
    padding: 9px 8px;
    min-height: 24px;
    font-weight: 600;
}

QMainWindow#modernMainWindow QHeaderView::section:hover {
    background: #1c2922;
    color: #dfe9e3;
}

QMainWindow#modernMainWindow QScrollBar:vertical {
    background: transparent;
    width: 12px;
    margin: 2px;
}

QMainWindow#modernMainWindow QScrollBar::handle:vertical {
    background: #394a41;
    min-height: 30px;
    border-radius: 5px;
}

QMainWindow#modernMainWindow QScrollBar::handle:vertical:hover {
    background: #4c6558;
}

QMainWindow#modernMainWindow QScrollBar::add-line:vertical,
QMainWindow#modernMainWindow QScrollBar::sub-line:vertical,
QMainWindow#modernMainWindow QScrollBar::add-page:vertical,
QMainWindow#modernMainWindow QScrollBar::sub-page:vertical {
    background: transparent;
    border: none;
    height: 0px;
}

QMainWindow#modernMainWindow QScrollBar:horizontal {
    background: transparent;
    height: 12px;
    margin: 2px;
}

QMainWindow#modernMainWindow QScrollBar::handle:horizontal {
    background: #394a41;
    min-width: 30px;
    border-radius: 5px;
}

QMainWindow#modernMainWindow QScrollBar::handle:horizontal:hover {
    background: #4c6558;
}

QMainWindow#modernMainWindow QScrollBar::add-line:horizontal,
QMainWindow#modernMainWindow QScrollBar::sub-line:horizontal,
QMainWindow#modernMainWindow QScrollBar::add-page:horizontal,
QMainWindow#modernMainWindow QScrollBar::sub-page:horizontal {
    background: transparent;
    border: none;
    width: 0px;
}

QMainWindow#modernMainWindow QSplitter::handle {
    background: #26342d;
}

QMainWindow#modernMainWindow QSplitter::handle:horizontal {
    width: 5px;
}

QMainWindow#modernMainWindow QSplitter::handle:vertical {
    height: 5px;
}

QMainWindow#modernMainWindow QStatusBar {
    background: #131b17;
    color: #a9b8b0;
    border-top: 1px solid #2a3931;
    min-height: 28px;
    padding: 2px 8px;
}

QMainWindow#modernMainWindow QToolTip {
    background: #1c2721;
    color: #eef7f1;
    border: 1px solid #3d5549;
    border-radius: 7px;
    padding: 6px 8px;
}

QMainWindow#modernMainWindow QWidget#modernFiltersSidebar {
    background: #0c110f;
    border-right: 1px solid #28372f;
}

QMainWindow#modernMainWindow QWidget#modernFiltersSidebarContent {
    background: #0c110f;
}

QMainWindow#modernMainWindow QWidget#modernSidebarSection {
    background: #151e1a;
    border: 1px solid #26382f;
    border-radius: 12px;
}

QMainWindow#modernMainWindow QWidget#modernSidebarSection:hover {
    border-color: #345343;
}

QMainWindow#modernMainWindow QWidget#modernSidebarSection QAbstractItemView {
    background: transparent;
    border: none;
    selection-background-color: #23513c;
}

QMainWindow#modernMainWindow QCheckBox#modernSidebarHeader {
    color: #e7f1eb;
    spacing: 8px;
    padding: 2px 4px;
}

QMainWindow#modernMainWindow QWidget#modernSidebarBody {
    background: transparent;
}
)QSS";

    void configureModernToolbar(QWidget *parent)
    {
        if (!parent)
            return;

        parent->setObjectName(u"modernMainWindow"_s);
        parent->setStyleSheet(QString::fromLatin1(MODERN_MAIN_STYLE));

        auto *toolbar = parent->findChild<QToolBar *>(u"toolBar"_s);
        if (!toolbar)
            return;

        toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolbar->setIconSize(QSize(28, 28));
        toolbar->setMinimumHeight(78);

        const auto actions = toolbar->actions();
        for (QAction *action : actions)
        {
            const QString name = action->objectName();
            if (name == u"actionOpen")
                action->setIconText(QObject::tr("Add"));
            else if (name == u"actionDownloadFromURL")
                action->setIconText(QObject::tr("Link"));
            else if (name == u"actionDelete")
                action->setIconText(QObject::tr("Remove"));
            else if (name == u"actionStart")
                action->setIconText(QObject::tr("Start"));
            else if (name == u"actionStop")
                action->setIconText(QObject::tr("Stop"));
            else if (name == u"actionTopQueuePos")
                action->setIconText(QObject::tr("Top"));
            else if (name == u"actionIncreaseQueuePos")
                action->setIconText(QObject::tr("Up"));
            else if (name == u"actionDecreaseQueuePos")
                action->setIconText(QObject::tr("Down"));
            else if (name == u"actionBottomQueuePos")
                action->setIconText(QObject::tr("Bottom"));
            else if (name == u"actionCreateTorrent")
                action->setIconText(QObject::tr("Create"));
            else if (name == u"actionOptions")
                action->setIconText(QObject::tr("Settings"));
            else if (name == u"actionLock")
                action->setIconText(QObject::tr("Lock"));
        }
    }
}

HidableTabWidget::HidableTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    configureModernToolbar(parent);

    setDocumentMode(true);
    tabBar()->setExpanding(false);
    tabBar()->setUsesScrollButtons(true);
    tabBar()->setIconSize(QSize(20, 20));

    // Skip single tab in keyboard navigation (no point navigating to it)
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
    // Skip single tab in keyboard navigation (no point navigating to it)
    tabBar()->setFocusPolicy((tabsCount > 1) ? Qt::StrongFocus : Qt::NoFocus);
}

#ifdef Q_OS_MACOS
void HidableTabWidget::paintEvent(QPaintEvent *event)
{
    // Hide the pane for macintosh style
    if (!style()->inherits("QMacStyle"))
        QTabWidget::paintEvent(event);
}
#endif
