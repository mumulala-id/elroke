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
#include "addtodatabase.h"
#include <dbmanager.h>
#include <QStorageInfo>
#include <QLabel>
#include <QHeaderView>
#include <QGridLayout>
#include <QGroupBox>
#include <QDebug>
#include <QTime>
#include <QPainter>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariantList>
#include <QtAlgorithms>//qSort
#include <QTabBar>
#include <QStackedLayout>
#include <QHeaderView>
#include <QTimer>
#include <QDragMoveEvent>

 class myListWidget :  public QListWidget
{
public :
     myListWidget(QWidget *parent) :
         QListWidget(parent){}

protected:
    void dragMoveEvent(QDragMoveEvent *e){
            if(e->source() != this){
                e->accept();
            } else {
                e->ignore();
            }
        }
 };


addtodatabase::addtodatabase(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *layout_main = new QVBoxLayout;
    QTabBar *tabbar = new QTabBar(this);
    tabbar->addTab(tr("Local"));
    tabbar->addTab(tr("Network"));

    QHBoxLayout *layout_top = new QHBoxLayout;
    QVBoxLayout *layout_top_left = new QVBoxLayout;

    combo_drive = new QComboBox(this);
   //get mounted drive
    getDrive();

    auto *button_refresh = new QPushButton(QIcon::fromTheme("stock_refresh"),"", this);
    button_refresh->setFixedWidth(40);
    connect(button_refresh,&QPushButton::pressed,this,&addtodatabase::getDrive);

    QHBoxLayout *layout_drive = new QHBoxLayout;
    layout_drive->addWidget(combo_drive);
    layout_drive->addWidget(button_refresh);
    layout_drive->addStretch();

     dir_model = new QFileSystemModel(this);
     dir_model->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
     currentDrive = combo_drive->itemText(0);
     dir_model->setRootPath(getCurrentDrive());

     treeview = new QTreeView(this);
     treeview->setModel(dir_model);
     treeview->setRootIndex(dir_model->index(getCurrentDrive()));
     treeview->hideColumn(1);
     treeview->hideColumn(2);
     treeview->hideColumn(3);
     treeview->header()->hide();

     connect(treeview,&QTreeView::clicked,[this](const QModelIndex &index)
     {
         current_dir = dir_model->fileInfo(index).absoluteFilePath();
         label_current_dir->setText(current_dir);
         getItem();
     });

     connect(combo_drive, static_cast<void (QComboBox::*)(const QString &)> (&QComboBox::activated),[this](const QString &drive)
     {
        treeview->setRootIndex(dir_model->index(drive));
     });

     file_model = new QFileSystemModel(this);
     file_model->setFilter(QDir::Files | QDir::NoDotAndDotDot);
     file_model->setNameFilters(supported_video);
     file_model->setNameFilterDisables(false);
     file_model->setRootPath(getCurrentDrive());

     label_current_dir = new QLabel(this);

     model = new QStandardItemModel;
     model->setColumnCount(2);

     view = new QTableView(this);
     view->setEditTriggers(QTableView::NoEditTriggers);
     view->setSelectionBehavior(QAbstractItemView::SelectRows);
     view->setSelectionMode(QAbstractItemView::ExtendedSelection);
     view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
     view->setModel(model);
     view->setColumnHidden(1,1);
     connect(view->selectionModel(),&QItemSelectionModel::selectionChanged,[this]()
     {
        QModelIndexList list =view->selectionModel()->selectedRows();
        if(list.count()>1)
            button_start->setEnabled(1);
     });

     layout_top_left->addLayout(layout_drive);
     layout_top_left->addWidget(treeview);
     layout_top ->addLayout(layout_top_left);

     auto *check_subfolder = new QCheckBox("include sub folder",this);
     connect(check_subfolder,&QCheckBox::toggled,[this](bool checked){
        if(checked)
        {
            subdir_flag = QDirIterator::Subdirectories;
            getItem();
        }
        else
        {
            subdir_flag = QDirIterator::NoIteratorFlags;
            getItem();
        }
     });

     auto *button_select_all = new QPushButton(tr("Select All"), this);

     connect(button_select_all,&QPushButton::pressed,view,&QTableView::selectAll);

     QHBoxLayout *layout_below_view = new QHBoxLayout;
     layout_below_view->addWidget(check_subfolder);
     layout_below_view->addStretch();
     layout_below_view->addWidget(button_select_all);
     //right
     QVBoxLayout *layout_list = new QVBoxLayout;
     layout_list->addWidget(label_current_dir);
     layout_list->addWidget(view);
     layout_list->addLayout(layout_below_view);

     layout_top->addLayout(layout_list);

    // ----

    QVBoxLayout *layout_splitter = new QVBoxLayout;
    auto *cb_auto = new QCheckBox(tr("Auto"), this);
    cb_auto->setChecked(automatic);

    QHBoxLayout *layout_use_char = new QHBoxLayout;
    auto *cb_splitby= new QCheckBox(tr("Split by"), this);



    le_splitter = new QLineEdit(this);
    le_splitter->setFixedWidth(30);
    le_splitter->setText(splitter);
    layout_use_char->addWidget(cb_splitby);
    layout_use_char->addWidget(le_splitter);
    layout_use_char->addStretch();
    layout_splitter->addWidget(cb_auto);
    layout_splitter->addLayout(layout_use_char,0);
    layout_splitter->addStretch();

    auto *grup_splitter = new QGroupBox(tr("Splitter"), this);
    grup_splitter->setLayout(layout_splitter);

////////////////////////////////////////// pattern/////////////////////////////////////////////////
    QVBoxLayout *layout_pattern = new QVBoxLayout;

    auto *view_choose_pattern = new myListWidget(this);
    view_choose_pattern->setFlow(QListView::LeftToRight);
    view_choose_pattern->setDragEnabled(true);
    view_choose_pattern->setDragDropMode(QAbstractItemView::DragDrop);
    view_choose_pattern->viewport()->setAcceptDrops(true);
    view_choose_pattern->setDropIndicatorShown(true);
    view_choose_pattern->setFixedHeight(view_choose_pattern->fontMetrics().height()+4);
    view_choose_pattern->setDefaultDropAction(Qt::MoveAction);

     QStringList patternList{"Title", "Singer", "Language"," Genre","Audio"};
     foreach (const QString &val, patternList) {
         auto *item = new QListWidgetItem;
         item->setText(val);
         view_choose_pattern->addItem(item);
     }

    auto *view_pattern = new myListWidget(this);
    view_pattern->setFlow(QListView::LeftToRight);
    view_pattern->setDragEnabled(true);
    view_pattern->setDragDropMode(QAbstractItemView::DragDrop);
    view_pattern->viewport()->setAcceptDrops(true);
    view_pattern->setDropIndicatorShown(true);
    view_pattern->setDefaultDropAction(Qt::MoveAction);
    view_pattern->setFixedHeight(view_pattern->fontMetrics().height()+4);

    layout_pattern->addWidget(new QLabel(tr("Select Pattern (Drag and Drop)"), this));
    layout_pattern->addWidget(view_choose_pattern);
    layout_pattern->addWidget(new QLabel(tr("Pattern"), this));
    layout_pattern->addWidget(view_pattern);
    layout_pattern->addStretch();

    auto *grup_pattern = new QGroupBox(tr("Pattern"), this);
    grup_pattern->setLayout(layout_pattern);

   QGridLayout *layout_additional_item = new QGridLayout;

   auto *le_singer = new QLineEdit(this);
   auto *le_language = new QLineEdit(this);
   auto *le_genre = new QLineEdit(this);

   layout_additional_item->addWidget( new QLabel(tr("Singer")),0,0);
   layout_additional_item->addWidget(le_singer,0,1);
   layout_additional_item->addWidget(new QLabel(tr("Language"), this), 1,0);
   layout_additional_item->addWidget(le_language,1,1);
   layout_additional_item->addWidget(new QLabel(tr("Genre"), this),2,0);
   layout_additional_item->addWidget(le_genre,2,1);
   layout_additional_item->setVerticalSpacing(0);

   auto *group_add_genre = new QGroupBox(tr("Add metadata if not available"));
   group_add_genre->setLayout(layout_additional_item);

   QVBoxLayout *layout_audio_channel = new QVBoxLayout;

   auto *combo_channel_left = new QCheckBox(tr("Left"), this);
   auto *combo_channel_right = new QCheckBox(tr("Right"), this);

   layout_audio_channel->addWidget(combo_channel_left);
   layout_audio_channel->addWidget(combo_channel_right);
   layout_audio_channel->addStretch();

   auto *group_audio_channel = new QGroupBox(tr("Audio Channel"), this);
   group_audio_channel->setLayout(layout_audio_channel);

   connect(cb_auto,&QCheckBox::toggled,[this,cb_splitby,grup_pattern,group_add_genre,group_audio_channel](bool a)
   {
       automatic = a;
       cb_splitby->setChecked(!a);
       grup_pattern->setEnabled(!a);
       group_add_genre->setEnabled(!a);
       group_audio_channel->setEnabled(!a);
   });
   connect(cb_splitby,&QCheckBox::toggled,[this, cb_auto,grup_pattern,group_add_genre,group_audio_channel](bool c)
   {
       automatic = !c;
      cb_auto->setChecked(!c);
      grup_pattern->setEnabled(c);
      group_add_genre->setEnabled(c);
      group_audio_channel->setEnabled(c);
   });

   QHBoxLayout *layout_bottom = new QHBoxLayout;
   layout_bottom->addWidget(grup_splitter);
   layout_bottom->addWidget(grup_pattern);
   layout_bottom->addWidget(group_add_genre);
   layout_bottom->addWidget(group_audio_channel);

   QWidget *widget_top = new QWidget(this);
   widget_top->setLayout(layout_top);

   QSizePolicy spTop(QSizePolicy::Preferred, QSizePolicy::Preferred);
   spTop.setVerticalStretch(2);
   widget_top->setSizePolicy(spTop);

   QWidget *widget_bottom = new QWidget(this);
   widget_bottom->setLayout(layout_bottom);

   QSizePolicy spBottom(QSizePolicy::Preferred, QSizePolicy::Preferred);
   spBottom.setVerticalStretch(1);
   widget_bottom->setSizePolicy(spBottom);

   QWidget *local_widget = new QWidget(this);
   QVBoxLayout *layout_local = new QVBoxLayout;
   layout_local->addWidget(widget_top);
   layout_local->addWidget(widget_bottom);
   local_widget->setLayout(layout_local);

//########################network widget############################
   ydownloader = new YoutubeDownloader(this);

   QStackedLayout *stack = new QStackedLayout;
   stack->addWidget(local_widget);
   stack->addWidget(ydownloader);
    connect(tabbar,&QTabBar::currentChanged,stack,&QStackedLayout::setCurrentIndex);
    connect(ydownloader,&YoutubeDownloader::finished,
           [this,stack](){
                                   stack->setCurrentIndex(0);
                                   current_dir = ydownloader->getPath();
                                   label_current_dir->setText(current_dir);
                                   getItem();

             });

   layout_main->addWidget(tabbar);
   layout_main->addLayout(stack);

   QHBoxLayout *layout_button = new QHBoxLayout;

   auto *button_cancel = new QPushButton(tr("Cancel"), this);
   connect(button_cancel,&QPushButton::pressed,this,&addtodatabase::close);

  button_start = new QPushButton(tr("Start"), this);
  button_start->setEnabled(0);
  connect(button_start,&QPushButton::pressed,this,&addtodatabase::saveToDatabase);

   layout_button->addStretch();
   layout_button->addWidget(button_cancel);
   layout_button->addWidget(button_start);
   layout_main->addLayout(layout_button);



    QPalette palet;
    palet.setColor(QPalette::Base, palette().dark().color());
    palet.setColor(QPalette::Window, Qt::black);
    palet.setColor(QPalette::Text, Qt::white);
    palet.setColor(QPalette::WindowText, Qt::white);
    palet.setColor(QPalette::Button, palette().dark().color());
    setPalette(palet);
      setLayout(layout_main);

//    setAutoFillBackground(1);
//    setWindowFlags(Qt::FramelessWindowHint );
    setWindowState(Qt::WindowFullScreen);
}

