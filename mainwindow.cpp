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

using namespace std;

mainWindow::mainWindow(QWidget *parent)
    : QDialog(parent)
{
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
  // setAttribute( Qt::AA_EnableHighDpiScaling);


}

void mainWindow::createWidgets(){

       resize(desktop_width, desktop_height);


    QVBoxLayout *layout_main = new QVBoxLayout;

    QHBoxLayout *layout_top = new QHBoxLayout;

    QPushButton *pb_menu = new QPushButton(QIcon(":/usr/share/elroke/file/icon/logo.png"),"", this);
    pb_menu->setFlat(1);
    pb_menu->setFocusPolicy(Qt::NoFocus);
    connect(pb_menu,SIGNAL(pressed()),this,SLOT(checkAdmin()));

    le_search = new CLineEdit(this);
    le_search->setPlaceholderText(tr("SEARCH"));

    QPushButton *button_show_all = new QPushButton(tr("ALL"), this);
    button_show_all->setFocusPolicy(Qt::NoFocus);
    button_show_all->setFlat(1);
    QPushButton *button_cat_indonesia = new QPushButton("category1", this);
    button_cat_indonesia->setFlat(1);
    button_cat_indonesia->setFocusPolicy(Qt::NoFocus);
    QPushButton *button_cat_barat = new QPushButton("category2", this);
    button_cat_barat->setFlat(1);
    button_cat_barat->setFocusPolicy(Qt::NoFocus);
    QPushButton *button_cat_rock = new QPushButton("category3", this);
    button_cat_rock->setFlat(1);
    button_cat_rock->setFocusPolicy(Qt::NoFocus);
    QPushButton *button_cat_pop = new QPushButton("category4", this);
    button_cat_pop->setFlat(1);
    button_cat_pop->setFocusPolicy(Qt::NoFocus);
    QPushButton *button_cat_dangdut = new QPushButton("category5", this);
    button_cat_dangdut->setFlat(1);
    button_cat_dangdut->setFocusPolicy(Qt::NoFocus);

    QPalette secon;

    secon.setColor(QPalette::Button, QColor("#80cbc4"));
    secon.setColor(QPalette::ButtonText,Qt::white);
    button_show_all->setAutoFillBackground(1);
    button_show_all->setPalette(secon);
    button_cat_indonesia->setPalette(secon);
    button_cat_barat->setPalette(secon);
    button_cat_rock->setPalette(secon);
    button_cat_pop->setPalette(secon);
    button_cat_dangdut->setPalette(secon);
//show clock
    clock =new QLabel(this);
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(showClock()));
    timer->start(60000);
    showClock();

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
//    QPixmap bg_trans(":/usr/share/elroke/file/background/backgroundTrans.png");
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
//    table->setSortingEnabled(0);
    tableRule();

    QPalette header_palette = table->horizontalHeader()->palette();
    header_palette.setColor(QPalette::Base, Qt::transparent);
    header_palette.setColor(QPalette::Button, Qt::transparent);
    header_palette.setColor(QPalette::Background, Qt::transparent);
    header_palette.setColor(QPalette::ButtonText, Qt::black);
    header_palette.setColor(QPalette::Normal, QPalette::Window, Qt::green);

    table->horizontalHeader()->setPalette(header_palette);
    table->setPalette(table_palette);

//    QHeaderView *horizontal = table->horizontalHeader();
//    horizontal->set

//space vertical each item prevent too close beetween items
    QHeaderView *vertical = table->verticalHeader();
    vertical->setDefaultSectionSize(vertical->fontMetrics().height()+30);

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

    connect(playlist_widget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(playPlayer()));
    connect(playlist_widget->model(),SIGNAL(rowsInserted(QModelIndex,int,int)),playlist_widget,SLOT(scrollToBottom()));

    auto *layout_button_playlist = new QHBoxLayout;

    auto *button_menu = new QPushButton(QIcon(":/usr/share/elroke/file/icon/menu.png"),"",this);

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

    auto *button_move_to_top = new QPushButton(QIcon(":/usr/share/elroke/file/icon/top.png"),"", this);

    connect(button_move_to_top,SIGNAL(pressed()),this,SLOT(moveItemToTop()));

    auto *button_moveTo_bottom = new QPushButton(QIcon(":/usr/share/elroke/file/icon/bottom.png"),"", this);


    auto *button_delete = new QPushButton(QIcon(":/usr/share/elroke/file/icon/delete.png"),"", this);
    button_delete->setFocusPolicy(Qt::NoFocus);
    connect(button_delete,SIGNAL(pressed()),this,SLOT(deleteItemPlaylist()));

    auto *button_clear_playlist = new QPushButton(QIcon(":/usr/share/elroke/file/icon/clear.png"),"", this);
    button_clear_playlist->setFocusPolicy(Qt::NoFocus);
    connect(button_clear_playlist,SIGNAL(pressed()),this,SLOT(clearPlaylist()));

