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

addtodatabase::addtodatabase(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *layout_main = new QVBoxLayout;

    QTabBar *tabbar = new QTabBar(this);
    tabbar->addTab(tr("Local"));
    tabbar->addTab(tr("Network"));

    QHBoxLayout *layout_top = new QHBoxLayout;
    QVBoxLayout *layout_top_left = new QVBoxLayout;

    combo_mounted = new QComboBox(this);

   //get mounted drive
    getDrive();

    QPushButton *button_refresh = new QPushButton(QIcon::fromTheme("stock_refresh"),"", this);
    button_refresh->setFixedWidth(40);
    connect(button_refresh,SIGNAL(clicked(bool)),this,SLOT(getDrive()));

     dir_model = new QFileSystemModel(this);
     dir_model->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
     dir_ = combo_mounted->itemText(0);
     dir_model->setRootPath(pakDir());

     tv_folder = new QTreeView(this);
     tv_folder->setModel(dir_model);
     tv_folder->setRootIndex(dir_model->index(pakDir()));
     tv_folder->hideColumn(1);
     tv_folder->hideColumn(2);
     tv_folder->hideColumn(3);
     tv_folder->header()->hide();

     connect(tv_folder, SIGNAL(clicked(QModelIndex)), this,SLOT(onTreeviewClicked(QModelIndex)));
//     connect(tv_folder,SIGNAL())

     connect(combo_mounted, static_cast<void (QComboBox::*)(const QString &)> (&QComboBox::activated),[this](const QString &drive){
        tv_folder->setRootIndex(dir_model->index(drive));
     });

     file_model = new QFileSystemModel(this);
     file_model->setFilter(QDir::Files | QDir::NoDotAndDotDot);
     file_model->setNameFilters(QStringList()<<"*.mp4"<<"*.mkv"<<"*.mpg"<<"*.dat"<<"*.avi"<<"*.mov"<<"*.webm");
     file_model->setNameFilterDisables(false);

     file_model->setRootPath(pakDir());

     //list for show files
     lw_list = new QListWidget(this);
     lw_list->setSelectionMode(QAbstractItemView::ExtendedSelection);
     connect(lw_list,&QListWidget::itemSelectionChanged,[this](){
         QListWidgetItem *current= lw_list->currentItem();
     if(current){
             button_start->setEnabled(1);
     }
     else{
          button_start->setEnabled(1);
     }
     });

     QHBoxLayout *layout_drive = new QHBoxLayout;
     layout_drive->addWidget(combo_mounted);
     layout_drive->addWidget(button_refresh);
     layout_top_left->addLayout(layout_drive);
     layout_top_left->addWidget(tv_folder);
     layout_top ->addLayout(layout_top_left);


     QPushButton *button_select_all = new QPushButton(tr("Select All"), this);
     connect(button_select_all,SIGNAL(pressed()),lw_list,SLOT(selectAll()));
     //right
     QVBoxLayout *layout_list = new QVBoxLayout;
     layout_list->addWidget(lw_list);
     layout_list->addWidget(button_select_all);

     layout_top->addLayout(layout_list);

    // ----

    QVBoxLayout *layout_splitter = new QVBoxLayout;
    cb_auto = new QCheckBox(tr("Auto"), this);
    cb_auto->setChecked(automatic);
    connect(cb_auto,SIGNAL(toggled(bool)),this,SLOT(setToAuto(bool)));

    QHBoxLayout *layout_use_char = new QHBoxLayout;
    QCheckBox *cb_splitby= new QCheckBox(tr("Split by"), this);
    connect(cb_splitby,SIGNAL(toggled(bool)),this,SLOT(setToManual(bool)));

    le_splitter = new QLineEdit(this);
    le_splitter->setFixedWidth(30);
    le_splitter->setText(splitter);
    connect(le_splitter,SIGNAL(textChanged(QString)),this,SLOT(splitterChange(QString)));
    layout_use_char->addWidget(cb_splitby);
    layout_use_char->addWidget(le_splitter);
    layout_use_char->addStretch();
    layout_splitter->addWidget(cb_auto);
    layout_splitter->addLayout(layout_use_char);
    layout_splitter->addStretch();

    QGroupBox *grup_splitter = new QGroupBox(tr("Splitter"), this);
    grup_splitter->setLayout(layout_splitter);

    QVBoxLayout *layout_pattern = new QVBoxLayout;

    cmb_titlefirst = new QCheckBox( this);
    cmb_titlefirst->setText(tr("Title")+splitter+tr("Singer"));
    cmb_titlefirst->setChecked(title_first);
    connect(cmb_titlefirst,SIGNAL(clicked(bool)),this,SLOT(setTitleFirst(bool)));

    cmb_singerfirst = new QCheckBox(this);
    cmb_singerfirst->setText(tr("Singer")+splitter+tr("Title"));
    connect(cmb_singerfirst,SIGNAL(clicked(bool)),this,SLOT(setSingerFirst(bool)));

    layout_pattern->addWidget(cmb_titlefirst);
    layout_pattern->addWidget(cmb_singerfirst);
    layout_pattern->addStretch();

    QGroupBox *grup_pattern = new QGroupBox(tr("Pattern"), this);
    grup_pattern->setLayout(layout_pattern);

   QGridLayout *layout_additional_item = new QGridLayout;

   QLineEdit *le_singer = new QLineEdit(this);
   QLineEdit *le_language = new QLineEdit(this);
   QLineEdit *le_category = new QLineEdit(this);

   layout_additional_item->addWidget( new QLabel(tr("Singer")),0,0);
   layout_additional_item->addWidget(le_singer,0,1);
   layout_additional_item->addWidget(new QLabel(tr("Language"), this), 1,0);
   layout_additional_item->addWidget(le_language,1,1);
   layout_additional_item->addWidget(new QLabel(tr("Category"), this),2,0);
   layout_additional_item->addWidget(le_category,2,1);
   layout_additional_item->setVerticalSpacing(0);


   QGroupBox *gr_addcat = new QGroupBox(tr("Add metadata if not available"));
   gr_addcat->setLayout(layout_additional_item);

   QVBoxLayout *layout_audio_channel = new QVBoxLayout;

   QCheckBox *combo_channel_left = new QCheckBox("Left", this);
   QCheckBox *combo_channel_right = new QCheckBox("Right", this);

   layout_audio_channel->addWidget(combo_channel_left);
   layout_audio_channel->addWidget(combo_channel_right);
   layout_audio_channel->addStretch();

   QGroupBox *gr_audio_channel = new QGroupBox(tr("Audio Channel"), this);

   gr_audio_channel->setLayout(layout_audio_channel);

   QHBoxLayout *layout_bottom = new QHBoxLayout;
   layout_bottom->addWidget(grup_splitter);
   layout_bottom->addWidget(grup_pattern);
   layout_bottom->addWidget(gr_addcat);
   layout_bottom->addWidget(gr_audio_channel);

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

//network widget
   ydownloader = new YoutubeDownloader(this);


   QStackedLayout *stack = new QStackedLayout;
   stack->addWidget(local_widget);
   stack->addWidget(ydownloader);

   connect(tabbar,SIGNAL(currentChanged(int)),stack,SLOT(setCurrentIndex(int)));
   connect(ydownloader,&YoutubeDownloader::finished,[this,stack](){
       stack->setCurrentIndex(0);
       dir_model->setRootPath(ydownloader->getPath());
       tv_folder->setRootIndex(dir_model->index(ydownloader->getPath()));
       lw_list->clear();

//       working_path = dir_model->fileInfo(index).absoluteFilePath();

       QDirIterator it(ydownloader->getPath(),QStringList()<<"*.mp4"<<"*.avi"<<"*.dat"<<"*.mkv"<<"*.mpg"<<"*.mov", QDir::Files,QDirIterator::Subdirectories);

             while (it.hasNext()) {
                   it.next();
                   lw_list->addItem(it.fileName());

             }
   });

   layout_main->addWidget(tabbar);
   layout_main->addLayout(stack);
//   layout_main->addLayout(layout_btn);

    setLayout(layout_main);
    QPalette palet;
    palet.setColor(QPalette::Base, palette().dark().color());
    palet.setColor(QPalette::Window, Qt::black);
    palet.setColor(QPalette::Text, Qt::white);
    palet.setColor(QPalette::WindowText, Qt::white);
    palet.setColor(QPalette::Button, palette().dark().color());
    setPalette(palet);

    setAutoFillBackground(1);

    setWindowFlags(Qt::FramelessWindowHint );
    setWindowState(Qt::WindowFullScreen);
}

