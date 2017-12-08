#include "editdb.h"
#include "dbmanager.h"
#include "liststringfileparser.h"
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QGridLayout>
#include <QTableWidget>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QSqlQuery>
#include <QHeaderView>
#include <QSizePolicy>
#include <QSqlError>
#include <QMessageBox>
#include <QtAlgorithms>
// for qSort()


managedb::managedb(QWidget *parent) :
    QDialog(parent)
{

    QVBoxLayout *lo_main = new QVBoxLayout;

    QHBoxLayout *lo_top = new QHBoxLayout;

    QGroupBox *grup_singer = new QGroupBox("Singer", this);

   list_singer = new QListWidget(this);
    list_singer->addItems(listStringFileParser::parse(QDir::homePath()+"/.elroke/meta/singer"));

    connect(list_singer,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(onListWidgetClicked(QListWidgetItem *)));
    QVBoxLayout *lo_grup_singer = new QVBoxLayout;


    lo_grup_singer->addWidget(list_singer);
    grup_singer->setLayout(lo_grup_singer);

    QGroupBox *grup_language = new QGroupBox("Language", this);

    list_language = new QListWidget(this);
    list_language->addItems(listStringFileParser::parse(QDir::homePath()+"/.elroke/meta/language"));
    connect(list_language, &QListWidget::itemClicked,this,&managedb::onListWidgetClicked);

    QVBoxLayout *lo_grup_language = new QVBoxLayout;
    lo_grup_language->addWidget(list_language);
    grup_language->setLayout(lo_grup_language);

     QGroupBox *grup_genre = new QGroupBox("Genre/category", this);
   list_genre = new QListWidget(this);
    list_genre->addItems(listStringFileParser::parse(QDir::homePath()+"/.elroke/meta/category"));
     connect(list_genre, &QListWidget::itemClicked,this,&managedb::onListWidgetClicked);


    QVBoxLayout *lo_grup_genre = new QVBoxLayout;
    lo_grup_genre->addWidget(list_genre);
    grup_genre->setLayout(lo_grup_genre);

     QGroupBox *grup_folder = new QGroupBox("Folder/Path", this);
   list_folder = new QListWidget(this);
    list_folder->addItems(listStringFileParser::parse(QDir::homePath()+"/.elroke/meta/path"));
     connect(list_folder, &QListWidget::itemClicked,this,&managedb::onListWidgetClicked);

    QVBoxLayout *lo_grup_folder = new QVBoxLayout;

    lo_grup_folder->addWidget(list_folder);

    grup_folder->setLayout(lo_grup_folder);

    lo_top->addWidget(grup_singer);
    lo_top->addWidget(grup_language);
    lo_top->addWidget(grup_genre);
    lo_top->addWidget(grup_folder);
    lo_top->setMargin(0);
    lo_top->setSpacing(0);


    /// bottom area ---------------------------------------------------------------------

    //DATABASE
    dbmanager *db = new dbmanager(dbmanager::edit, this);

    db->connectToDB();

    sql_model = new QSqlTableModel(this, db->database());
    sql_model->setTable("ELROKE123");
    sql_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    sql_model->select();



    proxy_model = new ProxyModel(ProxyModel::column, this);
    proxy_model->setSourceModel(sql_model);
    proxy_model->setAlignment(6, Qt::AlignCenter);

    connect(this,SIGNAL(toSearch(QVariantList)),proxy_model,SLOT(search(QVariantList)));

    QHBoxLayout *lo_bottom = new QHBoxLayout;
    QFrame *frame_left_bottom = new QFrame;

    QGridLayout *glo_button = new QGridLayout;

    QPushButton *button_title_singer = new QPushButton("Title <--> Singer", this);
    connect(button_title_singer, &QPushButton::clicked, this, &managedb::swapTitleSinger);

    QPushButton *button_singer_language = new QPushButton("Singer <--> Language", this);
    connect(button_singer_language,&QPushButton::clicked,this,&managedb::swapSingerLanguage);

    QPushButton *button_title_language = new QPushButton("Title <--> Language", this);
    connect(button_title_language, &QPushButton::clicked,this,&managedb::swapTitleLanguage);

    QPushButton *button_singer_category = new QPushButton("Singer <--> Category",this);
    connect(button_singer_category,&QPushButton::clicked,this,&managedb::swapSingerCategory);

    QPushButton *button_title_category = new QPushButton("Title <--> Category", this);
    connect(button_title_category,&QPushButton::clicked,this,&managedb::swapTitleCategory);

    QPushButton *button_language_category = new QPushButton("Language <--> Category", this);
    connect(button_language_category,&QPushButton::clicked,this,&managedb::swapLanguageCategory);

    QPushButton *button_set_title = new QPushButton("Set Title", this);
    connect(button_set_title,&QPushButton::clicked,this,&managedb::setTitle);

    le_set_title = new CLineEdit(this);
    le_set_title->setPlaceholderText("Title");


    QPushButton *button_set_singer = new QPushButton("Set Singer", this);
    connect(button_set_singer,&QPushButton::clicked,this,&managedb::setSinger);

    le_set_singer = new CLineEdit(this);
    le_set_singer->setPlaceholderText("Singer");


    QPushButton *button_set_language = new QPushButton("Set Language", this);
    connect(button_set_language,&QPushButton::clicked,this,&managedb::setLanguage);

    le_set_language = new CLineEdit(this);
    le_set_language->setPlaceholderText("Language");

    QPushButton *button_set_category = new QPushButton("Set Category", this);
    connect(button_set_category,&QPushButton::clicked,this,&managedb::setCategory);

    le_set_category = new CLineEdit(this);
    le_set_category->setPlaceholderText("Category");


    combo_audio_channel = new QComboBox(this);
    combo_audio_channel->addItem("LEFT");
    combo_audio_channel->addItem("RIGHT");
    combo_audio_channel->addItem("STEREO");

    QPushButton *button_set_audio_channel = new QPushButton("Set Audio Channel");
    connect(button_set_audio_channel,&QPushButton::clicked,this,&managedb::setAudioChannel);

    table = new QTableView(this);

    QPushButton *button_selectall = new QPushButton("Select All", this);
    connect(button_selectall,&QPushButton::clicked,table,&QTableView::selectAll);

    QPushButton *button_unselect = new QPushButton("Unselect", this);
    connect(button_unselect,&QPushButton::clicked,table,&QTableView::clearSelection);

    QPushButton *button_delete_selected = new QPushButton("Delete Selected", this);
    connect(button_delete_selected,&QPushButton::clicked,this,&managedb::deleteItem);

    QPushButton *button_save = new QPushButton("SAVE", this);
    connect(button_save, &QPushButton::clicked, this, &managedb::save);

    QPushButton *button_undo = new QPushButton("UNDO", this);
    connect(button_undo, &QPushButton::clicked, this, &managedb::undo);

    QPushButton *button_close = new QPushButton("CLOSE", this);
    connect(button_close,SIGNAL(pressed()),this,SLOT(dclose()));

    glo_button->addWidget( button_title_singer,0,0);
    glo_button->addWidget( button_singer_language,0,1);
    glo_button->addWidget(button_title_language,1,0);
    glo_button->addWidget(button_singer_category,1,1);
    glo_button->addWidget(button_title_category,2,0);
    glo_button->addWidget(button_language_category,2,1);
    glo_button->addWidget(button_set_title,3,1);
    glo_button->addWidget(le_set_title,3,0);
    glo_button->addWidget(button_set_singer,4,1);
    glo_button->addWidget(le_set_singer,4,0);
    glo_button->addWidget(button_set_language,5,1);
    glo_button->addWidget(le_set_language,5,0);
    glo_button->addWidget(button_set_category,6,1);
    glo_button->addWidget(le_set_category,6,0);
    glo_button->addWidget(combo_audio_channel,7,0);
    glo_button->addWidget(button_set_audio_channel,7,1);
    glo_button->addWidget(button_selectall,8,1);
    glo_button->addWidget(button_delete_selected,8,0);
    glo_button->addWidget(button_save,9,0);
    glo_button->addWidget(button_unselect,9,1);
    glo_button->addWidget(button_close,10,0);
    glo_button->addWidget(button_undo,10,1);
    glo_button->setMargin(0);
    frame_left_bottom->setLayout(glo_button);

    QSizePolicy spLeft(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spLeft.setHorizontalStretch(1);
    frame_left_bottom->setSizePolicy(spLeft);


    lo_bottom->addWidget(frame_left_bottom);

//right bottom
    QHBoxLayout *lo_search = new QHBoxLayout;
   combo_search = new QComboBox(this);
    combo_search->addItem("Title");
    combo_search->addItem("Singer");
    connect(combo_search,SIGNAL(activated(int)),this,SLOT(comboSearchChange(int)));



    le_search = new CLineEdit(this);
    le_search->setMaximumWidth(300);
    le_search->setPlaceholderText("SEARCH");
    connect(le_search,&CLineEdit::textChanged,this,&managedb::receiverSearch);

    CLineEdit *le_jump = new CLineEdit(this);
le_jump->setFixedWidth(100);
le_jump->setValidator(new QIntValidator(this));
connect( le_jump,SIGNAL(textChanged(QString)), this, SLOT(jumpTo(QString)));

auto *reset = new QPushButton("Show All", this);
connect(reset,SIGNAL(pressed()),proxy_model,SLOT(reset()));


    lo_search->addWidget(combo_search);
    lo_search->addWidget(le_search);
    lo_search->addWidget(new QLabel("Jump to : "));
    lo_search->addWidget(le_jump);
    lo_search->addWidget(reset);
    lo_search->addStretch();


    table->setModel(proxy_model);
//    table->verticalHeader()->hide();
    table->resizeColumnsToContents();
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode( QAbstractItemView::ExtendedSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->model()->setHeaderData(6, Qt::Horizontal,Qt::AlignLeft, Qt::TextAlignmentRole);
   connect(table->selectionModel(),&QItemSelectionModel::selectionChanged,this,&managedb::selectedCount);

   QPalette header_palette = table->horizontalHeader()->palette();
   header_palette.setBrush(QPalette::Button, Qt::blue);
   header_palette.setColor(QPalette::Background, Qt::transparent);
   header_palette.setColor(QPalette::ButtonText, Qt::red);
   table->horizontalHeader()->setPalette(header_palette);
   table->verticalHeader()->setPalette(header_palette);

    QSizePolicy spRight(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spRight.setHorizontalStretch(2);

    QVBoxLayout *lo_bottom_right = new QVBoxLayout;

    QHBoxLayout *lo_label_total_count = new QHBoxLayout;

    total_count_label = new QLabel(QString::number(sql_model->rowCount()), this);
    selected_count_label = new QLabel(QString::number(0), this);

    lo_label_total_count->addWidget(new QLabel("Selected : ", this));
    lo_label_total_count->addWidget(selected_count_label);
    lo_label_total_count->addStretch();
    lo_label_total_count->addWidget(new QLabel("Total : ", this));
    lo_label_total_count->addWidget(total_count_label);
    lo_label_total_count->addStretch();

    lo_bottom_right->addLayout(lo_search);
    lo_bottom_right->addWidget(table);
    lo_bottom_right->addLayout(lo_label_total_count);
    lo_bottom_right->setMargin(0);

    QWidget *w_bottom  = new QWidget(this);
    w_bottom->setLayout(lo_bottom_right);
    w_bottom->setSizePolicy(spRight);


    lo_bottom->addWidget(w_bottom);
    lo_bottom->setMargin(0);

    QFrame *top = new QFrame(this);
    QSizePolicy spTop (QSizePolicy::Preferred,QSizePolicy::Preferred);
    spTop.setVerticalStretch(1);
    top->setSizePolicy(spTop);


    QFrame *bottom = new QFrame(this);
    QSizePolicy spBottom (QSizePolicy::Preferred,QSizePolicy::Preferred);
    spBottom.setVerticalStretch(2);
    bottom->setSizePolicy(spBottom);


    top->setLayout(lo_top);
    bottom->setLayout(lo_bottom);

    lo_main->addWidget(top);
    lo_main->addWidget(bottom);
    lo_main->setMargin(0);
    setLayout(lo_main);
    setWindowTitle("Manage Database");



    setWindowFlags(Qt::FramelessWindowHint);
    showFullScreen();

    kboard =new Keyboard(this);
    kboard->hide();

    connect(le_set_title,&CLineEdit::focussed,kboard,&Keyboard::setVisible);
    connect(le_set_singer,&CLineEdit::focussed,kboard,&Keyboard::setVisible);
    connect(le_set_language,&CLineEdit::focussed,kboard,&Keyboard::setVisible);
    connect(le_set_category,&CLineEdit::focussed,kboard,&Keyboard::setVisible);
    connect(le_search,&CLineEdit::focussed,kboard,&Keyboard::setVisible);

}

void managedb::swapTitleSinger(){

    swapItem(1,2);
      anyChange=true;

}

void managedb::swapItem(int column1, int column2){


    QModelIndexList list_selected = table->selectionModel()->selectedRows();

    foreach (QModelIndex indexes, list_selected) {
        int r =  proxy_model->mapToSource( indexes).row();

        QString data1 = sql_model->index(r,column1).data().toString();
        QString data2 = sql_model->index(r,column2).data().toString();
        sql_model->setData(sql_model->index(r, column1), data2);
        sql_model->setData(sql_model->index(r, column2), data1);

    }

}

void managedb::swapSingerLanguage(){

    swapItem(2,3);
  anyChange=true;
}

void managedb::swapTitleLanguage(){


    swapItem(1,3);

  anyChange=true;
}
void managedb::swapSingerCategory(){
    swapItem(2,4);
      anyChange=true;
}

void managedb::swapTitleCategory(){
    swapItem(1,4);
  anyChange=true;
}

void managedb::swapLanguageCategory(){

    swapItem(3,4);
    anyChange=true;
}

void managedb::setitem(QString text, int column){

    QModelIndexList list_selected = table->selectionModel()->selectedRows();
int r=0;

    foreach (QModelIndex indexes, list_selected) {
        r =  proxy_model->mapToSource( indexes).row();
        sql_model->setData(sql_model->index(r, column), text);

    }

   QModelIndex index = table->indexAt(QPoint(0,0));
   if(!index.isValid())
       proxy_model->reset();
}

void managedb::setTitle(){

    if(!le_set_title->text().isEmpty())
    setitem(le_set_title->text(), 1);
    anyChange=true;

}
void managedb::setSinger(){

    if(!le_set_singer->text().isEmpty())
    setitem(le_set_singer->text(), 2);
    anyChange=true;
}
void managedb::setLanguage(){

    if(!le_set_language->text().isEmpty())
    setitem(le_set_language->text(), 3);
    anyChange=true;
}
void managedb::setCategory(){

    if(!le_set_category->text().isEmpty())
    setitem(le_set_category->text(), 4);
    anyChange=true;

}

void managedb::setAudioChannel(){

    setitem(combo_audio_channel->currentText(),5);
    anyChange=true;

}


void managedb::deleteItem(){
setCursor(Qt::WaitCursor);

    QModelIndexList listInd =  table->selectionModel()->selectedRows();
    if(listInd.size()==sql_model->rowCount()){

        QMessageBox::StandardButton warning;
        warning = QMessageBox::question(this, "Warning!", "Are you sure want to delete all item?",
                                        QMessageBox::Yes | QMessageBox::No);
        if(warning==QMessageBox::Yes){
            foreach (QModelIndex ind, listInd) {
                table->hideRow(proxy_model->mapToSource(ind).row());
                sql_model->removeRow(proxy_model->mapToSource(ind).row());
            }


        }
        else
        {
            setCursor(Qt::ArrowCursor);
        return;
        }
    }

    foreach (QModelIndex ind, listInd) {
        table->hideRow(proxy_model->mapToSource(ind).row());// need to hiding row first, deleted row will disappear after submit.
        sql_model->removeRow(proxy_model->mapToSource(ind).row());
    }
    QModelIndex index = table->indexAt(QPoint(0,0));
    if(!index.isValid())
        proxy_model->reset();
    anyChange=true;
    setCursor(Qt::ArrowCursor);
}



void managedb::save(){

    if(!anyChange)
        return;

    if(!sql_model->submitAll())
        qDebug()<<"error o submit"<<sql_model->lastError();
    sql_model->select();
    total_count_label->setText(QString::number(sql_model->rowCount()));
    anyChange=false;

    updateList();
}


void managedb::onListWidgetClicked(QListWidgetItem *item){

    setCursor(Qt::WaitCursor);

    table->clearSelection();

    QObject *obj = sender();
    QString text=item->text();

    int column=0;

    if (obj==list_singer)
        column=1;
    else if(obj==list_language)
        column=2;
    else if(obj==list_genre)
        column=3;
    else if(obj==list_folder)
            column=6;

    QVariantList list;
    list.append(column);
    list.append(text);

    emit toSearch(list);
    table->selectAll();

    setCursor(Qt::ArrowCursor);

}

void managedb::updateList(){

    setCursor(Qt::WaitCursor);

    //QSET FOR REMOVE DUPPLICATE
    QSet<QString>set_singer;
    QSet<QString>set_language;
    QSet<QString>set_category;
    QSet<QString>set_folder;

    for(int i=0; i<sql_model->rowCount(); i++){

        set_singer.insert(sql_model->data(sql_model->index(i,2),Qt::DisplayRole).toString().toUpper());
         set_language.insert(sql_model->data(sql_model->index(i,3),Qt::DisplayRole).toString().toUpper());
          set_category.insert(sql_model->data(sql_model->index(i,4),Qt::DisplayRole).toString().toUpper());
          QFileInfo info;
          info.setFile(sql_model->data(sql_model->index(i,7),Qt::DisplayRole).toString());
           set_folder.insert(info.path());


    }

    QList<QString>singer= set_singer.toList();
    QList<QString>lang = set_language.toList();
    QList<QString>cat = set_category.toList();
    QList<QString>path= set_folder.toList();


    //SORT LIST
    qSort(singer.begin(), singer.end());
    qSort(lang.begin(), lang.end());
    qSort(cat.begin(),cat.end());
    qSort(path.begin(), path.end());

    list_singer->clear();
    list_singer->addItems(singer);
    list_language->clear();
    list_language->addItems(lang);
    list_genre->clear();
    list_genre->addItems(cat);
    list_folder->clear();
    list_folder->addItems(path);


    setCursor(Qt::ArrowCursor);

}

managedb::~managedb(){

//update latest change
    QSet<QString>set_singer;
    QSet<QString>set_language;
    QSet<QString>set_category;
    QSet<QString>set_folder;

    for(int i=0; i<sql_model->rowCount(); i++){

        set_singer.insert(sql_model->data(sql_model->index(i,2),Qt::DisplayRole).toString().toUpper());
         set_language.insert(sql_model->data(sql_model->index(i,3),Qt::DisplayRole).toString().toUpper());
          set_category.insert(sql_model->data(sql_model->index(i,4),Qt::DisplayRole).toString().toUpper());
          QFileInfo info;
          info.setFile(sql_model->data(sql_model->index(i,7),Qt::DisplayRole).toString());
           set_folder.insert(info.path());

    }

    QList<QString>singer= set_singer.toList();
    QList<QString>lang = set_language.toList();
    QList<QString>cat = set_category.toList();
    QList<QString>path= set_folder.toList();

    qSort(singer.begin(), singer.end());
    qSort(lang.begin(), lang.end());
    qSort(cat.begin(),cat.end());
    qSort(path.begin(), path.end());

    writeTextStream(QDir::homePath()+"/.elroke/meta/singer", singer);
     writeTextStream(QDir::homePath()+"/.elroke/meta/language", lang);
      writeTextStream(QDir::homePath()+"/.elroke/meta/category", cat);
       writeTextStream(QDir::homePath()+"/.elroke/meta/path", path);


}

void managedb::writeTextStream(const QString &file, QList<QString>set){
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

void managedb::selectedCount(){
    QModelIndexList list = table->selectionModel()->selectedRows();
    selected_count_label->setText(QString::number(list.count()));
}

void managedb::dclose(){

    if(anyChange){
    QMessageBox::StandardButton warning;
    warning = QMessageBox::question(this, "Warning","Change not save yet. Save?", QMessageBox::Yes | QMessageBox::No);
    if(warning==QMessageBox::Yes){
      sql_model->submitAll();
    }
    }

    close();

}


void managedb::comboSearchChange(int i){
    QVariantList list;
    list.append(i);
    QString text = le_search->text();
    if(text==NULL)
        return;

    list.append(text);

    emit toSearch(list);
}

void managedb::receiverSearch(QString s){
    QVariantList list;
    list.append(combo_search->currentIndex());
    list.append(s);

    emit toSearch(list);
}

void managedb::jumpTo(QString t){

    int x = t.toInt();
    table->scrollTo( table->model()->index(x-1,0), QAbstractItemView::PositionAtTop);

}

void managedb::undo(){


    sql_model->revertAll();

    //unhide row
    for(int i=0; i<sql_model->rowCount();i++){

        table->setRowHidden(i,0);


    }



}