//    auto *button_up = new QPushButton(QIcon(":/usr/share/elroke/file/icon/up.png"),"", this);
//    button_up->setFocusPolicy(Qt::NoFocus);
//    connect(button_up,SIGNAL(pressed()),this,SLOT(moveItemUp()));

//    auto *button_down = new QPushButton(QIcon(":/usr/share/elroke/file/icon/down.png"), "",this);
//    button_down->setFocusPolicy(Qt::NoFocus);
//    connect(button_down,SIGNAL(pressed()),this,SLOT(moveItemDown()));

    button_lock_playlist = new QPushButton(QIcon(":/usr/share/elroke/file/icon/lock.png"),"", this);
    button_lock_playlist->setFocusPolicy(Qt::NoFocus);
    button_lock_playlist->setCheckable(true);
    connect(button_lock_playlist,&QPushButton::clicked,this,&mainWindow::updateLockButton);
//    button_lock_playlist->setCentang(1);

    layout_button_playlist->addWidget(button_menu);
    layout_button_playlist->addWidget(button_delete);
    layout_button_playlist->addWidget(button_clear_playlist);
       layout_button_playlist->addWidget(button_move_to_top);
       layout_button_playlist->addWidget(button_moveTo_bottom);
//    layout_button_playlist->addWidget(button_up);
//    layout_button_playlist->addWidget(button_down);
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
     btn_next->setIcon(QIcon(":/usr/share/elroke/file/icon/next.png"));
     btn_next->setFlat(1);
     btn_next->setIconSize(QSize(64,64));
     btn_next->setFocusPolicy(Qt::NoFocus);
     connect(btn_next,SIGNAL(pressed()),this,SLOT(playPlayer()));

//     QPushButton *button_stop = new QPushButton(QIcon::fromTheme("stop"),"", this);
//     button_stop->setFlat(1);
//     button_stop->setFocusPolicy(Qt::NoFocus);
//     button_stop->setPalette(btn_pal);
//     connect(button_stop, SIGNAL(pressed()),video,SLOT(stop()));

     auto *button_play_pause = new QPushButton(QIcon(":/usr/share/elroke/file/icon/play.png"), "", this);
     button_play_pause->setFlat(1);
     button_play_pause->setIconSize(QSize(64,64));
     button_play_pause->setFocusPolicy(Qt::NoFocus);
     //connect(button_play_pause, SIGNAL(pressed()),this,SLOT(playPausePlayer()));

     auto *button_favorit = new QPushButton(QIcon(":/usr/share/elroke/file/icon/favorit.png"), "", this);
      button_favorit->setFlat(1);
      button_favorit->setIconSize(QSize(64,64));
      button_favorit->setFocusPolicy(Qt::NoFocus);
      connect(button_favorit,SIGNAL(pressed()),this,SLOT(showHits()));

    button_audio_channel = new QPushButton(this);
    button_audio_channel->setIcon(QIcon(":/usr/share/elroke/file/icon/stereo.png"));
    button_audio_channel->setIconSize(QSize(64,64));
    button_audio_channel->setFlat(1);
    button_audio_channel->setFocusPolicy(Qt::NoFocus);
    connect(button_audio_channel,SIGNAL(pressed()),this,SLOT(setAudioChannelManual()));

     QPushButton *button_vol_down = new QPushButton(QIcon(":/usr/share/elroke/file/icon/minus.png"),"", this);
     button_vol_down->setFocusPolicy(Qt::NoFocus);
     button_vol_down->setFlat(1);
     button_vol_down->setIconSize(QSize(24,24));
     connect(button_vol_down, SIGNAL(pressed()),this,SLOT(setVolStepDown()));

     slider_vol = new QSlider(Qt::Horizontal, this);
     slider_vol->setRange(0, 100);
     slider_vol->setFixedWidth(200);
     slider_vol->setFocusPolicy(Qt::NoFocus);


     QPushButton *button_vol_up = new QPushButton(QIcon(":/usr/share/elroke/file/icon/plus.png"),"", this);
     button_vol_up->setFocusPolicy(Qt::NoFocus);
     button_vol_up->setIconSize(QSize(24,24));
     button_vol_up->setFlat(1);
     connect(button_vol_up,SIGNAL(pressed()), this, SLOT(setVolStepUp()));

     button_audio_mute = new QPushButton(QIcon(":/usr/share/elroke/file/icon/unmute.png"),"", this);
     button_audio_mute->setFocusPolicy(Qt::NoFocus);
     button_audio_mute->setIconSize(QSize(64,64));
     button_audio_mute->setFlat(1);
     connect(button_audio_mute,SIGNAL(pressed()),this,SLOT(setMute()));

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

    //spacer willbe mask when video playing, keep lyric vicible
    spacer = new QWidget(this);
    spacer->adjustSize();
    QSizePolicy spSpacer (QSizePolicy::Expanding,QSizePolicy::Expanding);
    spSpacer.setVerticalStretch(1);


    spacer->setSizePolicy(spSpacer);
    layout_main->addWidget(spacer);

    layout_main->addWidget(widget_bottom);
    layout_main->setMargin(0);
    setLayout(layout_main);


}

