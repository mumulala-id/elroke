#ifndef MANAGEDB_H
#define MANAGEDB_H

#include "proxymodel.h"
#include "keyboard.h"
#include "clineedit.h"
#include "dbmanager.h"
#include "videowidget.h"
#include <QDialog>
#include <QListWidget>
#include <QStandardPaths>
#include <QTableView>
#include <QLabel>
#include <QSqlTableModel>
#include <QComboBox>

class managedb : public QDialog
{
    Q_OBJECT
public:
    explicit managedb(QWidget *parent=nullptr);
    ~managedb();

private :
    QListWidget *list_singer,  *list_language, *list_genre,  *list_folder;
    CLineEdit  *le_title_fixed,  *le_set_title,  *le_set_language, *le_set_singer,  *le_set_category;
    QTableView *table;
    dbmanager *db;
    ProxyModel *proxy_model;
    QSqlTableModel *sql_model;
    QLabel *total_count_label;
    QLabel *selected_count_label;
    QComboBox *combo_audio_channel;
    QComboBox *combo_search;
    CLineEdit *le_search;
    VideoWidget *video;
    QString app_dir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    bool anyChange=false;

private slots:
    void swapItem(int, int);
    void setitem(QString, int);
    void setTitle();
    void setSinger();
    void setLanguage();
    void setCategory();
    void swapTitleSinger();
    void swapSingerLanguage();
    void swapTitleLanguage();
    void swapSingerCategory();
    void swapTitleCategory();
    void swapLanguageCategory();
    void deleteItem();
    void save();
    void selectedCount();
    void onListWidgetClicked(QListWidgetItem *);
    void updateList();
    void writeTextStream(const QString &file, QList<QString>set);
    void setAudioChannel();
    void receiverSearch(QString);
//    void comboSearchChange(int);
    void jumpTo(QString);
    void undo();
    void renameFile();
signals :
//    void toSearch(QVariantList);


};

#endif // MANAGEDB_H