void addtodatabase::onTreeviewClicked(const QModelIndex &index){

    setCursor(Qt::BusyCursor);
    lw_list->clear();

    working_path = dir_model->fileInfo(index).absoluteFilePath();

    QDirIterator it(working_path,QStringList()<<"*.mp4"<<"*.avi"<<"*.dat"<<"*.mkv"<<"*.mpg"<<"*.mov", QDir::Files,QDirIterator::Subdirectories);

          while (it.hasNext()) {
                it.next();
                lw_list->addItem(it.fileName());

          }

    setCursor(Qt::ArrowCursor);

}

void addtodatabase::splitterChange(QString split){

    if(split==NULL)
        return;
    cmb_titlefirst->setText(tr("Title")+split+tr("Singer"));
    cmb_singerfirst->setText(tr("Singer")+split+tr("Title"));

}

void addtodatabase::saveToDatabase(){

    setCursor(Qt::BusyCursor);
    button_start->setEnabled(0);
    splitter = le_splitter->text();

    QDirIterator it(working_path,QStringList()<<"*.mp4"<<"*.avi"<<"*.dat"<<"*.mkv"<<"*.mpg"<<"*.mov", QDir::Files,QDirIterator::Subdirectories);

    QStringList filename;
     QString title, singer, language, category, a_channel, path,folder;
     QFileInfo info;
     QString namefile;

     dbmanager *db = new dbmanager(dbmanager::add, this);
     db->connectToDB();
     db->prepare();

     bool sql_ok=true;


    QVariantList data;
    QSet<QString>set_singer;
    QSet<QString>set_language;
    QSet<QString>set_category;
    QSet<QString>set_folder;


    while(it.hasNext()){
        info.setFile(it.next());
        namefile= info.completeBaseName();
        path = info.absoluteFilePath();
        folder= info.absolutePath();


        if(namefile.contains(splitter)){
             filename = namefile.split(splitter);
                switch(filename.count()){
                            case 2:
                                title = filename.at(0);
                                singer = filename.at(1);
                                break;
                            case 3:
                                title = filename.at(0);
                                singer = filename.at(1);
                                a_channel =filename.last();
                                break;
                            case 4:
                                title = filename.at(0);
                                singer = filename.at(1);
                                language = filename.at(2);
                                a_channel =filename.last();
                                break;
                            case 5 :
                                title = filename.at(0);
                                singer = filename.at(1);
                                language = filename.at(2);
                                category = filename.at(3);
                                a_channel =filename.last();
                                break;
                            default:
                                break;
                            }
        }
        else {//usually name is title - singer or singer - title
            if(namefile.contains("-")){
               filename = namefile.split("-");
               title = filename.at(0);
               singer = filename.at(1);

            }
        }


        set_singer.insert(singer.toUpper());
        set_language.insert(language.toUpper());
        set_category.insert(category.toUpper());
        set_folder.insert(folder);

          data.append(title);
          data.append(singer);
          data.append(language);
          data.append(category);
          data.append(a_channel);
          data.append(path);

        sql_ok =  db->insertIntoTable(data);
        data.clear();

    }
    if(sql_ok)
                         db->submit();
    else

                                         qDebug()<<"sql not ok";

    QList<QString>list_singer=set_singer.toList();
    QList<QString>list_language= set_language.toList();
    QList<QString>list_genre=set_category.toList();
    QList<QString>list_path = set_folder.toList();

    qSort(list_singer.begin(), list_singer.end());
    qSort(list_language.begin(), list_language.end());
    qSort(list_genre.begin(), list_genre.end());

    writeTextStream(data_dir+"/meta/singer", list_singer);
    writeTextStream(data_dir+"/meta/language", list_language);
    writeTextStream(data_dir+"meta/category", list_genre);
    writeTextStream(data_dir+"/meta/path", list_path);

     setCursor(Qt::ArrowCursor);

      this->accept();

 }

