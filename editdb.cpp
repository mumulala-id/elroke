#include "editdb.h"
#include "dbmanager.h"
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QGridLayout>
#include <QTableWidget>
#include <QComboBox>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QSqlQuery>
#include <QHeaderView>
#include <QSizePolicy>
#include <QSqlError>
#include <QMessageBox>


managedb::managedb(QWidget *parent) :
    QDialog(parent)
{

    QVBoxLayout *lo_main = new QVBoxLayout;

    QHBoxLayout *lo_top = new QHBoxLayout;

    QGroupBox *grup_singer = new QGroupBox("Singer", this);
    list_singer = new QListWidget(this);
    list_singer->addItems(readListOfFile(QDir::homePath()+"/.config/elroke/meta/singer"));

    connect(list_singer,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(onListWidgetClicked(QListWidgetItem *)));
    QVBoxLayout *lo_grup_singer = new QVBoxLayout;

    check_singer = new QCheckBox("Filter Singer", this);
    QPushButton *button_replace_singer = new QPushButton("Replace", this);
    QHBoxLayout *lo_cb = new QHBoxLayout;
    lo_cb->addWidget(check_singer);
    lo_cb->addWidget(button_replace_singer);
    le_singer = new QLineEdit(this);
    lo_grup_singer->addWidget(list_singer);
    lo_grup_singer->addLayout(lo_cb);
    lo_grup_singer->addWidget(le_singer);
    grup_singer->setLayout(lo_grup_singer);

    QGroupBox *grup_language = new QGroupBox("Language", this);

    list_language = new QListWidget(this);
    list_language->addItems(readListOfFile(QDir::homePath()+"/.config/elroke/meta/language"));

    check_language = new QCheckBox("Filter Language", this);

    QPushButton *button_replace_language = new QPushButton("Replace",this);
    QHBoxLayout *lo_cb_language = new QHBoxLayout;
    lo_cb_language->addWidget(check_language);
    lo_cb_language->addWidget(button_replace_language);

    le_language = new QLineEdit(this);
    QVBoxLayout *lo_grup_language = new QVBoxLayout;
    lo_grup_language->addWidget(list_language);
    lo_grup_language->addLayout(lo_cb_language);
    lo_grup_language->addWidget(le_language);
    grup_language->setLayout(lo_grup_language);

     QGroupBox *grup_genre = new QGroupBox("Genre/category", this);
    list_genre = new QListWidget(this);
    list_genre->addItems(readListOfFile(QDir::homePath()+"/.config/elroke/meta/category"));

    check_genre = new QCheckBox("Filter Genre", this);
    QPushButton *button_replace_genre = new QPushButton("Replace", this);
    QHBoxLayout *lo_cb_genre = new QHBoxLayout;
    lo_cb_genre->addWidget(check_genre);
    lo_cb_genre->addWidget(button_replace_genre);
    le_genre = new QLineEdit(this);

    QVBoxLayout *lo_grup_genre = new QVBoxLayout;
    lo_grup_genre->addWidget(list_genre);
    lo_grup_genre->addLayout(lo_cb_genre);
    lo_grup_genre->addWidget(le_genre);
    grup_genre->setLayout(lo_grup_genre);

     QGroupBox *grup_folder = new QGroupBox("Folder/Path", this);
    list_folder = new QListWidget(this);
    list_folder->addItems(readListOfFile(QDir::homePath()+"/.config/elroke/meta/path"));
    QVBoxLayout *lo_grup_folder = new QVBoxLayout;

    QHBoxLayout *lo_cb_folder = new QHBoxLayout;
    check_genre_folder = new QCheckBox("Filter Genre", this);
    le_title_fixed = new QLineEdit(this);

    lo_cb_folder->addWidget(check_genre_folder);
    lo_cb_folder->addStretch();
    lo_cb_folder->addWidget(new QLabel("Filter Title", this));

    QComboBox *cmb_filter = new QComboBox(this);
    cmb_filter->addItems(QStringList()<<"Start with"<<"Contain"<<"End with");
    lo_cb_folder->addWidget(cmb_filter);
    lo_cb_folder->addWidget(le_title_fixed);

    lo_grup_folder->addWidget(list_folder);
    lo_grup_folder->addLayout(lo_cb_folder);

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
    db->setDBName("elroke");
    db->connectToDB();

    sql_model = new QSqlTableModel(this, db->database());
    sql_model->setTable("elroke123");
    sql_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    sql_model->select();



    proxy_model = new ProxyModel(this);
    proxy_model->setSourceModel(sql_model);

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

    le_set_title = new QLineEdit(this);
    le_set_title->setPlaceholderText("Title");
    QPushButton *button_set_singer = new QPushButton("Set Singer", this);
    connect(button_set_singer,&QPushButton::clicked,this,&managedb::setSinger);

    le_set_singer = new QLineEdit(this);
    le_set_singer->setPlaceholderText("Singer");
    QPushButton *button_set_language = new QPushButton("Set Language", this);
    connect(button_set_language,&QPushButton::clicked,this,&managedb::setLanguage);

    le_set_language = new QLineEdit(this);
    le_set_language->setPlaceholderText("Language");

    QPushButton *button_set_category = new QPushButton("Set Category", this);
    connect(button_set_category,&QPushButton::clicked,this,&managedb::setCategory);

    le_set_category = new QLineEdit(this);
    le_set_category->setPlaceholderText("Category");

    table = new QTableView(this);

    QPushButton *button_selectall = new QPushButton("Select All", this);
    connect(button_selectall,&QPushButton::clicked,table,&QTableView::selectAll);

QPushButton *button_unselect = new QPushButton("Unselect", this);
connect(button_unselect,&QPushButton::clicked,table,&QTableView::clearSelection);
    QPushButton *button_delete_selected = new QPushButton("Delete Selected", this);
    connect(button_delete_selected,&QPushButton::clicked,this,&managedb::deleteItem);

    QPushButton *button_save = new QPushButton("SAVE", this);
    connect(button_save, &QPushButton::clicked, this, &managedb::save);

    QPushButton *button_close = new QPushButton("CLOSE", this);
    connect(button_close,SIGNAL(pressed()),this,SLOT(close()));

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
    glo_button->addWidget(button_selectall,7,1);
    glo_button->addWidget(button_delete_selected,7,0);
    glo_button->addWidget(button_save,8,0);
    glo_button->addWidget(button_unselect,8,1);
    glo_button->addWidget(button_close,9,0);
    glo_button->setMargin(0);
    frame_left_bottom->setLayout(glo_button);

    QSizePolicy spLeft(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spLeft.setHorizontalStretch(1);
    frame_left_bottom->setSizePolicy(spLeft);


    lo_bottom->addWidget(frame_left_bottom);

//right bottom

    table->setModel(proxy_model);
    table->verticalHeader()->hide();
    table->resizeColumnsToContents();
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::MultiSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->model()->setHeaderData(6, Qt::Horizontal,Qt::AlignLeft, Qt::TextAlignmentRole);



    QSizePolicy spRight(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spRight.setHorizontalStretch(2);

    QVBoxLayout *lo_bottom_right = new QVBoxLayout;

    QHBoxLayout *lo_label_count = new QHBoxLayout;

    count_item = new QLabel(QString::number(sql_model->rowCount()), this);

    lo_label_count->addWidget(new QLabel("Total : ", this));
    lo_label_count->addWidget(count_item);
    lo_label_count->addStretch();

    lo_bottom_right->addWidget(table);
    lo_bottom_right->addLayout(lo_label_count);
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
    showMaximized();


}

void managedb::swapTitleSinger(){

    swapItem(1,2);

}

void managedb::swapItem(int column1, int column2){


    QModelIndexList list_selected = table->selectionModel()->selectedRows();

    foreach (QModelIndex indexes, list_selected) {

        QString data1 = sql_model->index(indexes.row(),column1).data().toString();
        QString data2 = sql_model->index(indexes.row(),column2).data().toString();
        sql_model->setData(sql_model->index(indexes.row(), column1), data2);
        sql_model->setData(sql_model->index(indexes.row(), column2), data1);

    }

}

void managedb::swapSingerLanguage(){

    swapItem(2,3);

}

void managedb::swapTitleLanguage(){


    swapItem(1,3);


}
void managedb::swapSingerCategory(){
    swapItem(2,4);
}

void managedb::swapTitleCategory(){
    swapItem(1,4);

}

void managedb::swapLanguageCategory(){

    swapItem(3,4);
}

void managedb::setitem(QString text, int column){

    QModelIndexList list_selected = table->selectionModel()->selectedRows();


    foreach (QModelIndex indexes, list_selected) {

        sql_model->setData(sql_model->index(indexes.row(), column), text);

    }

}

void managedb::setTitle(){

    if(!le_set_title->text().isEmpty())
    setitem(le_set_title->text(), 1);

}
void managedb::setSinger(){

    if(!le_set_singer->text().isEmpty())
    setitem(le_set_singer->text(), 2);

}
void managedb::setLanguage(){

    if(!le_set_language->text().isEmpty())
    setitem(le_set_language->text(), 3);

}
void managedb::setCategory(){

    if(!le_set_category->text().isEmpty())
    setitem(le_set_category->text(), 4);

}


void managedb::deleteItem(){

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
        return;
    }

    foreach (QModelIndex ind, listInd) {
        table->hideRow(proxy_model->mapToSource(ind).row());
        sql_model->removeRow(proxy_model->mapToSource(ind).row());
    }

}



void managedb::save(){

    if(!sql_model->submitAll())
        qDebug()<<"error o submit"<<sql_model->lastError();
    sql_model->select();
    count_item->setText(QString::number(sql_model->rowCount()));


}

QList<QString> managedb::readListOfFile(const QString &filename){

    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"cant read singer";
//        return;
    }

    QTextStream stream(&file);

    QString line = stream.readLine();
    QList<QString>list;

    while(line!=NULL){

        list<<line;
        line=stream.readLine();
    }
        return list;


}


void managedb::onListWidgetClicked(QListWidgetItem *item){

table->clearSelection();
    QString text=item->text();

    for(int i=0; i<sql_model->rowCount();i++){
        if(sql_model->data(sql_model->index(i,2),Qt::DisplayRole).toString()==text)

    table->selectRow(i);
}



}