void mainWindow::createShortcut(){

    QShortcut *sc_quit =new QShortcut(QKeySequence("Esc"),this);
    connect(sc_quit,SIGNAL(activated()),qApp,SLOT(quit()));

}
void mainWindow::getCategory(){

//QList <QString> s = listStringFileParser::parse(QDir::homePath()+"/.elroke/meta/category");


}


void mainWindow::setVolStepDown(){

    if(video->volume()<5)
         video->setVolume(0);
    else
    video->setVolume(video->volume()-5);

}

void mainWindow::setVolStepUp(){

    if(video->volume()>95)
        video->setVolume(100);
    else
     video->setVolume(video->volume()+5);
}

void mainWindow::setMute(){

    if(!video->isPlaying())
        return;

    if(video->isMute()){
        video->setMute(0);
        button_audio_mute->setIcon(QIcon(":/usr/share/elroke/file/icon/unmute.png"));
    }

    else{
        video->setMute(1);
         button_audio_mute->setIcon(QIcon(":/usr/share/elroke/file/icon/mute.png"));
    }
}
void mainWindow::showClock(){

     clock->setText(QTime::currentTime().toString("hh:mm"));
}

void mainWindow::addToPlaylist(){

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

}

void mainWindow::keyPressEvent(QKeyEvent *event){

  //IF USER TYPE USER TYPE NUMBER OR ALPHABET DIRECT TO LINEEDIT SEARCH
    if(isKeyValid(event->key())){
        le_search->setFocus();
        le_search->setText(event->text());
    }

        if(event->key()==Qt::Key_Enter || event->key()==Qt::Key_Return){
            //send item to playlist with enter key
            if(focusWidget()==table){
                addToPlaylist();
            }
            //play item with enter key
            else if(focusWidget()==playlist_widget){
                playPlayer();
            }
        }



        //move focus widget from table to playlist with right key
        else if(event->key()==Qt::Key_Right && qApp->focusWidget()==table){

            playlist_widget->setFocus();
        }

        //move focus widget from playlist to table with left key
        else if(event->key()==Qt::Key_Left && qApp->focusWidget()==playlist_widget){
            table->setFocus();

        }




        if(event->key()==Qt::Key_Backspace){
            if(le_search->text().isEmpty())
                return;
            else{
                le_search->setFocus();
                le_search->backspace();
            }
        }
}

void mainWindow::dialogAbout(){

    about About(this);
//    About.setAutoFillBackground(1);
    About.exec();

}

void mainWindow::updateInterface(){
//qDebug()<<video->position();
    circle->setValue(video->position());
    slider_vol->setSliderPosition(video->volume());

}

void mainWindow::moveItemToTop(){
//   int row = table_playlist->selectionModel()->currentIndex().row();

//    if(!selection.isValid())
//        return;

//    int row = selection.row();
//ignore first item
//    if(row==0)
//        return;

//    QList<QStandardItem *> item_list = model_playlist->takeRow(row);
//    model_playlist->insertRow(0, item_list);
//    table_playlist->selectRow(0);


}
void mainWindow::moveItemUp(){
//move up item in playlist
/*int row = table_playlist->selectionModel()->currentIndex().row();

    //ignore if empty
  if(model_playlist->rowCount()==0)
      return;

//ignore first item
    if(row==0)
        return;

    QList<QStandardItem *> item_list = model_playlist->takeRow(row);
    model_playlist->insertRow(row-1, item_list);

    //set new current row
    table_playlist->setCurrentIndex(model_playlist->index(row-1,0));*/
}