void addtodatabase::getItem()
{
    setCursor(Qt::BusyCursor);
    model->clear();

    int row = 0;
    QDirIterator it(current_dir,supported_video, QDir::Files,getSubDirFlag());
      while (it.hasNext())
      {
          model->setRowCount(model->rowCount()+1);
           QString filename = it.next();
           QFileInfo info;
           info.setFile(filename);

           QString path = info.absoluteFilePath();
           QString name = info.fileName();

           QStandardItem *item_name = new QStandardItem;
           item_name->setText(name);
           model->setItem(row,0,item_name);

           QStandardItem *item_path = new QStandardItem;
           item_path->setText(path);
           model->setItem(row,1,item_path);

           row++;
      }

    view->setColumnHidden(1,1);
    setCursor(Qt::ArrowCursor);
}

void addtodatabase::saveToDatabase()
{
    setCursor(Qt::BusyCursor);
    button_start->setEnabled(0);
    splitter = le_splitter->text();

    QModelIndexList list =  view->selectionModel()->selectedRows();

    dbmanager *db = new dbmanager(dbmanager::add, this);
    db->connectToDB();
    db->prepare();

    bool sql_ok=true;

   QVariantList data;
   QSet<QString>set_singer;
   QSet<QString>set_language;
   QSet<QString>set_genre;
   QSet<QString>set_folder;

    QStringList splitted;
    QString title, singer, language, genre, a_channel, path,folder;
    QString filename;

//    auto fillDefault = [](QString x){
//        if(x.isEmpty())
//                return "unknown";
//        return x;
////                qDebug()<<x;

////        } else {
//////              qDebug()<<x;
////        }
//    };

    if(!automatic)//manual
    {
        foreach (QModelIndex index, list)
        {
            filename = model->data(model->index(index.row(), 0)).toString();
            path = model->data(model->index(index.row(),1)).toString();
            QFileInfo info;
            info.setFile(path);

           if(filename.contains(splitter))
           {
               splitted = filename.split(splitter);

               singer.clear();
               title.clear();
               language.clear();
               genre.clear();
               a_channel.clear();

               switch(splitted.count())
               {
                           case 2:
                               title = splitted.at(0);
                               singer = splitted.at(1);
                               break;
                           case 3:
                               title = splitted.at(0);
                               singer = splitted.at(1);
                               a_channel =splitted.last();
                               break;
                           case 4:
                               title = splitted.at(0);
                               singer = splitted.at(1);
                               language = splitted.at(2);
                               a_channel =splitted.last();
                               break;
                           case 5 :
                               title = splitted.at(0);
                               singer = splitted.at(1);
                               language = splitted.at(2);
                               genre = splitted.at(3);
                               a_channel =splitted.last();
                               break;
                           default:
                               break;
               }

           }

        set_singer.insert(singer.toUpper());
        set_language.insert(language.toUpper());
        set_genre.insert(genre.toUpper());
        set_folder.insert(folder);

        data.append(title);
        data.append(singer);
        data.append(language);
        data.append(genre);
        data.append(a_channel);
        data.append(path);

        sql_ok =  db->insertIntoTable(data);
        data.clear();

        }

    }
    else//automatic
    {
        foreach (QModelIndex index, list)
        {
            path = model->data(model->index(index.row(),1)).toString();
            QFileInfo info;
            info.setFile(path);
            folder = info.absolutePath();
            filename = info.completeBaseName();
            splitter =  getSplitter(filename);

            splitted = filename.split(splitter);
            singer.clear();
            title.clear();
            language.clear();
            genre.clear();
            a_channel.clear();

            if(splitter.isEmpty())
                title = filename;

               switch(splitted.count())
               {
                   case 2:
                       title = splitted.at(0);
                       singer = splitted.at(1);
                       break;
                   case 3:
                       title = splitted.at(0);
                       singer = splitted.at(1);
                       a_channel =splitted.last();
                       break;
                   case 4:
                       title = splitted.at(0);
                       singer = splitted.at(1);
                       language = splitted.at(2);
                       a_channel =splitted.last();
                       break;
                   case 5 :
                       title = splitted.at(0);
                       singer = splitted.at(1);
                       language = splitted.at(2);
                       genre = splitted.at(3);
                       a_channel =splitted.last();
                       break;
                   default:
                       title = splitted.at(0);
                       singer = splitted.at(1);
                       language = splitted.at(2);
                       genre = splitted.at(3);
                       a_channel =splitted.last();
                       break;
           }

             if(singer.isEmpty())
                 singer="UNKNOWN";
             if(title.isEmpty())
                 title="UNKNOWN";
             if(language.isEmpty())
                 language="UNKNOWN";
             if(genre.isEmpty())
                 genre="UNKNOWN";
             if(a_channel.isEmpty())
                 a_channel="UNKNOWN";

               set_singer.insert(singer.trimmed().toUpper());
               set_language.insert(language.trimmed().toUpper());
               set_genre.insert(genre.trimmed().toUpper());
               set_folder.insert(folder);
               a_channel = a_channel.trimmed().toUpper();

            data.append(title.toUpper());
            data.append(singer.toUpper());
            data.append(language.toUpper());
            data.append(genre.toUpper());
            data.append(a_channel.toUpper());
            data.append(path);

            sql_ok =  db->insertIntoTable(data);
            data.clear();

        }
    }

    if(sql_ok)   {
        db->submit();
    }    else {
         qDebug()<<"sql not ok";
    }


    set_singer.remove("");
    set_singer.remove(" ");
    set_genre.remove("");
    set_genre.remove(" ");
    set_language.remove("");
    set_language.remove(" ");


    writeTextStream(data_dir+"/meta/singer", set_singer);
    writeTextStream(data_dir+"/meta/language", set_language);
    writeTextStream(data_dir+"/meta/genre", set_genre);
    writeTextStream(data_dir+"/meta/path", set_folder);

     setCursor(Qt::ArrowCursor);

      accept();

 }

