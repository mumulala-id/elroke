/*
 *
    ElRoke
    Copyright (C) Muhammad Mukharom

     This file is part of ElRoke.
    ElRoke is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ElRoke is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with ElRoke.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "player.h"
#include "proxymodel.h"
#include "keyboard.h"
#include "dbmanager.h"
#include "clineedit.h"
//#include "opening.h"
#include "songitemwidget.h"
//#include "ProgressCircle.h"
#include "spinnerprogress.h"
#include "videowidget.h"

#include <QApplication>
#include <QDialog>
#include <QComboBox>
#include <QTableView>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QSlider>
#include <QKeyEvent>
#include <QDesktopWidget>
#include <QSqlTableModel>
#include <QPainter>
#include <QStandardPaths>
#include <QThread>
#include <QPointer>
#include <QListWidget>
#include <QListWidget>
#include <QDrag>
#include <QMimeData>
#include <QPainter>

class ListWidget : public QListWidget
{

public :
    ListWidget(QWidget *parent) :
        QListWidget(parent){}

protected :
    void startDrag(Qt::DropActions supportedActions)
{
    QDrag *drag = new QDrag(this);
    drag->setMimeData(model()->mimeData(selectedIndexes()));
    QPixmap pixmap(viewport()->visibleRegion().boundingRect().size());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    for(QModelIndex index: selectedIndexes())
    {
        painter.drawPixmap(visualRect(index), viewport()->grab(visualRect(index)));
    }
    drag->setPixmap(pixmap);
    drag->setHotSpot(viewport()->mapFromGlobal(QCursor::pos()));
    drag->exec(supportedActions, Qt::MoveAction);
}
};


class mainWindow : public QDialog
{
    Q_OBJECT
public:
    mainWindow(QWidget *parent = 0);
    ~mainWindow();

private :
    QComboBox *combo_cat;
    QTableView *table;
    ProxyModel *proxy_model;
    CLineEdit *le_search;
    QSlider *slider_vol;
    QPushButton *button_audio_mute;
    VideoWidget *video;
    QSqlTableModel *sql_model;
    ListWidget *playlist_widget;
//    ProgressCircle *circle;
    spinnerProgress *spinner_progress;
    QString  app_dir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QString category1;
    QString category2;
    QString category3;
    QString category4;
    QString category5;
    QString background;
    QDesktopWidget *desktop = QApplication::desktop();
    unsigned short int desktop_width = desktop->width();
    unsigned short  int desktop_height = desktop->height();
    QString channel;
    bool lock_playlist=false;
    Keyboard *keyboard;
    dbmanager *db;
    QString userID, password;
    Player *effect_player;
//    Opening *opening;
    QWidget *spacer;
    QPushButton *button_audio_channel;
    bool main_window_visible=false;
    QAction *autosave_playlist;
    QPointer <QDialog> dialog_admin=NULL;
    QString c_font;
    QString language;
   unsigned short  int font_size;

private slots:
    void dialogAdmin();
    void dialogCreateAdmin();
//    void openingInstance();
    void dialogLogin();
    void dialogNextSong();
    void createShortcut();
    void createWidgets();
    void setBackground();
    void moveItemToTop();
    bool eventFilter(QObject *, QEvent *);
    QRegion getMaska();
//    void setMute();
    void audioEffectInstance();
    void randomNumberInstance();
    void setAudioChannelManual();
    void setAudioChannelAuto();
    void addToPlaylist();
    void playPlayer();
    void getCategory();
    void dialogSavePlaylist();
    void writePlaylist();
    void writePlaylist(const QString &);//write func
    void dialogLoadPlaylist();
    void loadPlaylist();//DEFAULT
    void loadPlaylist(const QString &);
//    void tableRule();
    bool isKeyValid(int);
    void showKeyboard(bool);
    void videoInstance();
    void keyBoardInstance();
    void fontSetup();
    void videoEnds();
    int getRandomNumber();
    void showHits();
    void moveItemToBottom();
    void updateInterface();
    void readSettings();
    void d_addtodatabse();

protected :
    void keyPressEvent(QKeyEvent *event);

};

// this class hide the focus of qtableview (dotted rectangle on selected item)
class NoFocusDelegate : public QStyledItemDelegate{

protected:

            void paint(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
                    {
                        QStyleOptionViewItem itemOption(option);
                        if (itemOption.state & QStyle::State_HasFocus)
                            itemOption.state = itemOption.state ^ QStyle::State_HasFocus;

                        if (itemOption.state & QStyle::State_Selected) {
                                itemOption.palette.setColor(QPalette::Highlight, QColor("#F5F5F5"));
                                itemOption.palette.setColor(QPalette::HighlightedText, QColor("#E91E63"));
                                QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &itemOption, painter, nullptr);

                               itemOption.palette.setColor(QPalette::Normal,QPalette::Background, Qt::green);


            }
                        else{
                            itemOption.palette.setColor(QPalette::Text, QColor(0,0,0,128));
//                            QPen blackPen(QColor(0,0,0,128));
                             painter->setPen(QColor(0,0,0,128));
                             painter->fillRect(option.rect,QColor(255,255,255));
                            painter->drawLine(option.rect.bottomLeft(),option.rect.bottomRight());
                        }

                        QStyledItemDelegate::paint(painter, itemOption, index);
                    }
};

#endif // MAINWINDOW_H