void addtodatabase::writeTextStream(const QString &file, QList<QString>set){
    QFileInfo info;
    info.setFile(file);
    if(!info.dir().exists())
        QDir().mkdir(info.path());

    QFile f(file);
        
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)){
         qDebug()<<"cannot write stream";
    }
    else{
          QTextStream stream(&f);
        
          for(int i=0; i<set.size(); i++){
                stream << set.at(i)<<'\n';
          }
    }
         f.close();

         
}

void addtodatabase::setSingerFirst(bool p){

    if(p){
        singer_first=1;
        cmb_titlefirst->setChecked(0);
        title_first=0;
    }
    else{
        singer_first=0;
        title_first=1;
        cmb_singerfirst->setChecked(1);
    }

}

void addtodatabase::setTitleFirst(bool j){

    if(j){
        title_first=1;
        singer_first=0;
        cmb_singerfirst->setChecked(0);
        //button_start->setEnabled(1);
    }
    else{
        title_first=0;
        singer_first=1;
      cmb_titlefirst->setChecked(1);
    }

}

void addtodatabase::getDrive(){

        combo_mounted->clear();
        combo_mounted->addItem(QDir::homePath() );
    //get mounted drive just support ntfs
        foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
                  if (storage.fileSystemType()=="ntfs-3g" || storage.fileSystemType()=="fuseblk") {

                          combo_mounted->addItem(storage.rootPath());

                     }
              }
}