void addtodatabase::writeTextStream(const QString &file, QSet<QString>set)
{
    QFileInfo info;
    info.setFile(file);

    if(!info.dir().exists())
        QDir().mkdir(info.path());

    QFile f(file);
        
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
         qDebug()<<"cannot write stream";
    }
    else
    {
          QTextStream stream(&f);
        
          foreach (const QString &val, set)
          {
                 stream << val<<'\n';
          }
    }
         f.close();         
}

void addtodatabase::getDrive()
{
    combo_drive->clear();
    combo_drive->addItem(QDir::homePath() );
    //get mounted drive just support ntfs
    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes())
    {
        if (storage.fileSystemType()=="ntfs-3g" || storage.fileSystemType()=="fuseblk")
        {
            combo_drive->addItem(storage.rootPath());
        }
    }
}

QString addtodatabase::getSplitter(const QString &filename)
{
//identify splitter
    QRegularExpression exp("[^a-zA-Z0-9\\s]+");
    QRegularExpressionMatchIterator i = exp.globalMatch(filename);
    QStringList symbol;
    while(i.hasNext())
    {
        QRegularExpressionMatch match=i.next();
        symbol<<match.captured();
    }

    QSet<QString>s;
    s=s.fromList(symbol);

    int x = 0;
    QString d;
//get largest count item
    foreach (QString val, s)
    {
        if(x<symbol.count(val))
        {
            x = symbol.count(val);
            d=val;
        }
    }
qDebug()<<d<<filename;
    return d;
}