void mainWindow::moveItemDown(){
//move down item in playlist
    int row = playlist_widget->selectionModel()->currentIndex().row();

    //ignore if empty
    if(playlist_widget->count()==0)
        return;

////ignore last item
    if(row==playlist_widget->count()-1)
        return;

    QListWidgetItem * i = new QListWidgetItem;
            i = playlist_widget->currentItem()->clone();
    songitemwidget *siw = new songitemwidget;
           siw =(songitemwidget*) playlist_widget->itemWidget(playlist_widget->currentItem());
           qDebug()<<siw->song()->getTitle()<<"test";

//    QPushButton *but = new QPushButton("BUT");

//    QListWidgetItem *i = new QListWidgetItem;
    playlist_widget->addItem(i);
    playlist_widget->setItemWidget(i, siw);



////    playlist_widget->insertItem(row+1, i);
//           playlist_widget->addItem(i);
////    playlist_widget->ins
//    playlist_widget->setItemWidget(i, siw);
////    delete playlist_widget->currentItem();

//    playlist_widget->setCurrentRow(row+1);
//////atek row
//    QList<QStandardItem *> item_list = model_playlist->takeRow(row);
////insert row
//    model_playlist->insertRow(row+1, item_list);

//    //set new current row
//     table_playlist->setCurrentIndex(model_playlist->index(row+1,0));

}

void mainWindow::setBackground(){

        QPixmap bg(background);
        bg=bg.scaled(desktop->size(),Qt::IgnoreAspectRatio,Qt::FastTransformation);

        QPalette pal;
        pal.setColor(QPalette::Window,Qt::black);
        pal.setColor(QPalette::WindowText,Qt::white);
        pal.setBrush(QPalette::Background, bg);
//        pal.setColor(QPalette::Button, Qt::transparent);

        setPalette(pal);
}

void mainWindow::clearPlaylist(){

//    model_playlist->setRowCount(0);
    playlist_widget->clear();

}

void mainWindow::deleteItemPlaylist(){

    qDeleteAll(playlist_widget->selectedItems());

}

void mainWindow::playPlayer(){
    qDebug()<<"enter pla player";

    if(playlist_widget->count()==0){
        video->close();
       clearMask();

        return;
    }


////STOP IF PLAYING
            if(video->isPlaying()){
                video->stop();
            }
           songitemwidget *item_widget = qobject_cast<songitemwidget*>(playlist_widget->itemWidget(playlist_widget->currentItem()));
qDebug()<<"setelah item eidget";
            int id = item_widget->song()->getId();
            QString file= item_widget->song()->getPath();
            QString title = item_widget->song()->getTitle();
            QString singer = item_widget->song()->getSinger();
//            channel = model_playlist->item(cur,4)->text();


            //CHECK IF FILE EXIST
            if (!QFile(file).exists()){

               QMessageBox message;
               message.setIcon(QMessageBox::Information);
               message.setInformativeText(tr("File is not found, maybe drive is not mounted"));
               message.setWindowFlags(Qt::FramelessWindowHint);
               message.setStandardButtons(QMessageBox::Close);
               message.exec();

               return;
           }

           video->setFile(file);

    cover->setData(title, singer);
    qDebug()<<"setealah setdata";
//    cover->raise();
    cover->start();
qDebug()<<cover->isVisible();
           db->updatePlayedTime(id);
           sql_model->select();
////tableRule();

            if(button_lock_playlist->isChecked()){
//                //move to last
//                QList<QStandardItem *> item_list = model_playlist->takeRow(cur);
//                model_playlist->insertRow(model_playlist->rowCount(), item_list);

            }else{
                deleteItemPlaylist();
            }

            if(playlist_widget->count()>1){
                playlist_widget->setCurrentRow(0);
}

}

void mainWindow::setaudiochannelAuto(){

        //wait while video get fully loaded but this break gui
        this_thread::sleep_for(chrono::milliseconds(100));

        if(channel.contains("LEFT", Qt::CaseInsensitive)){
            video->setAudioChannelLeft();
            button_audio_channel->setIcon(QIcon(":/usr/share/elroke/file/icon/left.png"));

        }
        else if(channel.contains("RIGHT", Qt::CaseInsensitive)){
            video->setAudioChannelRight();
            button_audio_channel->setIcon(QIcon(":/usr/share/elroke/file/icon/right.png"));
        }

}

void mainWindow::errorHandling(){

    QMessageBox *error_message = new QMessageBox(this);
    error_message->setInformativeText("Error playing "+video->getFile());
    error_message->show();

    QTimer::singleShot(3000, error_message,SLOT(deleteLater()));

}

