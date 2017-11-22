#include "addtodatabase.h"
#include <dbmanager.h>
#include <QHBoxLayout>
#include <QStorageInfo>
#include <QLabel>
#include <QHeaderView>
#include <QLineEdit>
#include <QGridLayout>
#include <QGroupBox>
#include <QDebug>
#include <QTime>
#include <QPainter>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariantList>
//#include <QScrollBar>


addtodatabase::addtodatabase(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *lo_main = new QVBoxLayout;

    QHBoxLayout *lo_top = new QHBoxLayout;
    QVBoxLayout *lo_top_left = new QVBoxLayout;

    QPalette dark_palet ;
    dark_palet.setColor(QPalette::Window, palette().dark().color());
    dark_palet.setColor(QPalette::WindowText, Qt::black);
    dark_palet.setColor(QPalette::Base, palette().shadow().color());
    dark_palet.setColor(QPalette::Button, palette().dark().color());

     combo_mounted = new QComboBox(this);

    combo_mounted->addItem(QDir::homePath());

//get mounted drive
    getDrive();

    connect(combo_mounted, SIGNAL(activated(QString)), this, SLOT(setDrive(QString)));

    QPushButton *btn_refresh = new QPushButton(QIcon::fromTheme("stock_refresh"),"", this);
    btn_refresh->setFixedWidth(40);
    connect(btn_refresh,SIGNAL(clicked(bool)),this,SLOT(getDrive()));

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

     file_model = new QFileSystemModel(this);
     file_model->setFilter(QDir::Files | QDir::NoDotAndDotDot);
     file_model->setNameFilters(QStringList()<<"*.mp4"<<"*.mkv"<<"*.mpg"<<"*.dat"<<"*.avi"<<"*.mov"<<"*.webm");
     file_model->setNameFilterDisables(false);

     file_model->setRootPath(pakDir());

     //list name files
     lw_list = new QListWidget(this);

     QHBoxLayout *lo_drive = new QHBoxLayout;
     lo_drive->addWidget(combo_mounted);
     lo_drive->addWidget(btn_refresh);
     lo_top_left->addLayout(lo_drive);
     lo_top_left->addWidget(tv_folder);
     lo_top ->addLayout(lo_top_left);
     lo_top->addWidget(lw_list);

    QHBoxLayout *lo_g = new QHBoxLayout;
    // ----

    QVBoxLayout *lo_splitter = new QVBoxLayout;
    cb_auto = new QCheckBox("Auto", this);
    cb_auto->setChecked(automatic);
    connect(cb_auto,SIGNAL(toggled(bool)),this,SLOT(setToAuto(bool)));

    QHBoxLayout *lo_use_char = new QHBoxLayout;
    QCheckBox *cb_splitby= new QCheckBox("Split by", this);
    connect(cb_splitby,SIGNAL(toggled(bool)),this,SLOT(setToManual(bool)));

    le_splitter = new QLineEdit(this);
    le_splitter->setFixedWidth(30);
    le_splitter->setText(splitter);
    connect(le_splitter,SIGNAL(textChanged(QString)),this,SLOT(splitterChange(QString)));
    lo_use_char->addWidget(cb_splitby);
    lo_use_char->addWidget(le_splitter);
    lo_use_char->addStretch();
    lo_splitter->addWidget(cb_auto);
    lo_splitter->addLayout(lo_use_char);

    QGroupBox *grup_splitter = new QGroupBox("Splitter", this);
    grup_splitter->setLayout(lo_splitter);

    QVBoxLayout *lo_pattern = new QVBoxLayout;

    cmb_titlefirst = new QCheckBox( this);
    cmb_titlefirst->setText("Title"+splitter+"Singer");
    cmb_titlefirst->setChecked(title_first);
    connect(cmb_titlefirst,SIGNAL(clicked(bool)),this,SLOT(setTitleFirst(bool)));

    cmb_singerfirst = new QCheckBox(this);
    cmb_singerfirst->setText("Singer"+splitter+"Title");
    connect(cmb_singerfirst,SIGNAL(clicked(bool)),this,SLOT(setSingerFirst(bool)));

    lo_pattern->addWidget(cmb_titlefirst);
    lo_pattern->addWidget(cmb_singerfirst);

    QGroupBox *grup_pattern = new QGroupBox("Pattern", this);
    grup_pattern->setLayout(lo_pattern);

   QGridLayout *lo_additional_item = new QGridLayout;

   QLineEdit *le_singer = new QLineEdit(this);
   QLineEdit *le_language = new QLineEdit(this);
   QLineEdit *le_category = new QLineEdit(this);

   lo_additional_item->addWidget( new QLabel("Singer"),0,0);
   lo_additional_item->addWidget(le_singer,0,1);
   lo_additional_item->addWidget(new QLabel("Language", this), 1,0);
   lo_additional_item->addWidget(le_language,1,1);
   lo_additional_item->addWidget(new QLabel("Category/Genre", this),2,0);
   lo_additional_item->addWidget(le_category,2,1);


   QGroupBox *gr_addcat = new QGroupBox("Add category if not available");
   gr_addcat->setLayout(lo_additional_item);

   QVBoxLayout *lo_audio_channel = new QVBoxLayout;

   QCheckBox *combo_channel_left = new QCheckBox("Left", this);
   QCheckBox *combo_channel_right = new QCheckBox("Right", this);

   lo_audio_channel->addWidget(combo_channel_left);
   lo_audio_channel->addWidget(combo_channel_right);
   lo_audio_channel->addStretch();

   QGroupBox *gr_audio_channel = new QGroupBox("Audio Channel", this);

   gr_audio_channel->setLayout(lo_audio_channel);

   lo_g->addWidget(grup_splitter);
   lo_g->addWidget(grup_pattern);
   lo_g->addWidget(gr_addcat);
   lo_g->addWidget(gr_audio_channel);

   QHBoxLayout *lo_btn = new QHBoxLayout;

   btn_start = new QPushButton("Start", this);
   connect(btn_start,SIGNAL(clicked(bool)),this,SLOT(saveToDatabase()));

   QPushButton *btn_cancel = new QPushButton("Cancel", this);

   connect(btn_cancel,SIGNAL(clicked(bool)),this,SLOT(close()));

   lo_btn->addWidget(btn_cancel);
   lo_btn->addStretch();
   lo_btn->addWidget(btn_start);

    lo_main->addLayout(lo_top);
    lo_main->addLayout(lo_g);
    lo_main->addLayout(lo_btn);

    setLayout(lo_main);
    setPalette(dark_palet);

    setMinimumSize(1000,400);

    QFont font;
    font.setPointSize(12);
    setFont(font);

    setWindowFlags(Qt::FramelessWindowHint);
}

