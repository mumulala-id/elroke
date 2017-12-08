#ifndef MANAGEDB_H
#define MANAGEDB_H
#include <QDialog>
#include <QListWidget>
#include <QLineEdit>
#include <QTableView>
#include "proxymodel.h"
#include "keyboard.h"
#include <QLabel>
#include <QSqlTableModel>
#include <QComboBox>
#include "clineedit.h"

class managedb : public QDialog
{
    Q_OBJECT
public:
    explicit managedb(QWidget *parent=0);
    ~managedb();

private :
    QListWidget *list_singer,  *list_language, *list_genre,  *list_folder;
    CLineEdit  *le_title_fixed,  *le_set_title,  *le_set_language, *le_set_singer,  *le_set_category;
    QTableView *table;
    ProxyModel *proxy_model;
    QSqlTableModel *sql_model;
    QLabel *total_count_label;
    QLabel *selected_count_label;
    QComboBox *combo_audio_channel;
    QComboBox *combo_search;
        CLineEdit *le_search;
    bool anyChange=false;
    Keyboard *kboard;

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
    void comboSearchChange(int);
    void jumpTo(QString);
    void dclose();
    void undo();
signals :
    void toSearch(QVariantList);


};

#endif // MANAGEDB_H
