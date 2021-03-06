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
#include <liststringfileparser.h>
#include <dbmanager.h>
#include <QStorageInfo>
#include <QGridLayout>
#include <QGroupBox>
#include <QDebug>
#include <QTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariantList>
#include <QtAlgorithms>//qSort
#include <QTabBar>
#include <QStackedLayout>
#include <QHeaderView>
#include <QMessageBox>

addtodatabase::addtodatabase(QWidget *parent) :
    QDialog(parent)
{
    auto layout_main = new QVBoxLayout;
    auto tabbar = new QTabBar(this);
    tabbar->addTab(tr("Local"));
    tabbar->addTab(tr("Network"));

    auto layout_top = new QHBoxLayout;
    auto layout_top_left = new QVBoxLayout;

    combo_drive = new QComboBox(this);
   //get mounted drive
    getDrive();


    auto button_refresh = new QPushButton(QIcon::fromTheme("stock_refresh"),"", this);
    button_refresh->setFixedWidth(40);
    connect(button_refresh,&QPushButton::pressed,this,&addtodatabase::getDrive);

    auto layout_drive = new QHBoxLayout;
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
         currentDirectory = dir_model->fileInfo(index).absoluteFilePath();
         label_currentDirectory->setText(currentDirectory);
         getItem();
     });

     connect(combo_drive, static_cast<void (QComboBox::*)(const QString &)> (&QComboBox::activated),[this](const QString &drive)
     {
        treeview->setRootIndex(dir_model->index(drive));
        currentDrive = drive;
        getItem();
     });

     file_model = new QFileSystemModel(this);
     file_model->setFilter(QDir::Files | QDir::NoDotAndDotDot);
     file_model->setNameFilters(supported_video);
     file_model->setNameFilterDisables(false);
     file_model->setRootPath(getCurrentDrive());

     label_currentDirectory = new QLabel(this);

     model = new QStandardItemModel;
     model->setColumnCount(2);
     model->setHorizontalHeaderLabels(QStringList()<<"NAME"<<"PATH");

     view = new QTableView(this);
     view->setEditTriggers(QTableView::NoEditTriggers);
     view->setSelectionBehavior(QAbstractItemView::SelectRows);
     view->setSelectionMode(QAbstractItemView::ExtendedSelection);
     view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
     view->setModel(model);
     view->setColumnHidden(1,1);//hide path
//     view->seth
     connect(view->selectionModel(),&QItemSelectionModel::selectionChanged,[this]()
     {
        QModelIndexList list =view->selectionModel()->selectedRows();
        if(list.count()>0)
            button_start->setEnabled(true);
     });

     currentDirectory = combo_drive->currentText();
     getItem();

     layout_top_left->addLayout(layout_drive);
     layout_top_left->addWidget(treeview);
     layout_top ->addLayout(layout_top_left);

     auto check_subfolder = new QCheckBox("include sub folder",this);
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

     auto button_select_all = new QPushButton(tr("Select All"), this);

     connect(button_select_all,&QPushButton::pressed,view,&QTableView::selectAll);

     auto layout_below_view = new QHBoxLayout;
     layout_below_view->addWidget(check_subfolder);
     layout_below_view->addStretch();
     layout_below_view->addWidget(button_select_all);

     auto layout_list = new QVBoxLayout;
     layout_list->addWidget(label_currentDirectory);
     layout_list->addWidget(view);
     layout_list->addLayout(layout_below_view);

     layout_top->addLayout(layout_list);
///////////////////////////////////////////// bottom ///////////////////////////////////////////////
     auto layout_splitter = new QVBoxLayout;
    auto radio_auto = new QRadioButton(tr("Auto"), this);
    radio_auto->setChecked(automatic);

    auto layout_use_char = new QHBoxLayout;
    auto radio_split_by= new QRadioButton(tr("Split by"), this);

    le_splitter = new QLineEdit(this);
    le_splitter->setFixedWidth(30);
    le_splitter->setText(splitter);
    connect(le_splitter,&QLineEdit::textChanged,[this](const QString &text)
    {
        splitter = text;
    });

    layout_use_char->addWidget(radio_split_by);
    layout_use_char->addWidget(le_splitter);
    layout_use_char->addStretch();
    layout_splitter->addWidget(radio_auto);
    layout_splitter->addLayout(layout_use_char,0);
    layout_splitter->addStretch();

    auto grup_splitter = new QGroupBox(tr("Splitter"), this);
    grup_splitter->setLayout(layout_splitter);