void addtodatabase::setDrive(const QString &drive){

   tv_folder->setRootIndex(dir_model->index(drive));

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
    cmb_titlefirst->setText("Title"+split+"Singer");
    cmb_singerfirst->setText("Singer"+split+"Title");

}

void addtodatabase::saveToDatabase(){

    setCursor(Qt::BusyCursor);
    btn_start->setEnabled(0);
     splitter = le_splitter->text();

    QDirIterator it(working_path,QStringList()<<"*.mp4"<<"*.avi"<<"*.dat"<<"*.mkv"<<"*.mpg"<<"*.mov", QDir::Files,QDirIterator::Subdirectories);

    QStringList filename;
     QString title, singer, language, category, a_channel, path,folder;
     QFileInfo info;
     QString namefile;

     dbmanager *db = new dbmanager(dbmanager::add, this);
     db->setDBName("elroke");
     db->connectToDB();
     db->prepare();

     bool sql_ok=true;


    QVariantList data;
 //   QSet<QString> set_title;
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
            qDebug()<<"failed"<<namefile<<title<<singer;
        }

        //set_title.insert(title);
        set_singer.insert(singer);
        set_language.insert(language);
        set_category.insert(category);
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

    grabMetadata(set_singer, set_language, set_category, set_folder);

     setCursor(Qt::ArrowCursor);

      this->accept();

 }


void addtodatabase::grabMetadata(const QSet<QString> &singer, const QSet<QString> &language, const QSet<QString> &category, const QSet<QString> &path){
    
    QString dir = QDir::homePath()+"/.config/elroke/meta";
    
    if(!QDir(dir).exists())
        QDir().mkpath(dir);


    QFile file_singer(dir+"/singer");
    QFile file_language(dir+"/language");
    QFile file_category(dir+"/category");
    QFile file_path(dir+"/path");


    if(!file_singer.open(QIODevice::WriteOnly | QIODevice::Text)){
         qDebug()<<"cannot write singer";
    }
    else{
          QTextStream stream(&file_singer);
          for(int i=0; i<singer.size(); i++){
                stream << singer.values().at(i)<<'\n';}

    }

         file_singer.close();

         if(!file_language.open(QIODevice::WriteOnly | QIODevice::Text)){
              qDebug()<<"cannot write singer";
         }
         else{
               QTextStream stream(&file_language);
               for(int i=0; i<language.size(); i++){
                     stream << language.values().at(i)<<'\n';
               }
         }
              file_language.close();

              if(!file_category.open(QIODevice::WriteOnly | QIODevice::Text)){
                   qDebug()<<"cannot write singer";
              }
              else{
                    QTextStream stream(&file_category);
                    for(int i=0; i<category.size(); i++){
                          stream << category.values().at(i)<<'\n';
                    }
              }
                   file_category.close();


                   if(!file_path.open(QIODevice::WriteOnly | QIODevice::Text)){
                        qDebug()<<"cannot write singer";
                   }
                   else{
                         QTextStream stream(&file_path);
                         for(int i=0; i<path.size(); i++){
                               stream << path.values().at(i)<<'\n';
                         }
                   }
                        file_path.close();


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
        //btn_start->setEnabled(1);
    }
    else{
        title_first=0;
        singer_first=1;
      cmb_titlefirst->setChecked(1);
    }

}

void addtodatabase::getDrive(){

        combo_mounted->clear();

    //get mounted drive just support ntfs
        foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
                  if (storage.fileSystemType()=="ntfs-3g" || storage.fileSystemType()=="fuseblk") {

                          combo_mounted->addItem(storage.rootPath());

                     }
              }
}

void addtodatabase::setToAuto(bool a){
    qDebug()<<"a"<<a;
    if(a){
        automatic=true;
        manual=false;
        cb_splitby->setChecked(false);
    }
    else{
          qDebug()<<"a"<<a;
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

void addtodatabase::paintEvent(QPaintEvent *){

    QColor backgroundColor = palette().dark().color();
    backgroundColor.setAlpha(200);
    QPainter customPainter(this);
    customPainter.fillRect(rect(), backgroundColor);

}

QString addtodatabase::getSplitter(const QString &filename){
    QStringList old_splitter;
    QStringList new_splitter;
   // QString text= "judul##artis##kategori#b,ahasa##saluran.mp4";

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