void mainWindow::dialogNextSong()
{

    // this will notify next item in playlist before song end


    QDialog *dia = new QDialog;

    QPalette let;
    let.setColor(QPalette::Background,Qt::white);
    let.setColor(QPalette::Foreground,Qt::black);

    QFont f;
    f.setPointSize(32);

    QLabel *notif = new QLabel(dia);

            if(playlist_widget->count()==0)
                 notif->setText(tr("Playlist Is Empty"));
          else{
                songitemwidget * widget_song = qobject_cast<songitemwidget*>(playlist_widget->itemWidget(playlist_widget->item(0)));
                QString title = widget_song->song()->getTitle();
                notif->setText(tr("Next song : ")+title );
            }

    QHBoxLayout *layout_main = new QHBoxLayout;
    dia->setWindowFlags(Qt::FramelessWindowHint);

    dia->setPalette(let);
    dia->setFont(f);
    layout_main->addWidget(notif);
    dia->setLayout(layout_main);
    dia->adjustSize();
    dia->move((desktop_width/2) - dia->rect().center().x(),0);
    dia->setAttribute(Qt::WA_DeleteOnClose);
    QTimer::singleShot(5000, dia, SLOT(close()));
    dia->show();
    qDebug()<<"enter next";

}


bool mainWindow::eventFilter(QObject *target, QEvent *event)
{

    if(target==video){

    if(event->type()==QEvent::KeyPress ){

            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

            if(isKeyValid(keyEvent->key())){
                    this->activateWindow();
                    this->setMask(getMaska());
                }

           le_search->setFocus();
           le_search->setText(keyEvent->text());


     }

    //if video is playing, user click, mainwindow will be shown
 if(event->type()==QEvent::MouseButtonPress){

     if(!main_window_visible){
         this->activateWindow();
         this->setMask(getMaska());
         main_window_visible=true;
     }
     else{
         video->activateWindow();
         main_window_visible=false;}
      }
}

        return QObject::eventFilter(target, event);
}

void mainWindow::delayHideWindow()
{

    qDebug()<<"tred started";

}

void mainWindow::delayEvent(){

    on_delay=false;
    qDebug()<<"delay event"<< on_delay;
    if(on_delay)
        return;
    this->hide();

}

void mainWindow::dialogAddToDatabase(){

    addtodatabase atd;

    connect(&atd,SIGNAL(accepted()),this,SLOT(tableRule()));
    connect(&atd,SIGNAL(accepted()),this,SLOT(getCategory()));
    atd.exec();

}

void mainWindow::dialogSavePlaylist(){

    QDialog *dialog_save_playlist = new QDialog;
dialog_save_playlist->setParent(this);
    QVBoxLayout *layout_main = new QVBoxLayout;

    CLineEdit *le_playlist_name = new CLineEdit(dialog_save_playlist);
    connect(le_playlist_name,SIGNAL(focussed(bool)),keyboard,SLOT(setVisible(bool)));
    QAction *action_delete = new QAction(QIcon(":/usr/share/elroke/file/icon/backspace.png"), "", this);
    le_playlist_name->addAction(action_delete, QLineEdit::TrailingPosition);
    connect(action_delete,&QAction::triggered,le_playlist_name,&QLineEdit::backspace);

    layout_main->addWidget(new QLabel("Save Playlist As :", dialog_save_playlist));
    layout_main->addWidget(le_playlist_name);

    QHBoxLayout *layout_btn = new QHBoxLayout;

    QPushButton *btn_close = new QPushButton("Cancel", dialog_save_playlist);
    connect(btn_close,SIGNAL(clicked(bool)),dialog_save_playlist,SLOT(close()));

    QPushButton *btn_save = new QPushButton("Save", dialog_save_playlist);
    connect(btn_save,SIGNAL(clicked(bool)),dialog_save_playlist,SLOT(accept()));
//    Keyboard *key = new Keyboard(dialog_save_playlist);
//    layout_main->addWidget(key);
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
   qDebug()<<mapToGlobal(dialog_save_playlist->pos());

   if(dialog_save_playlist->exec()==QDialog::Accepted){

       writePlaylist(le_playlist_name->text());

   }

   }