////////////////////////////////////////// pattern/////////////////////////////////////////////////
    auto layout_pattern = new QVBoxLayout;

    auto view_choose_pattern = new myListWidget(this);
    view_choose_pattern->setObjectName("sender");
    view_choose_pattern->setFlow(QListView::LeftToRight);
    view_choose_pattern->setDragEnabled(true);
    view_choose_pattern->setDragDropMode(QAbstractItemView::DragDrop);
    view_choose_pattern->viewport()->setAcceptDrops(true);
    view_choose_pattern->setDropIndicatorShown(true);
    view_choose_pattern->setFixedHeight(view_choose_pattern->fontMetrics().height()+4);
    view_choose_pattern->setDefaultDropAction(Qt::MoveAction);

     QStringList patternList{"Title", "Singer", "Language", "Genre","Audio"};
     foreach (const QString &val, patternList) {
         auto item = new QListWidgetItem;
         item->setText(val);
         view_choose_pattern->addItem(item);
     }

    view_pattern = new myListWidget(this);
    view_pattern->setObjectName("receiver");
    view_pattern->setFlow(QListView::LeftToRight);
    view_pattern->setDragEnabled(true);
    view_pattern->setDragDropMode(QAbstractItemView::DragDrop);
    view_pattern->viewport()->setAcceptDrops(true);
    view_pattern->setDropIndicatorShown(true);
    view_pattern->setDefaultDropAction(Qt::MoveAction);
    view_pattern->setFixedHeight(view_pattern->fontMetrics().height()+4);
    connect(view_choose_pattern,&myListWidget::dropped,this,&addtodatabase::pattern);
    connect(view_pattern,&myListWidget::dropped,this,&addtodatabase::pattern);

    label_pattern = new QLabel(this);
    QPalette p = label_pattern->palette();
    p.setColor(QPalette::WindowText,Qt::red);
    label_pattern->setPalette(p);

    connect(le_splitter,&QLineEdit::textChanged,[this](QString text){
        if(text.isEmpty())
            return;
        splitter=text;
        QStringList p;
        label_pattern->setText("");
        for(int i=0; i < view_pattern->count();i++)
        {
            auto item = view_pattern->item(i);
           p<<item->data(Qt::DisplayRole).toString();
        }
        label_pattern->setText(p.join(splitter));
    });
    layout_pattern->addWidget(new QLabel(tr("Select Pattern (Drag)"), this));
    layout_pattern->addWidget(view_choose_pattern);
    layout_pattern->addWidget(new QLabel("Drop here\t\u25BC\u25B2", this));
    layout_pattern->addWidget(view_pattern);
    layout_pattern->addWidget(new QLabel(tr("Pattern :"),this));
    layout_pattern->addWidget(label_pattern);
    layout_pattern->addStretch();

    auto group_pattern = new QGroupBox(tr("Pattern"), this);
    group_pattern->setLayout(layout_pattern);
    group_pattern->setEnabled(!automatic);

   auto layout_additional_item = new QVBoxLayout;

   le_singer = new QLineEdit(this);
   le_language = new QLineEdit(this);
   le_genre = new QLineEdit(this);

   auto radio_audio_left = new QRadioButton(tr("Left"), this);
   radio_audio_left->setChecked(true);
   auto radio_audio_right = new QRadioButton(tr("Right"), this);
   auto radio_audio_stereo = new QRadioButton(tr("Stereo"), this);

   auto item = [this](QWidget *a,QWidget *b){

       a->setFixedWidth(140);
       auto l = new QHBoxLayout;
      l->addWidget(a);
       l->addWidget(b);

       auto widget = new QWidget(this);
       widget->setLayout(l);
       return widget;

   };

    layout_additional_item->addWidget(item(new QLabel(tr("Singer"),this), le_singer));
    layout_additional_item->addWidget(item(new QLabel(tr("Language"), this), le_language));
    layout_additional_item->addWidget(item(new QLabel(tr("Genre"), this),le_genre));

    auto layout_audio = new QHBoxLayout;
    layout_audio->addWidget(new QLabel("Audio",this));
    layout_audio->addWidget(radio_audio_left);
    layout_audio->addWidget(radio_audio_right);
    layout_audio->addWidget(radio_audio_stereo);
    layout_additional_item->addLayout(layout_audio);

   auto group_metadata = new QGroupBox(tr("Add metadata if not available"));
   group_metadata->setLayout(layout_additional_item);
   group_metadata->setEnabled(!automatic);

   connect(radio_audio_left,&QCheckBox::toggled,[this]()
   {
        default_audio = "LEFT";
   });

   connect(radio_audio_right,&QCheckBox::toggled,[this]()
   {

       default_audio = "RIGHT";

   });

   connect(radio_audio_stereo,&QCheckBox::toggled,[this]()
   {
       default_audio="STEREO";

   });

   connect(radio_auto,&QCheckBox::toggled,[this,radio_split_by,group_pattern,group_metadata](bool a)
   {
       automatic = a;
       group_pattern->setEnabled(!a);
       group_metadata->setEnabled(!a);
   });
   connect(radio_split_by,&QCheckBox::toggled,[this, radio_auto,group_pattern,group_metadata](bool c)
   {
       automatic = !c;
      group_pattern->setEnabled(c);
      group_metadata->setEnabled(c);
   });

   auto layout_bottom = new QHBoxLayout;
