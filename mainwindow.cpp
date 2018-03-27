/*
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

#include "mainwindow.h"
#include "about.h"
#include "addtodatabase.h"
#include "editdb.h"
#include "liststringfileparser.h"
#include "preferences.h"

#include <QAction>
#include <QDebug>
#include <QDirIterator>
#include <QFile>
#include <QFontDatabase>
#include <QVBoxLayout>
#include <QMenu>
#include <QHeaderView>
#include <QShortcut>
#include <QTime>
#include <QMessageBox>
#include <thread>
#include <chrono>
#include <QMutex>
#include <QCryptographicHash>
#include <QEventLoop>
#include <QSettings>
using namespace std;

mainWindow::mainWindow(QWidget *parent)
    : QDialog(parent)
{

    readSettings();
    fontSetup();
    createWidgets();
    keyBoardInstance();
    videoInstance();
    openingInstance();
    audioEffectInstance();
    randomNumberInstance();
    createShortcut();
    setBackground();

    getCategory();
    setWindowFlags(Qt::FramelessWindowHint);
}

void mainWindow::createWidgets(){

    resize(desktop_width, desktop_height);
    QVBoxLayout *layout_main = new QVBoxLayout;
    QHBoxLayout *layout_top = new QHBoxLayout;

    auto *pb_menu = new QPushButton(QIcon(":/usr/share/elroke/icon/menu.png"),"", this);
    pb_menu->setFlat(1);
    pb_menu->setFocusPolicy(Qt::NoFocus);
    connect(pb_menu,SIGNAL(pressed()),this,SLOT(checkAdmin()));
//     connect(pb_menu,SIGNAL(pressed()),this,SLOT(d_addtodatabse()));

    le_search = new CLineEdit(this);
    le_search->setPlaceholderText(tr("SEARCH"));

    auto *button_show_all = new QPushButton(tr("ALL"), this);
    button_show_all->setFocusPolicy(Qt::NoFocus);
    button_show_all->setFlat(1);
    auto *button_cat_indonesia = new QPushButton("category1", this);
    button_cat_indonesia->setFlat(1);
    button_cat_indonesia->setFocusPolicy(Qt::NoFocus);
    auto *button_cat_barat = new QPushButton("category2", this);
    button_cat_barat->setFlat(1);
    button_cat_barat->setFocusPolicy(Qt::NoFocus);
    auto *button_cat_rock = new QPushButton("category3", this);
    button_cat_rock->setFlat(1);
    button_cat_rock->setFocusPolicy(Qt::NoFocus);
    auto *button_cat_pop = new QPushButton("category4", this);
    button_cat_pop->setFlat(1);
    button_cat_pop->setFocusPolicy(Qt::NoFocus);
    auto *button_cat_dangdut = new QPushButton("category5", this);
    button_cat_dangdut->setFlat(1);
    button_cat_dangdut->setFocusPolicy(Qt::NoFocus);

    QPalette secon;
    secon.setColor(QPalette::Button, QColor("#80cbc4"));
    secon.setColor(QPalette::ButtonText,Qt::white);
    button_show_all->setAutoFillBackground(1);
    button_show_all->setPalette(secon);
    button_cat_barat->setAutoFillBackground(1);
    button_cat_indonesia->setPalette(secon);
    button_cat_indonesia->setAutoFillBackground(1);
    button_cat_barat->setPalette(secon);
    button_cat_dangdut->setAutoFillBackground(1);
    button_cat_rock->setPalette(secon);
    button_cat_pop->setPalette(secon);
    button_cat_dangdut->setPalette(secon);
//show clock
    QLabel *clock = new QLabel(this);
    clock->setText(QTime::currentTime().toString("hh:mm"));
    QTimer *timer = new QTimer(this);
    connect(timer,&QTimer::timeout,[clock]()
    {
          clock->setText(QTime::currentTime().toString("hh:mm"));
    });
    timer->start(60000);

    layout_top->addWidget(pb_menu);
    layout_top->addWidget(le_search);
    layout_top->addStretch();
    layout_top->addWidget(button_show_all);
    layout_top->addWidget(button_cat_indonesia);
    layout_top->addWidget(button_cat_barat);
    layout_top->addWidget(button_cat_rock);
    layout_top->addWidget(button_cat_pop);
    layout_top->addWidget(button_cat_dangdut);
    layout_top->addStretch();
    layout_top->addWidget(clock);
    layout_top->setSpacing(0);
    layout_top->setMargin(0);

    QWidget *widget_top = new QWidget(this);
//    QPixmap bg_trans(":/usr/share/elroke/background/backgroundTrans.png");
    QPalette transparent_palette;

//    bg_trans =   bg_trans.scaled(widget_top->size(),Qt::IgnoreAspectRatio,Qt::FastTransformation);
    transparent_palette.setBrush(QPalette::Background, QColor("#009688"));

    widget_top->setAutoFillBackground(1);
    widget_top->setPalette(transparent_palette);
    widget_top->setLayout(layout_top);


    QHBoxLayout *layout_table = new QHBoxLayout;

    QPalette table_palette;
    table_palette.setColor(QPalette::Base, Qt::transparent);
    table_palette.setColor(QPalette::Text, Qt::white);
//    table_palette.setColor(QPalette::Active,Qt::red);
    table_palette.setColor(QPalette::Highlight, Qt::blue);
    table_palette.setColor(QPalette::HighlightedText, Qt::yellow);

    db = new dbmanager(dbmanager::show, this);
    db->connectToDB();

    sql_model = new QSqlTableModel(this, db->database());
    sql_model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    proxy_model = new ProxyModel(ProxyModel::smart, this);
    proxy_model->setSourceModel(sql_model);
    proxy_model->setAlignment(1, Qt::AlignLeft | Qt::AlignVCenter);
    proxy_model->setAlignment(2, Qt::AlignRight | Qt::AlignVCenter);

    table = new QTableView(this);
    table->setModel(proxy_model);
//    table->setSortingEnabled(1);
//    tableRule();
    sql_model->setTable("ELROKE123");
    sql_model->select();
    sql_model->setSort(1,Qt::AscendingOrder);
    table->verticalHeader()->hide();
    table->setShowGrid(0);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->hideColumn(0);
    table->hideColumn(3);
    table->hideColumn(4);
    table->hideColumn(5);
    table->hideColumn(6);
    table->hideColumn(7);
    table->hideColumn(8);
    table->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    table->model()->setHeaderData(1, Qt::Horizontal,Qt::AlignLeft, Qt::TextAlignmentRole);
    table->model()->setHeaderData(2, Qt::Horizontal,Qt::AlignRight, Qt::TextAlignmentRole);
    table->horizontalHeader()->setHighlightSections(0);
    table->setItemDelegate(new NoFocusDelegate());

    QPalette header_palette = table->horizontalHeader()->palette();
    header_palette.setColor(QPalette::Base, Qt::transparent);
    header_palette.setColor(QPalette::Button, Qt::transparent);
    header_palette.setColor(QPalette::Background, Qt::transparent);
    header_palette.setColor(QPalette::ButtonText, Qt::black);
    header_palette.setColor(QPalette::Normal, QPalette::Window, Qt::green);

    table->horizontalHeader()->setPalette(header_palette);
    table->setPalette(table_palette);

//space vertical each item prevent too close beetween items
    QHeaderView *vertical = table->verticalHeader();
    vertical->setDefaultSectionSize(vertical->fontMetrics().height()+10);

    connect(table,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(addToPlaylist()));
    connect(le_search,SIGNAL(focussed(bool)),this,SLOT(showKeyboard(bool)));
    connect(le_search,SIGNAL(textChanged(QString)),proxy_model,SLOT(search(QString)));
    connect(button_show_all,SIGNAL(pressed()),proxy_model,SLOT(reset()));

    QSizePolicy spLeft(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spLeft.setHorizontalStretch(3);
    table->setSizePolicy(spLeft);

    auto *layout_playlist = new QVBoxLayout;

    playlist_widget = new ListWidget(this);
    playlist_widget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    playlist_widget->setDragEnabled(true);
    playlist_widget->viewport()->setAcceptDrops(true);
    playlist_widget->setDropIndicatorShown(true);
    playlist_widget->setDragDropMode(QAbstractItemView::InternalMove);
    playlist_widget->setPalette(table_palette);
    playlist_widget->setAutoScroll(1);

    connect(playlist_widget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(playPlayer()));

    auto *layout_button_playlist = new QHBoxLayout;

    auto *button_menu = new QPushButton(QIcon(":/usr/share/elroke/icon/menu.png"),"",this);
    button_menu->setFlat(1);
    button_menu->setIconSize(QSize(32,32));

    auto *menu_playlist = new QMenu(this);
    autosave_playlist = new QAction(tr("Auto save playlist"), this);
    autosave_playlist->setFont(font());
    autosave_playlist->setCheckable(1);
    autosave_playlist->setChecked(1);
    //load playlist
    if(autosave_playlist->isChecked())
        loadPlaylist();

    auto*save_as = new QAction(tr("Save as"),  this);
    save_as->setFont(font());
    connect(save_as, SIGNAL(triggered(bool)), this, SLOT(dialogSavePlaylist()));
    auto *load_playlist = new QAction(tr("Load playlist") , this);
    load_playlist->setFont(font());
    connect(load_playlist, SIGNAL(triggered(bool)),this,SLOT(dialogLoadPlaylist()));

    menu_playlist->addAction(autosave_playlist);
    menu_playlist->addAction(save_as);
    menu_playlist->addAction(load_playlist);

    button_menu->setMenu(menu_playlist);
    button_menu->setFocusPolicy(Qt::NoFocus);

    auto *button_move_to_top = new QPushButton(QIcon(":/usr/share/elroke/icon/top.png"),"", this);
    button_move_to_top->setFocusPolicy(Qt::NoFocus);
    button_move_to_top->setFlat(1);
    button_move_to_top->setIconSize(QSize(32,32));
    connect(button_move_to_top,SIGNAL(pressed()),this,SLOT(moveItemToTop()));

    auto *button_move_to_bottom = new QPushButton(QIcon(":/usr/share/elroke/icon/bottom.png"),"", this);
    button_move_to_bottom->setFlat(1);
    button_move_to_bottom->setIconSize(QSize(32,32));
    button_move_to_bottom->setFocusPolicy(Qt::NoFocus);
    connect(button_move_to_bottom,SIGNAL(pressed()),this,SLOT(moveItemToBottom()));

    auto *button_delete = new QPushButton(QIcon(":/usr/share/elroke/icon/delete.png"),"", this);
    button_delete->setFocusPolicy(Qt::NoFocus);
    button_delete->setFlat(1);
    button_delete->setIconSize(QSize(32,32));
    connect(button_delete,&QPushButton::pressed,[this]()
    {
        qDeleteAll(playlist_widget->selectedItems());
    });

    auto *button_clear_playlist = new QPushButton(QIcon(":/usr/share/elroke/icon/clear.png"),"", this);
    button_clear_playlist->setFocusPolicy(Qt::NoFocus);
    button_clear_playlist->setFlat(1);
    button_clear_playlist->setIconSize(QSize(32,32));
    connect(button_clear_playlist,&QPushButton::pressed,[this]()
    {
         playlist_widget->clear();
    });

    auto *button_lock_playlist = new QPushButton(QIcon(":/usr/share/elroke/icon/lock.png"),"", this);
    button_lock_playlist->setFocusPolicy(Qt::NoFocus);
    button_lock_playlist->setCheckable(true);
    button_lock_playlist->setFlat(1);
    button_lock_playlist->setIconSize(QSize(32,32));
    connect(button_lock_playlist,&QPushButton::clicked,[this, button_lock_playlist](bool lock)
    {
        lock_playlist = lock;
        if(lock)
            button_lock_playlist->setIcon(QIcon(":/usr/share/elroke/icon/unlock.png"));
        else
             button_lock_playlist->setIcon(QIcon(":/usr/share/elroke/icon/lock.png"));
    });

    layout_button_playlist->addWidget(button_menu);
    layout_button_playlist->addWidget(button_delete);
    layout_button_playlist->addWidget(button_clear_playlist);
    layout_button_playlist->addWidget(button_move_to_top);
    layout_button_playlist->addWidget(button_move_to_bottom);
    layout_button_playlist->addWidget(button_lock_playlist);
    layout_button_playlist->setSpacing(0);
    layout_button_playlist->setMargin(0);

    layout_playlist->addLayout(layout_button_playlist);
    layout_playlist->addWidget(playlist_widget);
    layout_playlist->setSpacing(0);
    layout_playlist->setMargin(0);

    //CREATE WIDGET TO APPLY SIZEPOLICY
    auto *widget_playlist = new QWidget(this);
    widget_playlist->setLayout(layout_playlist);

    QSizePolicy spRight(QSizePolicy::Preferred,QSizePolicy::Preferred);
    spRight.setHorizontalStretch(1);
    widget_playlist->setSizePolicy(spRight);

    layout_table->addWidget(table);
    layout_table->addWidget(widget_playlist);
    layout_table->setMargin(0);
    layout_table->setSpacing(0);

    //CREATE WIDGET TO APPLY SIZEPOLICY(Height)
    auto *all_table = new QWidget(this);
    all_table->setLayout(layout_table);

    QSizePolicy spTable (QSizePolicy::Expanding,QSizePolicy::Expanding);
    spTable.setVerticalStretch(2);
    all_table->setSizePolicy(spTable);

     auto *layout_player_control = new QHBoxLayout;

    circle = new ProgressCircle(this);
    circle->setMaximum(100);
    circle->setColor(Qt::gray);
    circle->setFixedSize(64,64);

     auto *btn_next = new QPushButton(this);
     btn_next->setIcon(QIcon(":/usr/share/elroke/icon/next.png"));
     btn_next->setFlat(1);
     btn_next->setIconSize(QSize(64,64));
     btn_next->setFocusPolicy(Qt::NoFocus);
     connect(btn_next,SIGNAL(pressed()),this,SLOT(playPlayer()));

//     auto *button_stop = new QPushButton(QIcon::fromTheme("stop"),"", this);
//     button_stop->setFlat(1);
//     button_stop->setFocusPolicy(Qt::NoFocus);
//     button_stop->setPalette(btn_pal);
//     connect(button_stop, SIGNAL(pressed()),video,SLOT(stop()));

     auto *button_play_pause = new QPushButton(QIcon(":/usr/share/elroke/icon/play.png"), "", this);
     button_play_pause->setFlat(1);
     button_play_pause->setIconSize(QSize(64,64));
     button_play_pause->setFocusPolicy(Qt::NoFocus);
     //connect(button_play_pause, SIGNAL(pressed()),this,SLOT(playPausePlayer()));

     auto *button_favorit = new QPushButton(QIcon(":/usr/share/elroke/icon/favorit.png"), "", this);
      button_favorit->setFlat(1);
      button_favorit->setIconSize(QSize(64,64));
      button_favorit->setFocusPolicy(Qt::NoFocus);
      connect(button_favorit,SIGNAL(pressed()),this,SLOT(showHits()));

    button_audio_channel = new QPushButton(this);
    button_audio_channel->setIcon(QIcon(":/usr/share/elroke/icon/stereo.png"));
    button_audio_channel->setIconSize(QSize(64,64));
    button_audio_channel->setFlat(1);
    button_audio_channel->setFocusPolicy(Qt::NoFocus);
    connect(button_audio_channel,SIGNAL(pressed()),this,SLOT(setAudioChannelManual()));

     auto *button_vol_down = new QPushButton(QIcon(":/usr/share/elroke/icon/minus.png"),"", this);
     button_vol_down->setFocusPolicy(Qt::NoFocus);
     button_vol_down->setFlat(1);
     button_vol_down->setIconSize(QSize(24,24));
     connect(button_vol_down,&QPushButton::pressed,[this]()
     {
         if(video->player()->volume()<5)
              video->player()->setVolume(0);
         else
         video->player()->setVolume(video->player()->volume()-5);
     });

     slider_vol = new QSlider(Qt::Horizontal, this);
     slider_vol->setRange(0, 100);
     slider_vol->setFixedWidth(200);
     slider_vol->setFocusPolicy(Qt::NoFocus);

     auto *button_vol_up = new QPushButton(QIcon(":/usr/share/elroke/icon/plus.png"),"", this);
     button_vol_up->setFocusPolicy(Qt::NoFocus);
     button_vol_up->setIconSize(QSize(24,24));
     button_vol_up->setFlat(1);
     connect(button_vol_up,&QPushButton::pressed,[this]()
     {
         if(video->player()-> volume()>95)
             video->player()->setVolume(100);
         else
          video->player()->setVolume(video->player()->volume()+5);
     });

     button_audio_mute = new QPushButton(QIcon(":/usr/share/elroke/icon/unmute.png"),"", this);
     button_audio_mute->setFocusPolicy(Qt::NoFocus);
     button_audio_mute->setIconSize(QSize(64,64));
     button_audio_mute->setFlat(1);
     connect(button_audio_mute,&QPushButton::pressed,[this]()
     {
         if(!video->player()->isPlaying())
             return;

         if(video->player()->isMute())
         {
             video->player()->setMute(0);
             button_audio_mute->setIcon(QIcon(":/usr/share/elroke/icon/unmute.png"));
         }
         else
         {
             video->player()->setMute(1);
             button_audio_mute->setIcon(QIcon(":/usr/share/elroke/icon/mute.png"));
         }
     });

     layout_player_control->addWidget( circle);
     layout_player_control->addStretch();
     layout_player_control->addWidget(btn_next);
     layout_player_control->addWidget(button_play_pause);
     layout_player_control->addWidget(button_favorit);
     layout_player_control->addStretch();
     layout_player_control->addWidget(button_audio_channel);
     layout_player_control->addStretch();
     layout_player_control->addWidget(button_vol_down);
     layout_player_control->addWidget(slider_vol);
     layout_player_control->addWidget(button_vol_up);
     layout_player_control->addStretch();
     layout_player_control->addWidget(button_audio_mute);
     layout_player_control->setMargin(0);

     QWidget *widget_bottom = new QWidget(this);
     widget_bottom->setAutoFillBackground(1);
     widget_bottom->setPalette(transparent_palette);
     widget_bottom->setLayout(layout_player_control);

    layout_main->addWidget(widget_top);
    layout_main->addWidget(all_table);

    //spacer sholud be mask when video playing, keep lyric visible
    spacer = new QWidget(this);
    spacer->adjustSize();
    QSizePolicy spSpacer (QSizePolicy::Expanding,QSizePolicy::Expanding);
    spSpacer.setVerticalStretch(1);

    spacer->setSizePolicy(spSpacer);
    layout_main->addWidget(spacer);

    layout_main->addWidget(widget_bottom);
    layout_main->setMargin(0);
    layout_main->setSpacing(0);
    setLayout(layout_main);
}

void mainWindow::createShortcut(){

    QShortcut *sc_quit = new QShortcut(QKeySequence("Esc"),this);
    connect(sc_quit,&QShortcut::activated,this,&mainWindow::close);

}
void mainWindow::getCategory(){
//QList <QString> s = listStringFileParser::parse(QDir::homePath()+"/.elroke/meta/category");
}

void mainWindow::addToPlaylist()
{
    int row =  proxy_model->mapToSource( table->selectionModel()->currentIndex()).row();
    int id = sql_model->data(sql_model->index(row,0),Qt::DisplayRole).toInt();

   Song *song =   db->getSong(id);
   songitemwidget *item_song_widget = new songitemwidget;
   item_song_widget->setSong(song);

   QListWidgetItem *item = new QListWidgetItem;
   playlist_widget->addItem(item);
   playlist_widget->setItemWidget(item, item_song_widget);

//resize itemwidget to songitemwidget
   item->setSizeHint(QSize(playlist_widget->width()-qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent), item_song_widget->height()));

   playlist_widget->setCurrentRow(playlist_widget->count()-1);
}

void mainWindow::keyPressEvent(QKeyEvent *event){

  //IF USER TYPE USER TYPE NUMBER OR ALPHABET DIRECT TO LINEEDIT SEARCH
    if(isKeyValid(event->key()))
    {
        le_search->setFocus();
        le_search->setText(event->text());
    }

    if(event->key()==Qt::Key_Enter || event->key()==Qt::Key_Return)
    {
        //send item to playlist with enter key
        if(focusWidget()==table)
        {
            addToPlaylist();
        }
        //play item with enter key
         else if(focusWidget()==playlist_widget)
        {
            playPlayer();
        }
    }

        //move focus widget from table to playlist with right key
        else if(event->key()==Qt::Key_Right && qApp->focusWidget()==table)
        {
            playlist_widget->setFocus();
        }

        //move focus widget from playlist to table with left key
        else if(event->key()==Qt::Key_Left && qApp->focusWidget()==playlist_widget)
        {
            table->setFocus();
        }

        if(event->key()==Qt::Key_Backspace)
        {
            if(le_search->text().isEmpty())
                return;
            else
            {
                le_search->setFocus();
                le_search->backspace();
            }
        }
}

void mainWindow::moveItemToTop()
{
    int row = playlist_widget->currentIndex().row();
//ignore first item
    if(row==0)
    {
        return;
    }
//copy song. i'm not sure with this way
        Song *item = new Song;
        songitemwidget *item_current = qobject_cast<songitemwidget*>(playlist_widget->itemWidget(playlist_widget->currentItem()));

        item->setId(item_current->song()->getId());
        item->setTitle(item_current->song()->getTitle());
        item->setSinger(item_current->song()->getSinger());
        item->setPath(item_current->song()->getPath());
        item->setPath(item_current->song()->getgetCategory());
        item->setLanguage(item_current->song()->getLanguage());
        item->setAudioChannel(item_current->song()->getAudioChannel());
        item->setPlaytimes(item_current->song()->getPlayTimes());

        QListWidgetItem *item_new = new QListWidgetItem;

        songitemwidget *item_song_widget = new songitemwidget;
        item_song_widget->setSong(item);

        playlist_widget->insertItem(0,item_new);
        playlist_widget->setItemWidget(item_new, item_song_widget);

         item_new->setSizeHint(QSize(playlist_widget->width()-qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent), item_song_widget->height()));
         qDeleteAll(playlist_widget->selectedItems());
         playlist_widget->setCurrentRow(0);
}

void mainWindow::setBackground()
{
    QPixmap bg(background);
    bg=bg.scaled(desktop->size(),Qt::IgnoreAspectRatio,Qt::FastTransformation);

    QPalette pal;
    pal.setColor(QPalette::Window,Qt::black);
    pal.setColor(QPalette::WindowText,Qt::white);
    pal.setBrush(QPalette::Background, bg);
    //        pal.setColor(QPalette::Button, Qt::transparent);

    setPalette(pal);
}

void mainWindow::playPlayer()
{
    if(playlist_widget->count()==0)
    {
    video->close();
    clearMask();
    return;
    }

    ////STOP IF PLAYING
    if(video->player()->isPlaying())
    {
    video->player()->stop();
    }
    songitemwidget *item_widget = qobject_cast<songitemwidget*>(playlist_widget->itemWidget(playlist_widget->currentItem()));

    unsigned int id = item_widget->song()->getId();
    QString file= item_widget->song()->getPath();
    QString title = item_widget->song()->getTitle();
    QString singer = item_widget->song()->getSinger();
    channel = item_widget->song()->getAudioChannel();

    //CHECK IF FILE EXIST
    if (!QFile(file).exists())
    {
    QMessageBox message(this);
    message.setIcon(QMessageBox::Information);
    message.setInformativeText(tr("File is not found."));
    message.setWindowFlags(Qt::FramelessWindowHint);
    message.setStandardButtons(QMessageBox::Close);
    message.setAutoFillBackground(1);
    message.exec();
    return;
    }

    video->player()->setFile(file);
    video->activateWindow();
    cover->setData(title, singer);
    cover->start();

    db->updatePlayedTime(id);
    sql_model->select();

    if(lock_playlist)
    {
       moveItemToBottom();
    }
    else
    {
        qDeleteAll(playlist_widget->selectedItems());
    }

    if(playlist_widget->count()>1)
    {
        playlist_widget->setCurrentRow(0);
    }
}

void mainWindow::dialogNextSong()
{
    // this will notify next item in playlist before song end
    QPalette let;
    let.setColor(QPalette::Background,Qt::white);
    let.setColor(QPalette::Foreground,Qt::black);

    QFont f;
    f.setPointSize(32);

    QLabel *notif = new QLabel;

    if(playlist_widget->count()==0)
         notif->setText(tr("Playlist Is Empty"));
    else
    {
        songitemwidget * widget_song = qobject_cast<songitemwidget*>(playlist_widget->itemWidget(playlist_widget->item(0)));
        notif->setText(tr("Next song : ")+widget_song->song()->getTitle());
    }

    if(this->isActiveWindow())
        setParent(this);
    else
        setParent(video);

    notif->setWindowFlags(Qt::FramelessWindowHint);
    notif->setPalette(let);
    notif->setFont(f);
    notif->layout()->setMargin(6);
    notif->adjustSize();
    notif->move((desktop_width-notif->width())/2,0);
    notif->setAttribute(Qt::WA_DeleteOnClose);
    QTimer::singleShot(5000, notif, SLOT(close()));
    notif->show();
}

bool mainWindow::eventFilter(QObject *target, QEvent *event)
{
    if(target==video)
    {
        if(event->type()==QEvent::KeyPress )
        {
                QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
                if(isKeyValid(keyEvent->key()))
                {
                        this->activateWindow();
                        this->setMask(getMaska());
                }

               le_search->setFocus();
               le_search->setText(keyEvent->text());
         }

    //if video is playing, user click, mainwindow will be shown
     if(event->type()==QEvent::MouseButtonPress)
     {
         if(!main_window_visible)
         {
             this->activateWindow();
             this->setMask(getMaska());
             main_window_visible=true;
         }
         else
         {
             video->activateWindow();
             main_window_visible=false;
         }
      }
    }

        return QObject::eventFilter(target, event);
}

void mainWindow::dialogSavePlaylist()
{
    QDialog *dialog_save_playlist = new QDialog;
    dialog_save_playlist->setParent(this);
    QVBoxLayout *layout_main = new QVBoxLayout;

    CLineEdit *le_playlist_name = new CLineEdit(dialog_save_playlist);
    connect(le_playlist_name,&CLineEdit::focussed,keyboard,&Keyboard::setVisible);
    QAction *action_delete = new QAction(QIcon(":/usr/share/elroke/icon/backspace.png"), "", this);
    le_playlist_name->addAction(action_delete, QLineEdit::TrailingPosition);
    connect(action_delete,&QAction::triggered,le_playlist_name,&QLineEdit::backspace);

    layout_main->addWidget(new QLabel("Save Playlist As :", dialog_save_playlist));
    layout_main->addWidget(le_playlist_name);

    QHBoxLayout *layout_btn = new QHBoxLayout;

    auto *btn_close = new QPushButton("Cancel", dialog_save_playlist);
    connect(btn_close,&QPushButton::pressed,dialog_save_playlist,&QDialog::close);

    auto *btn_save = new QPushButton("Save", dialog_save_playlist);
    connect(btn_save,&QPushButton::pressed,dialog_save_playlist,&QDialog::accept);

    keyboard->move(QPoint(0,0));

    layout_btn->addWidget(btn_close);
    layout_btn->addStretch();
    layout_btn->addWidget(btn_save);

    layout_main->addLayout(layout_btn);
    dialog_save_playlist->setLayout(layout_main);

   dialog_save_playlist->setWindowFlags( Qt::FramelessWindowHint);
   dialog_save_playlist->setAttribute(Qt::WA_DeleteOnClose);

   le_playlist_name->setFocus();
   dialog_save_playlist->adjustSize();
   dialog_save_playlist->setAutoFillBackground(1);

   connect(dialog_save_playlist,&QDialog::accepted,[this,le_playlist_name]()
   {
       writePlaylist(le_playlist_name->text());
   });
   dialog_save_playlist->show();
   }

void mainWindow::dialogLoadPlaylist(){

    auto *dialog_load_playlist = new QDialog;
    dialog_load_playlist->setParent(this);

    QDirIterator it(app_dir+"/playlist", QDir::Files | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
    QList<QString> list_play;
    QFileInfo info;
    while(it.hasNext())
    {
        info.setFile(it.next());
        if(info.suffix()=="elp" && info.baseName()!="playlist" )//exception for "playlist", this is for default /auto save
        list_play << info.baseName();
    }

    auto *layout_main = new QVBoxLayout;

    auto *_list = new QListWidget(dialog_load_playlist);

    _list->addItems(list_play);

    layout_main->addWidget(new QLabel("Load Playlist :", dialog_load_playlist));

    auto *layout_btn = new QHBoxLayout;
    auto *btn_cancel = new QPushButton("Cancel", dialog_load_playlist);
    connect(btn_cancel,&QPushButton::pressed,dialog_load_playlist,&QDialog::close);

    auto *btn_load = new QPushButton("Load", dialog_load_playlist);
    connect(btn_load,&QPushButton::pressed,dialog_load_playlist,&QDialog::accept);

    layout_btn->addWidget(btn_cancel);
    layout_btn->addStretch();
    layout_btn->addWidget(btn_load);

    layout_main->addWidget(_list);
    layout_main->addLayout(layout_btn);

    dialog_load_playlist->setLayout(layout_main);

    dialog_load_playlist->setWindowFlags(Qt::FramelessWindowHint);
    dialog_load_playlist->setAutoFillBackground(1);
    dialog_load_playlist->setAttribute(Qt::WA_DeleteOnClose);
    dialog_load_playlist->setMinimumSize(300,200);

    connect(dialog_load_playlist,&QDialog::accepted,[this,_list](){
         loadPlaylist(_list->currentItem()->text());
    });

    dialog_load_playlist->show();
}
void mainWindow::writePlaylist()
{
    writePlaylist("playlist");
}

void mainWindow::writePlaylist(const QString &playlistname)
{
    if(playlistname==NULL)        return;

    QString dir_playlist =app_dir+"/playlist";

    if(!QDir(dir_playlist).exists())
        QDir().mkpath(dir_playlist);

    QFile file(dir_playlist+"/"+playlistname+".elp");

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"cant open playlist file";
    }
     QTextStream stream(&file);

     stream << "[elroke playlist]\n";

     for(int i =0;i<playlist_widget->count();i++)
     {
       songitemwidget *item_widget = qobject_cast<songitemwidget*>(playlist_widget->itemWidget(playlist_widget->item(i)));
       stream<<QString::number(item_widget->song()->getId())<<"\n";
     }

     file.close();
}

void mainWindow::loadPlaylist()
{
//default saved playlist
    loadPlaylist("playlist");
}

void mainWindow::loadPlaylist(const QString &s)
{
    playlist_widget->clear();
    QFile file(app_dir+"/playlist/"+s+".elp");
    if(!file.exists())
        return;

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "cant read playlist" << s;
    return;
    }

    QTextStream stream(&file);
    QString stuff= stream.readLine();
    //validate first line
    if(stuff!="[elroke playlist]")
        return;
    stuff= stream.readLine();
    if(stuff==NULL)     return;
    while(stuff!=NULL)
    {
        int id = stuff.toInt();
        Song *song =   db->getSong(id);
        if(song==nullptr) return;
        songitemwidget *item_song_widget = new songitemwidget;
        item_song_widget->setSong(song);
        QListWidgetItem *item = new QListWidgetItem;
        playlist_widget->addItem(item);
        playlist_widget->setItemWidget(item, item_song_widget);
        //resize itemwidget to songitemwidget
        item->setSizeHint(QSize(playlist_widget->width()-qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent), item_song_widget->height()));
        stuff= stream.readLine();
    }
}

QRegion mainWindow::getMaska()
{
    unsigned short int   free_top_point = spacer->mapToGlobal(spacer->rect().topLeft()).y();
    unsigned short int   free_bottom_point = spacer->mapToGlobal( spacer->rect().bottomLeft()).y();
    QRegion r(0,free_top_point,desktop_width,free_bottom_point-free_top_point, QRegion::Rectangle);
    QRegion desk (0,0, desktop_width, desktop_height, QRegion::Rectangle);
    return desk.subtracted(r);
}

void mainWindow::setAudioChannelManual()
{
    switch(video->player()->getAudioChannel()){
    case 1://stereo
        video->player()->setAudioChannelRight();
        button_audio_channel->setIcon(QIcon(":/usr/share/elroke/icon/right.png"));
        break;
    case 3://left
        video->player()->setAudioChannelStereo();
        button_audio_channel->setIcon(QIcon(":/usr/share/elroke/icon/stereo.png"));
        break;
    case 4://right
        video->player()->setAudioChannelLeft();
        button_audio_channel->setIcon(QIcon(":/usr/share/elroke/icon/left.png"));
       break;
    }
}

//void mainWindow::tableRule()
//{
//    //rebuild rule after sql_model->clear();
//    //is there better way to update table?
//    qDebug()<<"tableRule()";
//    sql_model->setTable("ELROKE123");
//    sql_model->select();
//    sql_model->setSort(1,Qt::AscendingOrder);
//    qDebug()<<"tableRule()v";
//    table->verticalHeader()->hide();
//    table->setShowGrid(0);
//    table->setSelectionBehavior(QAbstractItemView::SelectRows);
//    table->setSelectionMode(QAbstractItemView::SingleSelection);
//    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
//     qDebug()<<"tableRule()c";
//    table->hideColumn(0);
//    table->hideColumn(3);
//    table->hideColumn(4);
//    table->hideColumn(5);
//    table->hideColumn(6);
//    table->hideColumn(7);
//    table->hideColumn(8);
//    table->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
//    table->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
//    table->model()->setHeaderData(1, Qt::Horizontal,Qt::AlignLeft, Qt::TextAlignmentRole);
//    table->model()->setHeaderData(2, Qt::Horizontal,Qt::AlignRight, Qt::TextAlignmentRole);
//    table->horizontalHeader()->setHighlightSections(0);
//    table->setItemDelegate(new NoFocusDelegate());
//     qDebug()<<"tableRule()z";
//}

bool mainWindow::isKeyValid(int key)
{
    QList <int> validKey;
    //this enum key 0-9a-zspace
    validKey<<0x30<<0x31<<0x32<<0x33<<0x34<<0x35<<0x36<<0x37<<0x38<<0x39<<0x41<<0x42<<0x43
                   <<0x44<<0x45<<0x46<<0x47<<0x48<<0x49<<0x4a<<0x4b<<0x4c<<0x4d<<0x4e<<0x4f<<0x50
                   <<0x51<<0x52<<0x53<<0x54<<0x55<<0x56<<0x57<<0x58<<0x59<<0x5a<<0x20;

    if(validKey.contains(key))
        return true;

    return false;
}

void mainWindow::showKeyboard(bool x)
{
    if(x){
        int x =(table->width()-keyboard->width())/2;
        int y = table->mapToGlobal(table->rect().bottomLeft()).y()-keyboard->height();
        //key show on the midle of table
         keyboard->showKeyboard(QPoint(x,y));
        keyboard->show();
    }
      else
            keyboard->hide();
}

void mainWindow::videoInstance(){
    //video player
    video = new VideoWidget;
    video->hide();
    video->installEventFilter(this);

    connect(video->player(),&Player::positionChanged,this,&mainWindow::updateInterface);
    connect(video->player(),&Player::reachEnded,this,&mainWindow::videoEnds);
    connect(video->player(),&Player::almostEnded,this,&mainWindow::dialogNextSong);
    connect(video->player(),&Player::playing,this,&mainWindow::setAudioChannelAuto);

    connect(video->player(),&Player::error,[this]()
    {
        QMessageBox error_message(this);// = new QMessageBox(this);
        error_message.setInformativeText("Error playing "+video->player()->getFile());
        error_message.show();
        QTimer::singleShot(3000,&error_message,SLOT(deleteLater()));
    });
    connect(slider_vol,&QSlider::sliderMoved,video->player(),&Player::setVolume);
}

void mainWindow::keyBoardInstance()
{
    keyboard = new Keyboard(this);
    keyboard->hide();
}

void mainWindow::fontSetup()
{
    QFont font;
    font.setPointSize(font_size);
    font.setFamily(c_font);
    setFont(font);
}

void mainWindow::dialogAdmin()
{
    dialog_admin = new QDialog;
    dialog_admin->setParent(this);

    QVBoxLayout *layout_main = new QVBoxLayout;

    auto *button_add_to_database = new QPushButton(tr("ADD TO DATABASE"), dialog_admin);
    connect(button_add_to_database,&QPushButton::pressed,this,&mainWindow::d_addtodatabse);
//    {
//        addtodatabase atd;
////        connect(&atd,SIGNAL(accepted()),dialog_admin,SLOT(close()));
//        connect(&atd,SIGNAL(accepted()),this,SLOT(tableRule()));
////        connect(&atd,SIGNAL(accepted()),this,SLOT(getCategory()));
//        atd.exec();
//    });

    auto *button_manage_database = new QPushButton(tr("MANAGE DATABASE"), dialog_admin);
    connect(button_manage_database,&QPushButton::pressed,[this]()
    {
//        sql_model->clear();
        managedb md;
        connect(&md, SIGNAL(finished(int)),this,SLOT(tableRule()));
        md.exec();
    });

    auto *button_preferences = new QPushButton(tr("PREFERENCES"), dialog_admin);
    connect(button_preferences,&QPushButton::pressed,[this]()
    {
        preferences *pref = new preferences;
        pref->setAttribute(Qt::WA_DeleteOnClose);
        pref->setWindowFlags(Qt::FramelessWindowHint);
        pref->setAutoFillBackground(1);
        pref->setParent(this);
        pref->show();
    });

    auto *button_about = new QPushButton(tr("ABOUT"), dialog_admin);
    connect(button_about,&QPushButton::pressed,[this]()
    {
        about About(this);
        About.exec();
    });

    auto *button_close = new QPushButton(tr("CLOSE"), dialog_admin);
    connect(button_close,&QPushButton::pressed,dialog_admin,&QDialog::close);

    auto *button_exit = new QPushButton(tr("QUIT APP"), dialog_admin);
    connect(button_exit,&QPushButton::pressed,dialog_admin,&QDialog::close);
    connect(button_exit,&QPushButton::pressed,this,&mainWindow::close);

    layout_main->addWidget(button_add_to_database);
    layout_main->addWidget(button_manage_database);
    layout_main->addWidget(button_preferences);
    layout_main->addWidget(button_about);
    layout_main->addWidget(button_close);
    layout_main->addWidget(button_exit);

    dialog_admin->setLayout(layout_main);
    dialog_admin->setWindowFlags(Qt::FramelessWindowHint);
    QPalette palet;
    palet.setColor(QPalette::Base, palette().dark().color());
    palet.setColor(QPalette::Window, Qt::black);
    palet.setColor(QPalette::Text, palette().light().color());
    palet.setColor(QPalette::WindowText, palette().light().color());
    palet.setColor(QPalette::Button, palette().dark().color());
    palet.setColor(QPalette::ButtonText, palette().light().color());

    dialog_admin->setPalette(palet);
    dialog_admin->setAutoFillBackground(1);

    dialog_admin->adjustSize();
//    dialog_admin->setWindowFlags(Qt::FramelessWindowHint);
    dialog_admin->setAttribute(Qt::WA_DeleteOnClose);
//    dialog_admin->setParent(this);
    dialog_admin->show();

}

void mainWindow::checkAdmin()
{
    QSettings settings("elroke","elroke");
    QString user;
    settings.beginGroup("Authentication");
    user=settings.value("username").toString();
    settings.endGroup();

    if(user==NULL)
        dialogCreateAdmin();
    else
        dialogLogin();
}

void mainWindow::dialogCreateAdmin()
{
    QDialog *dialog = new QDialog;
    QVBoxLayout *layout_main = new QVBoxLayout;

    le_userName = new CLineEdit(dialog);

    le_password = new CLineEdit(dialog);
    le_password->setEchoMode(QLineEdit::Password);

    le_password_confirm = new CLineEdit(dialog);
    le_password_confirm->setEchoMode(QLineEdit::Password);

    QHBoxLayout *layout_button = new QHBoxLayout;
    auto *button_create_admin = new QPushButton(tr("Create Administrator"), dialog);
    connect(button_create_admin,&QPushButton::pressed,this,&mainWindow::createAdminAccount);

    auto *button_close = new QPushButton("Close", dialog);
    connect(button_close,&QPushButton::pressed,dialog,&QDialog::close);
    connect(this,&mainWindow::loginAccepted,dialog,&QDialog::close);

    layout_button->addWidget(button_close);
    layout_button->addWidget(button_create_admin);

    layout_main->addWidget(new QLabel(tr("First setup,  create administrator account"), dialog));
    layout_main->addStretch();
    layout_main->addWidget(new QLabel(tr("Username"),dialog));
    layout_main->addWidget(le_userName);

    layout_main->addWidget(new QLabel(tr("Password"),dialog));
    layout_main->addWidget(le_password);
    layout_main->addWidget(new QLabel(tr("Confirm password"),dialog));
    layout_main->addWidget(le_password_confirm);
    layout_main->addStretch();
    layout_main->addLayout(layout_button);

     dialog->setLayout(layout_main);
     connect(this,&mainWindow::usernameCreated,dialog,&QDialog::close);
     dialog->setWindowFlags(Qt::FramelessWindowHint);
     dialog->setParent(this);
     QPalette palet;
     palet.setColor(QPalette::Base, palette().dark().color());
     palet.setColor(QPalette::Window, Qt::black);
     palet.setColor(QPalette::Text, palette().light().color());
     palet.setColor(QPalette::WindowText, palette().light().color());
     palet.setColor(QPalette::Button, palette().dark().color());
     palet.setColor(QPalette::ButtonText, palette().light().color());
     dialog->setPalette(palet);
     dialog->adjustSize();
     dialog->setAutoFillBackground(1);
     dialog->setAttribute(Qt::WA_DeleteOnClose);
     dialog->show();
}

void mainWindow::dialogLogin()
{
    if(dialog_admin!=NULL)
    {
        return;
    }

    QDialog *dialog = new QDialog;
    QVBoxLayout *layout_main = new QVBoxLayout;

    le_userName = new CLineEdit(dialog);
    le_password = new CLineEdit(dialog);
    le_password->setEchoMode(QLineEdit::Password);

    auto *button_close = new QPushButton(tr("Close"), dialog);
    connect(button_close,&QPushButton::pressed,dialog,&QDialog::close);

    auto *button_login = new QPushButton(tr("Login"), dialog);

    connect(le_userName,&CLineEdit::returnPressed,button_login,&QPushButton::pressed);
    connect(le_password,&CLineEdit::returnPressed,button_login,&QPushButton::pressed);
    connect(button_login,&QPushButton::pressed,this,&mainWindow::login);

    QHBoxLayout *layout_button = new QHBoxLayout;
    layout_button->addWidget(button_close);
    layout_button->addWidget(button_login);

    layout_main->addStretch();
    layout_main->addWidget(new QLabel(tr("Username"),dialog));
    layout_main->addWidget(le_userName);
    layout_main->addWidget(new QLabel(tr("Password"),dialog));

    layout_main->addWidget(le_password);
    layout_main->addStretch();
    layout_main->addLayout(layout_button);

     dialog->setLayout(layout_main);
     dialog->setParent(this);
     dialog->setAutoFillBackground(1);
     QPalette palet;
     palet.setColor(QPalette::Base, palette().dark().color());
     palet.setColor(QPalette::Window, Qt::black);
     palet.setColor(QPalette::Text, palette().light().color());
     palet.setColor(QPalette::WindowText, palette().light().color());
     palet.setColor(QPalette::Button, palette().dark().color());
     palet.setColor(QPalette::ButtonText, palette().light().color());
     dialog->setPalette(palet);

     connect(this,&mainWindow::loginAccepted,dialog,&QDialog::close);
     dialog->adjustSize();
     dialog->setAttribute(Qt::WA_DeleteOnClose);
     le_userName->setFocus();
     dialog->show();
}

void mainWindow::createAdminAccount()
{
    QString user = le_userName->text();
    QString pass = le_password->text();
    QString pass_confirm = le_password_confirm->text();

    if(user.isEmpty() || pass.isEmpty() || pass_confirm.isEmpty())
    {
        return;
    }

    if(pass!=pass_confirm){
        return;
    }
        QSettings setting("elroke","elroke");
        setting.beginGroup("Authentication");
        setting.setValue("username", QCryptographicHash::hash(user.toUtf8(), QCryptographicHash::Sha1));
        setting.setValue("password", QCryptographicHash::hash(pass.toUtf8(), QCryptographicHash::Sha1));
        setting.endGroup();

        emit usernameCreated();
        dialogLogin();
}

void mainWindow::login(){

    QSettings settings("elroke","elroke");
    QByteArray user, password;
    settings.beginGroup("Authentication");
    user = settings.value("username").toByteArray();
    password = settings.value("password").toByteArray();
    settings.endGroup();

   QByteArray u  = QCryptographicHash::hash(le_userName->text().toUtf8(), QCryptographicHash::Sha1);
   QByteArray p = QCryptographicHash::hash(le_password->text().toUtf8(), QCryptographicHash::Sha1);

    if(u==user && p==password)
    {
        emit loginAccepted();
        dialogAdmin();
    }
}

void mainWindow::showHits()
{
//    proxy_model->sort(7, ProxyModel::sortRole, Qt::AscendingOrder);
//proxy_model->setSortRole(ProxyModel::sortRole);
proxy_model->sort(6,Qt::AscendingOrder);
}

int mainWindow::getRandomNumber()
{
//    max 100 min 55
    return (qrand()%46)+60;
}

void mainWindow::videoEnds()
{
    auto *dialog_random_number = new QDialog;
    dialog_random_number->setAttribute(Qt::WA_DeleteOnClose);
    dialog_random_number->setWindowFlags(Qt::FramelessWindowHint);

    QLabel *value_label = new QLabel(dialog_random_number);

    effect_player->setFile("/usr/share/elroke/soundfx/applause.mp3");
    effect_player->play();

    connect(effect_player,&Player::reachEnded,dialog_random_number,&QDialog::close);
    connect(effect_player,&Player::reachEnded,this,&mainWindow::playPlayer);

    QFont font;
    font.setPointSize(156);
    font.setBold(1);

    value_label->setFont(font);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(value_label);
    dialog_random_number->setLayout(layout);
    value_label->setText(QString::number(getRandomNumber()));

    QPalette pal;
    pal.setColor(QPalette::WindowText,Qt::blue);
    pal.setColor(QPalette::Window,Qt::white);

    dialog_random_number->setPalette(pal);
    dialog_random_number->show();
}

void mainWindow::audioEffectInstance()
{
    effect_player = new Player(this);
}

void mainWindow::randomNumberInstance(){
    //need to generate random number, for random scoring.
      qsrand(static_cast<uint>(QTime::currentTime().msec()));
}

void mainWindow::openingInstance()
{
    cover = new opening();
    connect(cover,&opening::passed,video,&VideoWidget::show);
    connect(cover,&opening::passed,video->player(),&Player::play);
}

void mainWindow::moveItemToBottom()
{
    int row = playlist_widget->currentIndex().row();

 //ignore first item
     if(row==playlist_widget->count()-1){
         return;
     }
 //copy song. i'm not sure with this way
         Song *item = new Song;
         songitemwidget *item_current = qobject_cast<songitemwidget*>(playlist_widget->itemWidget(playlist_widget->currentItem()));
         item->setId(item_current->song()->getId());
         item->setTitle(item_current->song()->getTitle());
         item->setSinger(item_current->song()->getSinger());
         item->setPath(item_current->song()->getPath());
         item->setPath(item_current->song()->getgetCategory());
         item->setLanguage(item_current->song()->getLanguage());
         item->setPlaytimes(item_current->song()->getPlayTimes());
         item->setAudioChannel(item_current->song()->getAudioChannel());

         QListWidgetItem *item_new = new QListWidgetItem;

         songitemwidget *item_song_widget = new songitemwidget;
         item_song_widget->setSong(item);

         playlist_widget->addItem(item_new);
         playlist_widget->setItemWidget(item_new, item_song_widget);

         item_new->setSizeHint(QSize(playlist_widget->width()-qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent), item_song_widget->height()));
         qDeleteAll(playlist_widget->selectedItems());
         playlist_widget->setCurrentRow(playlist_widget->count()-1);
}
void mainWindow::updateInterface()
{
    circle->setValue(video->player()->position());
    slider_vol->setSliderPosition(video->player()->volume());
}

void mainWindow::setAudioChannelAuto()
{
//wait video fully load
    this_thread::sleep_for(chrono::milliseconds(500));

        if(channel.contains("LEFT", Qt::CaseInsensitive)){
            video->player()->setAudioChannelLeft();
            button_audio_channel->setIcon(QIcon(":/usr/share/elroke/icon/left.png"));
        }
        else if(channel.contains("RIGHT", Qt::CaseInsensitive)){
            video->player()->setAudioChannelRight();
            button_audio_channel->setIcon(QIcon(":/usr/share/elroke/icon/right.png"));
        }
}

void mainWindow::readSettings()
{
    QSettings setting("elroke","elroke");
    setting.beginGroup("Preferences");
    c_font = setting.value("font").toString();
    if(c_font==NULL) c_font = "Roboto";
    background = setting.value("background").toString();
    if(background==NULL) background = ":/usr/share/elroke/background/butterfly.jpeg";
    font_size = setting.value("font_size").toInt();
    if(font_size==0) font_size=16;
    language = setting.value("language").toString();
    setting.endGroup();
}

void mainWindow::d_addtodatabse()
{
    addtodatabase *atd  = new addtodatabase;
//    atd->setAutoFillBackground(1);
//      atd->setAttribute(Qt::WA_DeleteOnClose);
    connect(atd,&addtodatabase::accepted,sql_model,&QSqlTableModel::select);
    atd->show();
}

mainWindow::~mainWindow()
{
    delete video;
    delete cover;
    if(autosave_playlist->isChecked())
        writePlaylist();
}