void mainWindow::dialogLoadPlaylist(){

    auto *dialog_load_playlist = new QDialog(this);

    QDirIterator it(QDir::homePath()+"/.elroke/playlist", QDir::Files | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);

    QList<QString> list_play;

    QFileInfo info;

    while(it.hasNext()){

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
    connect(btn_cancel,SIGNAL(clicked(bool)),dialog_load_playlist,SLOT(close()));

    auto *btn_load = new QPushButton("Load", dialog_load_playlist);
    connect(btn_load,SIGNAL(clicked(bool)),dialog_load_playlist,SLOT(accept()));

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


    if(dialog_load_playlist->exec()==QDialog::Accepted){

        loadPlaylist(_list->currentItem()->text());
    }
}
void mainWindow::writePlaylist(){

    writePlaylist("playlist");

}

void mainWindow::writePlaylist(const QString &playlistname){

    if(playlistname==NULL)
        return;

    QString dir_playlist = QDir::homePath()+"/.elroke/playlist";

    if(!QDir(dir_playlist).exists())
        QDir().mkpath(dir_playlist);

    QFile file(dir_playlist+"/"+playlistname+".elp");

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug()<<"cant open playlist file";
    }
     QTextStream stream(&file);

     stream << "[elroke playlist]\n";

//     for(int i=0; i<model_playlist->rowCount(); i++){
//            for (int x=0; x<5; x++){
//                stream<<model_playlist->item(i,x)->text();
//                if(x<=3)
//                    stream<<'\t';

//            }
//            stream<<'\n';
//     }

     file.close();

}

void mainWindow::loadPlaylist(){
//default saved playlist
    loadPlaylist("playlist");

}

void mainWindow::loadPlaylist(const QString &s){

////    /*model_playlist->setRowCount(0);


//    QFile file(QDir::homePath()+"/.elroke/playlist/"+s+".elp");

//    if(!file.exists())
//        return;

//    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
//        qDebug() << "cant read playlist" << s;

//    QTextStream stream(&file);

//    QString stuff= stream.readLine();
//    //validate first line
//    if(stuff!="[elroke playlist]")
//        return;


//    stuff= stream.readLine();//move to next line
//    int line_number=0;

//    QStandardItem *item_id, *item_path, *item_title, *item_singer, *item_channel;

//    while(stuff!=NULL){
//        QString id=stuff.split('\t').at(0);
//        QString path=stuff.split('\t').at(1);
//        QString title=stuff.split('\t').at(2);
//        QString singer=stuff.split('\t').at(3);
//        QString channel=stuff.split('\t').at(4);
////        model_playlist->setRowCount(line_number+1);

//        item_id =  new QStandardItem;
//        item_id->setText(path);
//        model_playlist->setItem(line_number,0, item_id);

//        item_path =  new QStandardItem;
//        item_path->setText(path);
//        model_playlist->setItem(line_number,1, item_path);

//        item_title = new QStandardItem;
//        item_title->setText(title);
//        item_title->setTextAlignment(Qt::AlignCenter);
//        model_playlist->setItem(line_number,2,item_title);

//        item_singer = new QStandardItem;
//        item_singer->setText(singer);
//        model_playlist->setItem(line_number,3,item_singer);

//        item_channel = new QStandardItem;
//        item_channel->setText(channel);
//        model_playlist->setItem(line_number,4, item_channel);

//         line_number++;
//         stuff= stream.readLine();
//    }

//    table_playlist->selectRow(0);*/

}


QRegion mainWindow::getMaska(){

    int   free_top_point = spacer->mapToGlobal(spacer->rect().topLeft()).y();
    int   free_bottom_point = spacer->mapToGlobal( spacer->rect().bottomLeft()).y();
    QRegion r(0,free_top_point,desktop_width,free_bottom_point-free_top_point, QRegion::Rectangle);
    QRegion desk (0,0, desktop_width, desktop_height, QRegion::Rectangle);
    return desk.subtracted(r);

}


void mainWindow::dialogEditDatabase(){

    //sql_model must be inActive before editing
    sql_model->clear();

    managedb md;
    connect(&md, SIGNAL(finished(int)),this,SLOT(tableRule()));
    md.exec();

}


void mainWindow::setAudioChannelManual(){

    switch(video->getAudioChannel()){
    case 1://stereo
        video->setAudioChannelRight();
        button_audio_channel->setIcon(QIcon(":/usr/share/elroke/file/icon/right.png"));
        break;
    case 3://left
        video->setAudioChannelStereo();
        button_audio_channel->setIcon(QIcon(":/usr/share/elroke/file/icon/stereo.png"));
        break;
    case 4://right
        video->setAudioChannelLeft();
        button_audio_channel->setIcon(QIcon(":/usr/share/elroke/file/icon/left.png"));
        break;

    }

}


void mainWindow::tableRule(){

    //rebuild rule after sql_model->clear();
    //is there better way to update table?
    sql_model->setTable("ELROKE123");
    sql_model->setSort(1,Qt::AscendingOrder);
    sql_model->select();
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

}