//   layout_bottom->addStretch();
   layout_bottom->addWidget(grup_splitter);
   layout_bottom->addWidget(group_pattern);
   layout_bottom->addWidget(group_metadata);
//   layout_bottom->addStretch();

   auto widget_top = new QWidget(this);
   widget_top->setLayout(layout_top);

   QSizePolicy spTop(QSizePolicy::Preferred, QSizePolicy::Preferred);
   spTop.setVerticalStretch(2);
   widget_top->setSizePolicy(spTop);

   auto widget_bottom = new QWidget(this);
   widget_bottom->setLayout(layout_bottom);

   QSizePolicy spBottom(QSizePolicy::Preferred, QSizePolicy::Preferred);
   spBottom.setVerticalStretch(1);
   widget_bottom->setSizePolicy(spBottom);

   auto layout_button = new QHBoxLayout;
   auto button_cancel = new QPushButton(tr("Cancel"), this);
   connect(button_cancel,&QPushButton::pressed,this,&addtodatabase::close);

  button_start = new QPushButton(tr("Start"), this);
  button_start->setEnabled(0);
  connect(button_start,&QPushButton::pressed,this,&addtodatabase::saveToDatabase);

   auto local_widget = new QWidget(this);
   auto layout_local = new QVBoxLayout;
   layout_local->addWidget(widget_top);
   layout_local->addWidget(widget_bottom);
   layout_local->addLayout(layout_button);
   local_widget->setLayout(layout_local);

//########################network widget############################
   auto ydownloader = new YoutubeDownloader(this);

   auto stack = new QStackedLayout;
   stack->addWidget(local_widget);
   stack->addWidget(ydownloader);
    connect(tabbar,&QTabBar::currentChanged,stack,&QStackedLayout::setCurrentIndex);
    connect(ydownloader,&YoutubeDownloader::finished,
           [this,stack,ydownloader](){
                                   stack->setCurrentIndex(0);
                                   currentDirectory = ydownloader->getPath();
                                   label_currentDirectory->setText(currentDirectory);
                                   getItem();

             });

   layout_main->addWidget(tabbar);
   layout_main->addLayout(stack);

   layout_button->addStretch();
   layout_button->addWidget(button_cancel);
   layout_button->addWidget(button_start);

    QPalette palet;
    palet.setColor(QPalette::Base, theme.backgroundColor());
    palet.setColor(QPalette::Window, theme.backgroundColor());
    palet.setColor(QPalette::Text,theme.textColor());
    palet.setColor(QPalette::WindowText, theme.textColor());
    palet.setColor(QPalette::Button, Qt::darkGray);
    palet.setColor(QPalette::ButtonText, Qt::white);
    setPalette(palet);
    setLayout(layout_main);
}