void addtodatabase::setToAuto(bool a){

    if(a){
        automatic=true;
        manual=false;
        cb_splitby->setChecked(false);
    }
    else{
          automatic=false;
        manual=true;
        cb_splitby->setChecked(true);
    }

}

void addtodatabase::setToManual(bool m){

    if(m){
        automatic=false;
        manual=true;
        cb_auto->setChecked(false);
    }
    else{

        automatic=true;
        manual=false;
        cb_auto->setChecked(true);
    }

}

QString addtodatabase::getSplitter(const QString &filename){
    QStringList old_splitter;
    QStringList new_splitter;

//identify splitter
    QRegularExpression exp("[^a-zA-Z0-9]+");
    QRegularExpressionMatchIterator i =exp.globalMatch(filename);

    while(i.hasNext()){
        QRegularExpressionMatch match=i.next();

       old_splitter<< filename.at(match.capturedEnd()-1);
    }

    //trim double item
    for (int i=0;i<old_splitter.size();i++){
        if(!new_splitter.contains(old_splitter.at(i))){
            new_splitter<<old_splitter.at(i);
        }
    }

    int max=0;
    QString splitter="";
    
//get greater count
    for(int x=0;x<new_splitter.size();x++){

        if(max<old_splitter.count(new_splitter.at(x))){

            splitter = new_splitter.at(x);

        }

        max = old_splitter.count(new_splitter.at(x));

    }
    
    return splitter;
    
}

void addtodatabase::enableStartButton(){

}