bool mainWindow::isKeyValid(int key){

    QList <int> validKey;
    //this enum key 0-9a-zspace
    validKey<<0x30<<0x31<<0x32<<0x33<<0x34<<0x35<<0x36<<0x37<<0x38<<0x39<<0x41<<0x42<<0x43
                   <<0x44<<0x45<<0x46<<0x47<<0x48<<0x49<<0x4a<<0x4b<<0x4c<<0x4d<<0x4e<<0x4f<<0x50
                   <<0x51<<0x52<<0x53<<0x54<<0x55<<0x56<<0x57<<0x58<<0x59<<0x5a<<0x20;

    if(validKey.contains(key))
        return true;

    return false;


}

void mainWindow::showKeyboard(bool x){


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
    video = new Player;
    video->hide();

    video->installEventFilter(this);

    connect(video, SIGNAL(positionChanged()),this,SLOT(updateInterface()));
    connect(video,SIGNAL(reachEnded()),this,SLOT(videoEnds()));
    connect(video,SIGNAL(almostEnded()),this,SLOT(dialogNextSong()));
    connect(video,SIGNAL(playing()),this,SLOT(setaudiochannelAuto()));
//    connect(video,SIGNAL(playing()),video,SLOT(show()));
    connect(video,SIGNAL(error()),this,SLOT(errorHandling()));
//    connect(slider_pos,SIGNAL(sliderMoved(int)),video,SLOT(changePosition(int)));
    connect(slider_vol,&QSlider::sliderMoved,video,&Player::setVolume);
}

void mainWindow::keyBoardInstance(){
    keyboard = new Keyboard(this);

    keyboard->hide();
}

void mainWindow::fontSetup(){

    //set font
//    QFontDatabase basefont;
//    int fontindex = basefont.addApplicationFont(":/usr/share/elroke/file/font/Roboto/Roboto-Black.ttf");
////    basefont

//    if(fontindex==-1)
//        qDebug()<<"Failed to load custom font";

    QFont font;//= basefont.font("Roboto-Black", "normal", 16);
    font.setPointSize(16);
    font.setFamily("Roboto");
    setFont(font);


}

void mainWindow::updateLockButton(bool lock){

    if(lock)
        button_lock_playlist->setIcon(QIcon(":/usr/share/elroke/file/icon/unlock.png"));
    else
         button_lock_playlist->setIcon(QIcon(":/usr/share/elroke/file/icon/lock.png"));
}

void mainWindow::dialogAdmin(){

    QDialog *dialog_admin = new QDialog;

    QVBoxLayout *layout_main = new QVBoxLayout;

    auto *button_add_to_database = new QPushButton(tr("ADD TO DATABASE"), dialog_admin);
    connect(button_add_to_database,SIGNAL(pressed()),this,SLOT(dialogAddToDatabase()));

    auto *button_manage_database = new QPushButton(tr("MANAGE DATABASE"), dialog_admin);
    connect(button_manage_database,SIGNAL(pressed()),this,SLOT(dialogEditDatabase()));

    auto *button_preferences = new QPushButton(tr("PREFERENCES"), dialog_admin);

    auto *button_about = new QPushButton(tr("ABOUT"), dialog_admin);
    connect(button_about,SIGNAL(pressed()),this,SLOT(dialogAbout()));

    auto *button_close = new QPushButton(tr("CLOSE"), dialog_admin);
    connect(button_close,SIGNAL(pressed()),dialog_admin,SLOT(close()));

    auto *button_exit = new QPushButton(tr("QUIT APP"), dialog_admin);
    connect(button_exit,SIGNAL(pressed()),dialog_admin,SLOT(close()));
    connect(button_exit,SIGNAL(pressed()),this,SLOT(close()));

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
    dialog_admin->setParent(this);
    dialog_admin->adjustSize();
    dialog_admin->setAttribute(Qt::WA_DeleteOnClose);
    dialog_admin->show();

}

void mainWindow::checkAdmin(){

    qDebug()<<"check admin";

    QSettings settings("elroke","elroke");
    QString user;
    settings.beginGroup(group_auth);
    user=settings.value("username").toString();
    settings.endGroup();

    if(user==NULL)
        dialogCreateAdmin();
    else
        dialogLogin();

}