void addtodatabase::getItem()
{
    setCursor(Qt::BusyCursor);
    model->clear();

    int row = 0;
    QDirIterator it(currentDirectory,supported_video, QDir::Files,getSubDirFlag());
      while (it.hasNext())
      {
          model->setRowCount(model->rowCount()+1);
           QString filename = it.next();
           QFileInfo info;
           info.setFile(filename);

           QString path = info.absoluteFilePath();
           QString name = info.fileName();

           auto item_name = new QStandardItem;
           item_name->setText(name);
           model->setItem(row,0,item_name);

           auto item_path = new QStandardItem;
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

    auto db = new dbmanager(dbmanager::add, this);
    db->connectToDB();
    db->prepare();

    bool sql_ok=true;

   QVariantList data;
   QSet<QString>set_singer;
   QSet<QString>set_language;
   QSet<QString>set_genre;
   QSet<QString>set_folder;

   QString default_singer, default_language, default_genre, default_audio;

    QStringList splitted;
    QString title, singer, language, genre, a_channel, path,folder;
    QString filename;
    QFileInfo info;

    if(!automatic)//manual
    {
        QString _pattern = label_pattern->text();
        if(_pattern.isEmpty())
        {
            setCursor(Qt::ArrowCursor);
            QMessageBox message;
            message.setInformativeText(tr("Pattern not set yet"));
            message.exec();
            return;
        }


            if(!le_singer->text().isEmpty())
                 default_singer = le_singer->text();
            else
                default_singer="UNKNOWN";

            if(!le_language->text().isEmpty())
                 default_language = le_language->text();
            else
               default_language="UKNOWN";

            if(!le_genre->text().isEmpty())
                  default_genre = le_genre->text();
            else
               default_genre = "UNKNOWN";

        QStringList p = _pattern.split(splitter);

        uint short titlePos = p.indexOf("Title");
        uint short singerPos = p.indexOf("Singer");
        uint short languagePos = p.indexOf("Language");
        uint short genrePos = p.indexOf("Genre");
        uint short audioPos = p.indexOf("Audio");


        foreach (QModelIndex index, list)
        {
            path = model->data(model->index(index.row(),1)).toString();
            info.setFile(path);
            filename = info.completeBaseName();
            folder = info.absolutePath();

            singer = default_singer;
            language = default_language;
            genre = default_genre;

           if(filename.contains(splitter))
           {
               splitted = filename.split(splitter);

               if(titlePos!=-1 && titlePos<splitted.count())

                  title = splitted.at(titlePos);

               if(singerPos!=-1 && singerPos<splitted.count())
                       singer = splitted.at(singerPos);

               if(languagePos!=-1 && languagePos<splitted.count())
//                   if(language=="UNKNOWN")
                    language = splitted.at(languagePos);

               if(genrePos!=-1&&genrePos<splitted.count())
//                   if(genre=="UNKNOWN")
                genre = splitted.at(genrePos);

               if(audioPos!=-1&&audioPos<splitted.count())
               a_channel = splitted.at(audioPos);

           } else {

               //generally filename pattern title - singer
               if(filename.contains('-')){

               title = filename.split('-').at(0);
               singer = filename.split('-').at(1);
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

            data.append(title.trimmed().toUpper());
            data.append(singer.trimmed().toUpper());
            data.append(language.trimmed().toUpper());
            data.append(genre.trimmed().toUpper());
            data.append(a_channel.trimmed().toUpper());
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

    QStringList exist = listStringFileParser::parse(file);
    set = set+exist.toSet();

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
    s = s.fromList(symbol);

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
    return d;
}

void addtodatabase::pattern(){
   QString obj = sender()->objectName();
   //   QStringList p;
   QStringList listPattern;

    for(int i=0; i < view_pattern->count();i++)
    {
        auto item = view_pattern->item(i);
        listPattern<<item->data(Qt::DisplayRole).toString();
    }

    if(obj == "sender"){
        myListWidget *sdr = qobject_cast<myListWidget*>(sender());
        QStringList ori ;
        for(int i=0; i < sdr->count();i++)
        {
            auto item = sdr->item(i);
             ori<<item->data(Qt::DisplayRole).toString();
        }

        foreach (QString s, ori ) {
                    listPattern.removeAll(s);
        }
    }
   label_pattern->setText(listPattern.join(splitter));
}
