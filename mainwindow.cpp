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
#include "editdatabse.h"
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
#include <QCryptographicHash>
#include <QSettings>

mainWindow::mainWindow(QWidget *parent)
    : QDialog(parent)
{
    readSettings();
    fontSetup();
    createWidgets();
    keyBoardInstance();
    videoInstance();
    scoringInstance();
    audioEffectInstance();
    createShortcut();
    setBackground();

    getCategory();
    setWindowFlags(Qt::FramelessWindowHint);
}

void mainWindow::createWidgets()
{
    resize(desktop_width, desktop_height);
    auto layout_main = new QVBoxLayout;
   auto layout_top = new QHBoxLayout;

    auto pb_menu = new QPushButton(QIcon(":/usr/share/elroke/icon/menu.png"),"", this);
    pb_menu->setFlat(1);
    pb_menu->setFocusPolicy(Qt::NoFocus);
    connect(pb_menu,&QPushButton::pressed,this,&mainWindow::dialogLogin);

    le_search = new CLineEdit(this);
    le_search->setPlaceholderText(tr("SEARCH"));

    QPalette button_cat_palette;
    button_cat_palette.setColor(QPalette::Button, QColor("#80cbc4"));
   button_cat_palette.setColor(QPalette::ButtonText,Qt::white);

    auto cat_button = [this,button_cat_palette](const QString &title){
        auto button = new QPushButton(title, this);
        button->setFocusPolicy(Qt::NoFocus);
        button->setFlat(1);
        button->setAutoFillBackground(1);
        button->setPalette(button_cat_palette);
        return button;
    };

    auto button_show_all = cat_button(tr("ALL"));

    connect(button_show_all,&QPushButton::pressed,[this]()
    {
        proxy_model->search("");
    });

    auto button_fav = cat_button(tr("Favorite"));
    connect(button_fav,&QPushButton::pressed,[this]()
    {
        proxy_model->filterFavorite();
    });

    auto button_cat_indonesia = cat_button("JAZZ");

    auto button_cat_barat = cat_button("WESTERN");

    auto button_cat_rock = cat_button("ROCK");

    auto button_cat_pop = cat_button("POP");

    auto button_cat_dangdut = cat_button("TRADITIONAL");

//show clock
    auto clock = new QLabel(this);
    clock->setText(QTime::currentTime().toString("hh:mm"));
    auto timer = new QTimer(this);
    connect(timer,&QTimer::timeout,[clock]()
    {
          clock->setText(QTime::currentTime().toString("hh:mm"));
    });
    timer->start(60000);

    layout_top->addWidget(pb_menu);
    layout_top->addWidget(le_search);
    layout_top->addStretch();
    layout_top->addWidget(button_show_all);
    layout_top->addWidget(button_fav);
    layout_top->addWidget(button_cat_indonesia);
    layout_top->addWidget(button_cat_barat);
    layout_top->addWidget(button_cat_rock);
    layout_top->addWidget(button_cat_pop);
    layout_top->addWidget(button_cat_dangdut);
    layout_top->addStretch();
    layout_top->addWidget(clock);
    layout_top->setSpacing(2);
    layout_top->setMargin(0);

    auto widget_top = new QWidget(this);
    QPalette top_widget_palette;
    top_widget_palette.setBrush(QPalette::Background, QColor("#00796B"));

    widget_top->setAutoFillBackground(1);
    widget_top->setPalette(top_widget_palette);
    widget_top->setLayout(layout_top);

   auto layout_table = new QHBoxLayout;

    db = new dbmanager(dbmanager::show, this);
    db->connectToDB();

    sql_model = new QSqlTableModel(this, db->database());
    sql_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    sql_model->setTable("ELROKE123");
    sql_model->select();

    while (sql_model->canFetchMore()) {
        sql_model->fetchMore();
    }

    proxy_model = new ProxyModel( this);
    proxy_model->setSourceModel(sql_model);
    proxy_model->setAlignment(1, Qt::AlignLeft | Qt::AlignVCenter);
    proxy_model->setAlignment(2, Qt::AlignRight | Qt::AlignVCenter);

    table = new QTableView(this);
    table->setModel(proxy_model);

//    sql_model->setSort(2,Qt::DescendingOrder);
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
    table->hideColumn(9);
//    table->horizontalHeader()->setSectionsClickable(false);
    table->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    table->model()->setHeaderData(1, Qt::Horizontal,Qt::AlignLeft, Qt::TextAlignmentRole);
    table->model()->setHeaderData(2, Qt::Horizontal,Qt::AlignRight, Qt::TextAlignmentRole);
    table->horizontalHeader()->setHighlightSections(0);
    table->setItemDelegate(new NoFocusDelegate());
    table->setSortingEnabled(1);
    proxy_model->sort(1,Qt::AscendingOrder);

    QPalette header_palette = table->horizontalHeader()->palette();
    header_palette.setColor(QPalette::Button, QColor("#009688"));
    header_palette.setColor(QPalette::ButtonText, Qt::white);
    header_palette.setColor(QPalette::Background, Qt::white);
    table->horizontalHeader()->setPalette(header_palette);

//space vertical each item prevent too close beetween items
    QHeaderView *vertical = table->verticalHeader();
    vertical->setDefaultSectionSize(vertical->fontMetrics().height()+10);

    connect(table,&QTableView::doubleClicked,this,&mainWindow::addToPlaylist);
    connect(le_search,&CLineEdit::focussed,this,&mainWindow::showKeyboard);
    connect(le_search,&CLineEdit::textChanged,proxy_model,&ProxyModel::search);

    QSizePolicy spLeft(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spLeft.setHorizontalStretch(3);
    table->setSizePolicy(spLeft);

    auto layout_playlist = new QVBoxLayout;

    playlist_widget = new ListWidget(this);
    playlist_widget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    playlist_widget->setDragEnabled(true);
    playlist_widget->viewport()->setAcceptDrops(true);
    playlist_widget->setDropIndicatorShown(true);
    playlist_widget->setDragDropMode(QAbstractItemView::InternalMove);
    playlist_widget->setAutoScroll(1);
    playlist_widget->setItemDelegate(new itemDelegate);

    connect(playlist_widget,&ListWidget::doubleClicked,this,&mainWindow::playPlayer);

    auto layout_button_playlist = new QHBoxLayout;

    auto playlistButton = [this](QIcon icon){
        auto button = new QPushButton(this);
        button->setIcon(icon);
        button->setIconSize(QSize(32,32));
        button->setFocusPolicy(Qt::NoFocus);
        button->setFlat(true);
        return button;
    };

    auto button_menu = playlistButton(QIcon(":/usr/share/elroke/icon/menu.png"));

    auto menu_playlist = new QMenu(this);
    autosave_playlist = new QAction(tr("Auto save playlist"), this);
    autosave_playlist->setFont(font());
    autosave_playlist->setCheckable(1);
    autosave_playlist->setChecked(1);
    //load playlist
    if (autosave_playlist->isChecked())
        loadPlaylist();

    auto save_as = new QAction(tr("Save as"),  this);
    save_as->setFont(font());
    connect(save_as,&QAction::triggered,this,&mainWindow::dialogSavePlaylist);

    auto load_playlist = new QAction(tr("Load playlist") , this);
    load_playlist->setFont(font());
    connect(load_playlist,&QAction::triggered,this,&mainWindow::dialogLoadPlaylist);

    menu_playlist->addAction(autosave_playlist);
    menu_playlist->addAction(save_as);
    menu_playlist->addAction(load_playlist);

    button_menu->setMenu(menu_playlist);

    auto button_move_to_top = playlistButton( QIcon(":/usr/share/elroke/icon/top.png"));
    connect(button_move_to_top,&QPushButton::pressed,this,&mainWindow::moveItemToTop);

    auto button_move_to_bottom = playlistButton(QIcon(":/usr/share/elroke/icon/bottom.png"));
     connect(button_move_to_bottom,&QPushButton::pressed,this,&mainWindow::moveItemToBottom);

    auto button_delete = playlistButton(QIcon(":/usr/share/elroke/icon/delete.png"));
    connect(button_delete,&QPushButton::pressed,[this]()
    {
        qDeleteAll(playlist_widget->selectedItems());
    });

    auto button_clear_playlist = playlistButton(QIcon(":/usr/share/elroke/icon/clear.png"));
    connect(button_clear_playlist,&QPushButton::pressed,[this]()
    {
         playlist_widget->clear();
    });

    auto button_lock_playlist = playlistButton(QIcon(":/usr/share/elroke/icon/lock.png"));
    button_lock_playlist->setCheckable(true);
    connect(button_lock_playlist,&QPushButton::clicked,[this, button_lock_playlist](bool lock)
    {
        if(playlist_widget->count()==0)
            return;

        lock_playlist = lock;
        if (lock)
        {
            button_lock_playlist->setIcon(QIcon(":/usr/share/elroke/icon/unlock.png"));
        } else{
             button_lock_playlist->setIcon(QIcon(":/usr/share/elroke/icon/lock.png"));
        }
    });

    auto widget_control_playlist = new QWidget(this);
    widget_control_playlist->setAutoFillBackground(1);

    layout_button_playlist->addWidget(button_menu);
    layout_button_playlist->addWidget(button_delete);
    layout_button_playlist->addWidget(button_clear_playlist);
    layout_button_playlist->addWidget(button_move_to_top);
    layout_button_playlist->addWidget(button_move_to_bottom);
    layout_button_playlist->addWidget(button_lock_playlist);
    layout_button_playlist->setSpacing(0);
    layout_button_playlist->setMargin(0);

    widget_control_playlist->setLayout(layout_button_playlist);
    QPalette palt;
    palt.setBrush(QPalette::Background, QColor("#009688"));
    widget_control_playlist->setPalette(palt);

    layout_playlist->addWidget(widget_control_playlist);
    layout_playlist->addWidget(playlist_widget);
    layout_playlist->setSpacing(0);
    layout_playlist->setMargin(0);

    //CREATE WIDGET TO APPLY SIZEPOLICY
    auto widget_playlist = new QWidget(this);
    widget_playlist->setLayout(layout_playlist);

    QSizePolicy spRight(QSizePolicy::Preferred,QSizePolicy::Preferred);
    spRight.setHorizontalStretch(1);
    widget_playlist->setSizePolicy(spRight);

    layout_table->addWidget(table);
    layout_table->addWidget(widget_playlist);
    layout_table->setMargin(0);
    layout_table->setSpacing(0);

    //CREATE WIDGET TO APPLY SIZEPOLICY(Height)
    auto all_table = new QWidget(this);
    all_table->setLayout(layout_table);

    QSizePolicy spTable (QSizePolicy::Expanding,QSizePolicy::Expanding);
    spTable.setVerticalStretch(2);
    all_table->setSizePolicy(spTable);

     auto layout_player_control = new QHBoxLayout;

    spinner_progress = new spinnerProgress(this);
    spinner_progress->setFixedSize(64,64);

    auto buttonControl = [this](QIcon icon)
    {
        auto btn= new QPushButton(this);
        btn->setIcon(icon);
        btn->setFlat(1);
        btn->setIconSize(QSize(48,48));
        btn->setFocusPolicy(Qt::NoFocus);
        return btn;
    };

    auto btn_next = buttonControl(QIcon(":/usr/share/elroke/icon/next.png"));
    connect(btn_next,&QPushButton::pressed,this,&mainWindow::playPlayer);

     auto button_play_pause = buttonControl(QIcon(":/usr/share/elroke/icon/play.png"));
     connect(button_play_pause,&QPushButton::pressed,[this]()
     {
         if(video->player()->isPlaying())
             video->player()->pause();
         else if(video->player()->isPausing())
             video->player()->play();

     });

     buttonFavorite = buttonControl(QIcon(":/usr/share/elroke/icon/unfavorite.png"));
     connect(buttonFavorite,&QPushButton::pressed,[this](){

         QString id{video->song()->getId()};
         if(id.isEmpty())
             return;
         db->setFavorite(id);
         handleFavorite();
     });

    button_audio_channel = buttonControl(QIcon(":/usr/share/elroke/icon/vocal_off.png"));
    connect(button_audio_channel,&QPushButton::pressed,this,&mainWindow::setAudioChannelManual);


     auto button_vol_down = buttonControl(QIcon(":/usr/share/elroke/icon/minus.png"));
     connect(button_vol_down,&QPushButton::pressed,[this]()
     {
         if (video->player()->volume()<5)
         {
              video->player()->setVolume(0);
         }  else {
              video->player()->setVolume(video->player()->volume()-5);
         }
     });

     auto button_vol_up = buttonControl(QIcon(":/usr/share/elroke/icon/plus.png"));
     connect(button_vol_up,&QPushButton::pressed,[this]()
     {
         if (video->player()-> volume()>95)
         {
             video->player()->setVolume(100);
         } else {
          video->player()->setVolume(video->player()->volume()+5);
         }
     });

    auto  button_audio_mute = buttonControl(QIcon(":/usr/share/elroke/icon/unmute.png"));
     connect(button_audio_mute,&QPushButton::pressed,[this,button_audio_mute]()
     {
         if (!video->player()->isPlaying())
             return;

         if (video->player()->isMute())
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

        layout_player_control->addWidget( spinner_progress);
        layout_player_control->addStretch();
        layout_player_control->addWidget(btn_next);
        layout_player_control->addSpacing(48);
        layout_player_control->addWidget(button_play_pause);
        layout_player_control->addSpacing(48);
        layout_player_control->addWidget(button_audio_channel);
        layout_player_control->addSpacing(48);
        layout_player_control->addWidget(buttonFavorite);
        layout_player_control->addStretch();
        layout_player_control->addWidget(button_vol_down);
        layout_player_control->addSpacing(0);
        layout_player_control->addWidget(button_audio_mute);
        layout_player_control->addSpacing(0);
        layout_player_control->addWidget(button_vol_up);
        layout_player_control->setMargin(0);

     auto widget_bottom = new QWidget(this);
     QPalette plt;
     plt.setBrush(QPalette::Background, QColor(255,255,255,200));

      widget_bottom->setAutoFillBackground(1);
     widget_bottom->setPalette(plt);
     widget_bottom->setLayout(layout_player_control);
     widget_bottom->setFixedHeight(72);


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

void mainWindow::createShortcut()
{
    auto sc_quit = new QShortcut(QKeySequence("Esc"),this);
    connect(sc_quit,&QShortcut::activated,qApp,&QApplication::quit);
}
void mainWindow::getCategory()
{
    QList <QString> s = listStringFileParser::parse(app_dir+"/meta/genre");
}

void mainWindow::addToPlaylist()
{
    int row =  proxy_model->mapToSource( table->selectionModel()->currentIndex()).row();
   QString id = sql_model->data(sql_model->index(row,0),Qt::DisplayRole).toString();

   Song *song =   db->getSong(id);
   songitemwidget *item_song_widget = new songitemwidget;
   item_song_widget->setSong(song);


   auto item = new QListWidgetItem;
   playlist_widget->addItem(item);
   playlist_widget->setItemWidget(item, item_song_widget);

//resize itemwidget to songitemwidget
   item->setSizeHint(QSize(playlist_widget->width()-qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent), item_song_widget->height()));

   playlist_widget->setCurrentRow(playlist_widget->count()-1);
}

void mainWindow::keyPressEvent(QKeyEvent *event)
{
  //IF USER TYPE USER TYPE NUMBER OR ALPHABET DIRECT TO LINEEDIT SEARCH
    if (isKeyValid(event->key()))
    {
        le_search->setFocus();
        le_search->setText(event->text());
    }

    if (event->key()==Qt::Key_Enter || event->key()==Qt::Key_Return)
    {
        //send item to playlist with enter key
        if (focusWidget()==table)
        {
            addToPlaylist();
        }
        //play item with enter key
         else if (focusWidget()==playlist_widget)
        {
            playPlayer();
        }
    }

        //move focus widget from table to playlist with right key
        else if (event->key()==Qt::Key_Right && qApp->focusWidget()==table)
        {
            playlist_widget->setFocus();
        }

        //move focus widget from playlist to table with left key
        else if (event->key()==Qt::Key_Left && qApp->focusWidget()==playlist_widget)
        {
            table->setFocus();
        }

        if (event->key()==Qt::Key_Backspace)
        {
            if (le_search->text().isEmpty())
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
    if(playlist_widget->count()==0)
        return;

    int row = playlist_widget->currentIndex().row();
//ignore first item
    if (row==0)
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
        item->setCategory(item_current->song()->getCategory());
        item->setLanguage(item_current->song()->getLanguage());
        item->setAudioChannel(item_current->song()->getAudioChannel());
        item->setPlaytimes(item_current->song()->getPlayTimes());

        auto item_new = new QListWidgetItem;

        auto item_song_widget = new songitemwidget;
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
    bg = bg.scaled(desktop->size(),Qt::IgnoreAspectRatio,Qt::FastTransformation);

    QPalette pal;
    pal.setColor(QPalette::Window,Qt::black);
    pal.setColor(QPalette::WindowText,Qt::white);
    pal.setBrush(QPalette::Background, bg);
    //        pal.setColor(QPalette::Button, Qt::transparent);

    setPalette(pal);
}

void mainWindow::playPlayer()
{
    if (playlist_widget->count()==0)
    {
        if(video->player()->isPlaying())
        {            //if user pres  next button
            video->close();
            clearMask();
        }
        return;
    }

    ////STOP IF PLAYING
    if (video->player()->isPlaying())
        video->player()->stop();

    songitemwidget *item_widget = qobject_cast<songitemwidget*>(playlist_widget->itemWidget(playlist_widget->currentItem()));

    QString id = item_widget->song()->getId();
    QString file= item_widget->song()->getPath();
    QString title = item_widget->song()->getTitle();
    QString singer = item_widget->song()->getSinger();
    QString channel = item_widget->song()->getAudioChannel();

    //CHECK IF FILE EXIST
    if (!QFile(file).exists())
    {
        QMessageBox message;
        message.setIcon(QMessageBox::Information);
        message.setInformativeText(tr("File is not found."));
        message.setWindowFlags(Qt::Popup);
        message.setStandardButtons(QMessageBox::Close);
        message.exec();
        return;
    }

    video->setData(item_widget->song());
    video->play();

    handleFavorite();
    db->updatePlayedTime(id);

    //lock playlist
    if (lock_playlist)
    {
       moveItemToBottom();
    }    else    {
        qDeleteAll(playlist_widget->selectedItems());
    }

    if (playlist_widget->count()>1)
        playlist_widget->setCurrentRow(0);

}

void mainWindow::dialogNextSong()
{
    auto dialog = new QDialog();

    auto layout =new QVBoxLayout;
     // this will notify next item in playlist before song end
    QPalette let;
    let.setColor(QPalette::Background,Qt::white);
    let.setColor(QPalette::Foreground,Qt::black);

    QFont f;
    f.setPointSize(32);

    auto notif = new QLabel(dialog);
    layout->addWidget(notif);
    dialog->setLayout(layout);

    if (playlist_widget->count()==0)
    {
         notif->setText(tr("Playlist Is Empty"));
    }    else    {
        auto widget_song = qobject_cast<songitemwidget*>(playlist_widget->itemWidget(playlist_widget->item(0)));
        notif->setText(tr("Next song : ")+widget_song->song()->getTitle());
    }

    dialog->setWindowFlags(Qt::Popup);
    dialog->setPalette(let);
    notif->setFont(f);
    dialog->adjustSize();
    dialog->move((desktop_width-dialog->width())/2,0);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    QTimer::singleShot(5000, dialog,&QDialog::close);
    dialog->show();
}

bool mainWindow::eventFilter(QObject *target, QEvent *event)
{
    if (target==video)
    {
        if (event->type()==QEvent::KeyPress )
        {
            auto keyEvent = static_cast<QKeyEvent*>(event);
            if (isKeyValid(keyEvent->key()))
            {
                this->activateWindow();
                this->setMask(getMaska());
                le_search->setFocus();
                le_search->setText(keyEvent->text());
            }
         }

    //if video is playing, user click, mainwindow will be shown
     if (event->type()==QEvent::MouseButtonPress)
     {
         if (!main_window_visible)
         {
             this->activateWindow();
             this->setMask(getMaska());
             main_window_visible=true;
         }  else   {
             video->activateWindow();
             main_window_visible=false;
         }
      }
    }

    return QObject::eventFilter(target, event);
}

void mainWindow::dialogSavePlaylist()
{
    auto dialog_save_playlist = new QDialog;
    auto layout_main = new QVBoxLayout;

    auto le_playlist_name = new CLineEdit(dialog_save_playlist);
    connect(le_playlist_name,&CLineEdit::focussed,keyboard,&Keyboard::setVisible);

    layout_main->addWidget(new QLabel("Save Playlist As :", dialog_save_playlist));
    layout_main->addWidget(le_playlist_name);

    auto layout_btn = new QHBoxLayout;

    auto btn_close = new QPushButton("Cancel", dialog_save_playlist);
    connect(btn_close,&QPushButton::pressed,dialog_save_playlist,&QDialog::close);

    auto btn_save = new QPushButton("Save", dialog_save_playlist);
    connect(btn_save,&QPushButton::pressed,dialog_save_playlist,&QDialog::accept);

    keyboard->move(mapFromGlobal(dialog_save_playlist->geometry().bottomLeft()));

    layout_btn->addWidget(btn_close);
    layout_btn->addStretch();
    layout_btn->addWidget(btn_save);

    layout_main->addLayout(layout_btn);
    dialog_save_playlist->setLayout(layout_main);

   dialog_save_playlist->setWindowFlags(Qt::Popup);
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

    auto dialog_load_playlist = new QDialog;

    QDirIterator it(app_dir+"/playlist", QDir::Files | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
    QList<QString> list_play;
    QFileInfo info;
    while(it.hasNext())
    {
        info.setFile(it.next());
        if (info.suffix()=="elp" && info.baseName()!="playlist" )//exception for "playlist", this is for default /auto save
        list_play << info.baseName();
    }

    auto layout_main = new QVBoxLayout;

    auto _list = new QListWidget(dialog_load_playlist);

    _list->addItems(list_play);

    layout_main->addWidget(new QLabel("Load Playlist :", dialog_load_playlist));

    auto layout_btn = new QHBoxLayout;
    auto btn_cancel = new QPushButton("Cancel", dialog_load_playlist);
    connect(btn_cancel,&QPushButton::pressed,dialog_load_playlist,&QDialog::close);

    auto btn_load = new QPushButton("Load", dialog_load_playlist);
    connect(btn_load,&QPushButton::pressed,dialog_load_playlist,&QDialog::accept);

    layout_btn->addWidget(btn_cancel);
    layout_btn->addStretch();
    layout_btn->addWidget(btn_load);

    layout_main->addWidget(_list);
    layout_main->addLayout(layout_btn);

    dialog_load_playlist->setLayout(layout_main);

    dialog_load_playlist->setWindowFlags(Qt::Popup);
    dialog_load_playlist->setAttribute(Qt::WA_DeleteOnClose);
    dialog_load_playlist->setMinimumSize(300,200);
       dialog_load_playlist->setAutoFillBackground(1);

    connect(dialog_load_playlist,&QDialog::accepted,[this,_list](){
         loadPlaylist(_list->currentItem()->text());
    });

    dialog_load_playlist->exec();
}
void mainWindow::writePlaylist()
{
    writePlaylist("playlist");
}

void mainWindow::writePlaylist(const QString &playlistname)
{
    if (playlistname.isEmpty())
        return;

    QString dir_playlist =app_dir+"/playlist";

    if (!QDir(dir_playlist).exists())
        QDir().mkpath(dir_playlist);

    QFile file(dir_playlist+"/"+playlistname+".elp");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"cant open playlist file";
    }
     QTextStream stream(&file);

     stream << "[elroke playlist]\n";

     for(int i =0;i<playlist_widget->count();i++)
     {
       songitemwidget *item_widget = qobject_cast<songitemwidget*>(playlist_widget->itemWidget(playlist_widget->item(i)));
       stream<<item_widget->song()->getId()<<"\n";
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
    if (!file.exists())
        return;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "cant read playlist" << s;
    return;
    }

    QTextStream stream(&file);
    QString stuff= stream.readLine();
    //validate first line
    if (stuff!="[elroke playlist]")
        return;
    stuff= stream.readLine();
    if (stuff==NULL)     return;

    while(stuff!=NULL)
    {
        auto song =   db->getSong(stuff);
        if (song==nullptr) return;
        auto item_song_widget = new songitemwidget;
        item_song_widget->setSong(song);
        auto item = new QListWidgetItem;
        playlist_widget->addItem(item);
        playlist_widget->setItemWidget(item, item_song_widget);
        //resize itemwidget to songitemwidget
        item->setSizeHint(QSize(playlist_widget->width()-qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent), item_song_widget->height()));
        stuff= stream.readLine();
    }
    playlist_widget->setCurrentRow(0);
}

QRegion mainWindow::getMaska()
{
    unsigned short int   free_top_point = spacer->mapToGlobal(spacer->rect().topLeft()).y();
    unsigned short int   free_bottom_point = spacer->mapToGlobal(spacer->rect().bottomLeft()).y()+1;
    QRegion r(0,free_top_point,desktop_width,free_bottom_point-free_top_point, QRegion::Rectangle);
    QRegion desk (0,0, desktop_width, desktop_height, QRegion::Rectangle);
    return desk.subtracted(r);
}

void mainWindow::setAudioChannelManual()
{
    switch(video->player()->getAudioChannel()){
    case 1://stereo
        video->player()->setAudioChannelRight();
        break;
    case 3://left
        video->player()->setAudioChannelStereo();
        break;
    case 4://right
        video->player()->setAudioChannelLeft();
       break;
    default:
        break;
    }
}

bool mainWindow::isKeyValid(int key)
{
    QList <int> validKey;
    //this enum key 0-9a-zspace
    validKey<<0x30<<0x31<<0x32<<0x33<<0x34<<0x35<<0x36<<0x37<<0x38<<0x39<<0x41<<0x42<<0x43
                   <<0x44<<0x45<<0x46<<0x47<<0x48<<0x49<<0x4a<<0x4b<<0x4c<<0x4d<<0x4e<<0x4f<<0x50
                   <<0x51<<0x52<<0x53<<0x54<<0x55<<0x56<<0x57<<0x58<<0x59<<0x5a<<0x20;

    if (validKey.contains(key))
        return true;

    return false;
}

void mainWindow::showKeyboard(bool x)
{
    if (x){
        int x =(table->width()-keyboard->width())/2;
        int y = table->mapToGlobal(table->rect().bottomLeft()).y()-keyboard->height();
        //key show on the midle of table
         keyboard->showKeyboard(QPoint(x,y));
        keyboard->show();
    }  else{
            keyboard->hide();
    }
}

void mainWindow::videoInstance(){
    //video player
    video = new VideoWidget;
    video->setFullscreenMode();
    video->hide();
    video->installEventFilter(this);

    connect(video->player(),&Player::positionChanged,[this](){
        spinner_progress->setValue(video->player()->position());
    });
    connect(video->player(),&Player::reachEnded,this,&mainWindow::videoEnds);
    connect(video->player(),&Player::reachEnded,spinner_progress,&spinnerProgress::stop);
    connect(video->player(),&Player::almostEnded,this,&mainWindow::dialogNextSong);
    connect(video->player(),&Player::playing,spinner_progress,&spinnerProgress::start);
    connect(video->player(),&Player::playing,this,&mainWindow::setAudioChannelAuto);

    connect(video->player(),&Player::error,[this]()
    {
        QMessageBox error_message;
        error_message.setInformativeText("Error playing "+video->player()->getFile());
        error_message.setWindowFlags(Qt::Popup);
        error_message.exec();
        QTimer::singleShot(3000,&error_message,SLOT(close()));
    });
//    connect(slider_vol,&QSlider::sliderMoved,video->player(),&Player::setVolume);
}

void mainWindow::keyBoardInstance()
{
    keyboard = new Keyboard(this);
    keyboard->setAutoFillBackground(1);
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
    dialog_admin = new QDialog(this);
    dialog_admin->setAutoFillBackground(true);

    auto layout_main = new QVBoxLayout;

    auto button_add_to_database = new QPushButton(tr("ADD TO DATABASE"), dialog_admin);
    connect(button_add_to_database,&QPushButton::pressed,[this]()
    {
    auto atd = new addtodatabase;
    atd->setAttribute(Qt::WA_DeleteOnClose);
//    connect(atd,&addtodatabase::accepted,sql_model,&QSqlTableModel::select);
    connect(atd,&addtodatabase::accepted,[this]()
    {
        sql_model->select();
        while(sql_model->canFetchMore())
        {
            sql_model->fetchMore();
        }
    });
    atd->setModal(true);
    atd->show();
    });

    auto button_manage_database = new QPushButton(tr("MANAGE DATABASE"), dialog_admin);
    connect(button_manage_database,&QPushButton::pressed,[this]()
    {
        managedb *md = new managedb;
        md->setAttribute(Qt::WA_DeleteOnClose);
        connect(md, &managedb::finished,[this](){
            sql_model->select();
            while(sql_model->canFetchMore())
            {
                sql_model->fetchMore();
            }
        });
//        md->setModal(true);
        md->show();
//        managedb md;
//        md.exec();
    });

    auto button_preferences = new QPushButton(tr("PREFERENCES"), dialog_admin);
    connect(button_preferences,&QPushButton::pressed,[this]()
    {
        auto pref = new preferences;
        pref->setAttribute(Qt::WA_DeleteOnClose);
        pref->setModal(true);
        pref->show();
    });

    auto button_change_password = new QPushButton(tr("CHANGE PASSWORD"), dialog_admin);
    connect(button_change_password,&QPushButton::pressed,this,&mainWindow::dialogCreateAdmin);

    auto button_close = new QPushButton(tr("CLOSE"), dialog_admin);
    connect(button_close,&QPushButton::pressed,dialog_admin,&QDialog::close);

    auto button_exit = new QPushButton(tr("QUIT APP"), dialog_admin);
    connect(button_exit,&QPushButton::pressed,dialog_admin,&QDialog::close);
    connect(button_exit,&QPushButton::pressed,qApp,&QApplication::quit);

    layout_main->addWidget(button_add_to_database);
    layout_main->addWidget(button_manage_database);
    layout_main->addWidget(button_preferences);
    layout_main->addWidget(button_change_password);
    layout_main->addWidget(button_close);
    layout_main->addWidget(button_exit);

    dialog_admin->setLayout(layout_main);

    QPalette palet;
    palet.setColor(QPalette::Base, Qt::white);
    palet.setColor(QPalette::Window, Qt::white);
    palet.setColor(QPalette::Text, QColor(0,0,0,128));
    palet.setColor(QPalette::WindowText, QColor(0,0,0,128));
    palet.setColor(QPalette::Button, palette().dark().color());
    palet.setColor(QPalette::ButtonText, Qt::white);

    dialog_admin->setPalette(palet);


    dialog_admin->setAttribute(Qt::WA_DeleteOnClose);
    dialog_admin->setWindowFlags(Qt::FramelessWindowHint);
    dialog_admin->setModal(true);
      dialog_admin->adjustSize();
    dialog_admin->show();

}

void mainWindow::dialogCreateAdmin()
{
   auto dialog = new QDialog;
    auto layout_main = new QVBoxLayout;

   auto le_old_password= new CLineEdit(dialog);
    le_old_password->setEchoMode(QLineEdit::Password);

  auto  le_password = new CLineEdit(dialog);
    le_password->setEchoMode(QLineEdit::Password);

   auto le_password_confirm = new CLineEdit(dialog);
    le_password_confirm->setEchoMode(QLineEdit::Password);

    QHBoxLayout *layout_button = new QHBoxLayout;
    auto button_create_admin = new QPushButton(tr("Apply"), dialog);
    connect(button_create_admin,&QPushButton::pressed,[this,dialog,le_old_password,le_password,le_password_confirm]()
    {

        QString old_pass = le_old_password->text();
        QString pass = le_password->text();
        QString pass_confirm = le_password_confirm->text();

        if (old_pass.isEmpty() || pass.isEmpty() || pass_confirm.isEmpty())
             return;


        if (pass!=pass_confirm)
             return;

                QSettings setting("elroke","elroke");
                setting.beginGroup("Admin");
                QByteArray p = setting.value("pasword").toByteArray();
                if (QString(p).isEmpty())
                    p=QString("elroke").toUtf8();

                if (old_pass!=QString(p))
                       return;

                setting.setValue("password", QCryptographicHash::hash(pass.toUtf8(), QCryptographicHash::Sha1));
                setting.endGroup();
                dialog->close();
    });

    auto button_close = new QPushButton("Close", dialog);
    connect(button_close,&QPushButton::pressed,dialog,&QDialog::close);

    layout_button->addWidget(button_close);
    layout_button->addWidget(button_create_admin);

    layout_main->addWidget(new QLabel(tr("Old Password"),dialog));
    layout_main->addWidget(le_old_password);

    layout_main->addWidget(new QLabel(tr("New Password"),dialog));
    layout_main->addWidget(le_password);
    layout_main->addWidget(new QLabel(tr("Confirm New password"),dialog));
    layout_main->addWidget(le_password_confirm);
    layout_main->addStretch();
    layout_main->addLayout(layout_button);

     dialog->setLayout(layout_main);
     dialog->setWindowFlags(Qt::FramelessWindowHint);

     QPalette palet;
     palet.setColor(QPalette::Base, Qt::white);
     palet.setColor(QPalette::Window, Qt::white);
     palet.setColor(QPalette::Text, QColor(0,0,0,128));
     palet.setColor(QPalette::WindowText, QColor(0,0,0,128));
     palet.setColor(QPalette::Button, palette().dark().color());
     palet.setColor(QPalette::ButtonText, QColor(0,0,0,128));
     dialog->setPalette(palet);
     dialog->adjustSize();
     dialog->setAttribute(Qt::WA_DeleteOnClose);
     dialog->exec();
}

void mainWindow::dialogLogin()
{
    if (dialog_admin!=nullptr)
        return;

    auto dialog = new QDialog;
    auto layout_main = new QVBoxLayout;

    auto le_password = new CLineEdit(dialog);
    le_password->setEchoMode(QLineEdit::Password);

    QByteArray pass;

    QSettings setting("elroke", "elroke");
    setting.beginGroup("Admin");
    pass = setting.value("password").toByteArray();
    setting.endGroup();

    if (QString(pass).isEmpty())
        le_password->setText("elroke");

    auto button_admin = new QPushButton(tr("Administrator"), dialog);
    connect(le_password,&CLineEdit::returnPressed,button_admin,&QPushButton::pressed);
    connect(button_admin,&QPushButton::pressed,[this,dialog,le_password,pass]()
    {
        if (QString(pass).isEmpty() && le_password->text()=="elroke")
        {
            //open
            dialog->close();
            dialogAdmin();
        }  else   {
           QByteArray p = QCryptographicHash::hash(le_password->text().toUtf8(), QCryptographicHash::Sha1);
           if (pass==p)
           {
               //open
               dialog->close();
               dialogAdmin();
           }
        }

    });

    auto button_about = new QPushButton(tr("About"), dialog);
    connect(button_about,&QPushButton::pressed,[this]()
    {
        about About;
        About.exec();
    });

    auto button_close = new QPushButton(tr("Close"), dialog);
    connect(button_close,&QPushButton::pressed,dialog,&QDialog::close);

     layout_main->addWidget(new QLabel(tr("Password, default \"elroke\""),dialog));

    layout_main->addWidget(le_password);
    layout_main->addWidget(button_admin);
    layout_main->addStretch();
    layout_main->addWidget(button_about);
    layout_main->addWidget(button_close);

     dialog->setLayout(layout_main);

     QPalette palet;
     palet.setColor(QPalette::Base, palette().dark().color());
     palet.setColor(QPalette::Window, Qt::white);
     palet.setColor(QPalette::Text, QColor(0,0,0,128));
     palet.setColor(QPalette::WindowText, QColor(0,0,0,128));
     palet.setColor(QPalette::Button, palette().dark().color());
     palet.setColor(QPalette::ButtonText, palette().light().color());
     dialog->setPalette(palet);

     dialog->setMinimumSize(300,500);
     dialog->setAttribute(Qt::WA_DeleteOnClose);
     dialog->setWindowFlags( Qt::Popup);
     le_password->setFocus();
     dialog->show();
}

void mainWindow::showHits()
{
//    proxy_model->sort(7, ProxyModel::sortRole, Qt::AscendingOrder);
//proxy_model->setSortRole(ProxyModel::sortRole);
proxy_model->sort(6,Qt::AscendingOrder);
}

void mainWindow::videoEnds()
{
    scoring->start();
}

void mainWindow::audioEffectInstance()
{
    effect_player = new Player(this);
}

void mainWindow::scoringInstance()
{
    scoring = new Scoring();
    connect(scoring,&Scoring::finished,this,&mainWindow::playPlayer);
}

void mainWindow::moveItemToBottom()
{
    if(playlist_widget->count()==0)
        return;

    int row = playlist_widget->currentIndex().row();

 //ignore first item
     if (row==0)
         return;

 //copy song. i'm not sure with this way
         Song *item = new Song;
         songitemwidget *item_current = qobject_cast<songitemwidget*>(playlist_widget->itemWidget(playlist_widget->currentItem()));
         item->setId(item_current->song()->getId());
         item->setTitle(item_current->song()->getTitle());
         item->setSinger(item_current->song()->getSinger());
         item->setPath(item_current->song()->getPath());
         item->setCategory(item_current->song()->getCategory());
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

void mainWindow::setAudioChannelAuto()
{
    QString audio = video->song()->getAudioChannel();
    QTimer::singleShot(500,[this,audio](){
        if (audio.compare("LEFT",Qt::CaseInsensitive)==0){
            video->player()->setAudioChannelLeft();
        }
        else if (audio.compare("RIGHT",Qt::CaseInsensitive)==0){
            video->player()->setAudioChannelRight();
        }
    });
}

void mainWindow::readSettings()
{
    QSettings setting("elroke","elroke");
    setting.beginGroup("Preferences");
    c_font = setting.value("font").toString();
    if (c_font==NULL)
        c_font = "Roboto";
    background = setting.value("background").toString();
    if (background==NULL)
        background = ":/usr/share/elroke/background/butterfly.jpeg";
    font_size = setting.value("font_size").toInt();
    if (font_size==0)
        font_size=16;
    language = setting.value("language").toString();
    setting.endGroup();
}

void mainWindow::handleFavorite(){

    if(db->isFavorite(video->song()->getId()))
    {
        buttonFavorite->setIcon(QIcon(":/usr/share/elroke/icon/favorite.png"));
    } else {
        buttonFavorite->setIcon(QIcon(":/usr/share/elroke/icon/unfavorite.png"));
    }

}

mainWindow::~mainWindow()
{

    delete video;
    if (autosave_playlist->isChecked())
        writePlaylist();
}