void mainWindow::dialogCreateAdmin(){

    QDialog *dialog = new QDialog;
    QVBoxLayout *layout_main = new QVBoxLayout;

    le_userName = new CLineEdit(dialog);

    le_password = new CLineEdit(dialog);
    le_password->setEchoMode(QLineEdit::Password);

    le_password_confirm = new CLineEdit(dialog);
    le_password_confirm->setEchoMode(QLineEdit::Password);

   QHBoxLayout *layout_button = new QHBoxLayout;
    auto *button_create_admin = new QPushButton(tr("Create Administrator"), dialog);
    connect(button_create_admin,SIGNAL(pressed()),this,SLOT(createAdminAccount()));

    auto *button_close = new QPushButton("Close", dialog);
    connect(button_close,SIGNAL(pressed()),dialog,SLOT(close()));

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
     connect(this,SIGNAL(usernameCreated()),dialog,SLOT(close()));
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

void mainWindow::dialogLogin(){

    QDialog *dialog = new QDialog;
    QVBoxLayout *layout_main = new QVBoxLayout;

    le_userName = new CLineEdit(dialog);


    le_password = new CLineEdit(dialog);
    le_password->setEchoMode(QLineEdit::Password);

    auto *button_close = new QPushButton(tr("Close"), dialog);
    connect(button_close,SIGNAL(pressed()),dialog,SLOT(close()));

    auto *button_login = new QPushButton(tr("Login"), dialog);

    connect(le_userName,SIGNAL(returnPressed()),button_login,SLOT(click()));
    connect(le_password,SIGNAL(returnPressed()),button_login,SLOT(click()));
    connect(button_login,SIGNAL(pressed()),this,SLOT(login()));

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
     connect(this,SIGNAL(loginAccepted()),dialog,SLOT(close()));

     dialog->adjustSize();
     dialog->setAttribute(Qt::WA_DeleteOnClose);
       le_userName->setFocus();
     dialog->show();


}

void mainWindow::createAdminAccount(){

QSettings setting("elroke","elroke");

QString user = le_userName->text();
QString pass = le_password->text();
QString pass_confirm = le_password_confirm->text();

if(user.isEmpty() || pass.isEmpty() || pass_confirm.isEmpty()){
    return;
}

if(pass!=pass_confirm){
    return;
}

    setting.beginGroup(group_auth);
    setting.setValue("username", QCryptographicHash::hash(user.toUtf8(), QCryptographicHash::Sha1));
    setting.setValue("password", QCryptographicHash::hash(pass.toUtf8(), QCryptographicHash::Sha1));
    setting.endGroup();

    emit usernameCreated();
    dialogLogin();

}

void mainWindow::login(){

    QSettings settings("elroke","elroke");
    QByteArray user;
    QByteArray password;
    settings.beginGroup(group_auth);
    user=settings.value("username").toByteArray();
    password=settings.value("password").toByteArray();
    settings.endGroup();

   QByteArray u  = QCryptographicHash::hash(le_userName->text().toUtf8(), QCryptographicHash::Sha1);
   QByteArray p = QCryptographicHash::hash(le_password->text().toUtf8(), QCryptographicHash::Sha1);

    if(u==user && p==password){
     emit loginAccepted();
        dialogAdmin();
    }


}

void mainWindow::showHits(){

//    proxy_model->sort(7, ProxyModel::sortRole, Qt::AscendingOrder);
//proxy_model->setSortRole(ProxyModel::sortRole);
proxy_model->sort(6, Qt::DescendingOrder);




}
mainWindow::~mainWindow()
{
    delete video;
    delete cover;
    if(autosave_playlist->isChecked())
        writePlaylist();

}

int mainWindow::getRandomNumber(){

//    max 100 min 55

    return (qrand()%46)+60;
}

void mainWindow::videoEnds(){

    auto *dialog_random_number = new QDialog;
    dialog_random_number->setAttribute(Qt::WA_DeleteOnClose);
    dialog_random_number->setWindowFlags(Qt::FramelessWindowHint);

    QLabel *value_label = new QLabel(dialog_random_number);

    effect_player->setFile("/home/gafi/applause.mp3");
    effect_player->play();
    connect(effect_player,SIGNAL(reachEnded()),dialog_random_number,SLOT(close()));
    connect(effect_player,SIGNAL(reachEnded()),this,SLOT(playPlayer()));

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

void mainWindow::audioEffectInstance(){

    effect_player = new Player(this);
    effect_player->setVisible(0);

}

void mainWindow::randomNumberInstance(){
    //need to generate random number, for random scoring.
      qsrand(static_cast<uint>(QTime::currentTime().msec()));
}


void mainWindow::openingInstance(){

    cover = new opening();
    connect(cover,SIGNAL(passed()),video,SLOT(show()));
    connect(cover,SIGNAL(passed()),video,SLOT(play()));

}

Song* mainWindow::extractSong(){

    Song * the_song = qobject_cast<Song*>(playlist_widget->itemWidget(playlist_widget->currentItem()));

    return the_song;


}

void mainWindow::moveItemToBottom(){




}
