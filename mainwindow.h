
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "player.h"
#include "proxymodel.h"
#include "delay.h"
#include "keyboard.h"
#include "dbmanager.h"
#include "clineedit.h"
#include <QDialog>
#include <QComboBox>
#include <QTableView>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>
#include <QDesktopWidget>
//#include <QApplication>
#include <QSqlTableModel>
//#include <QStackedLayout>


class mainWindow : public QDialog
{
    Q_OBJECT

public:

    mainWindow(QWidget *parent = 0);
    ~mainWindow();

private :
    QComboBox *combo_cat;
    QTableView *table;
    QTableView *table_playlist;
    ProxyModel *proxy_model;

    CLineEdit *le_search;
    QSlider *slider;
    QSlider *slider_pos;
    QSlider *slider_vol;
    QPushButton *button_audio_mute;
    Player *video;

    QSqlTableModel *sql_model;
     QStandardItemModel *model_playlist;

//    QStringList category;

    QString category1;
    QString category2;
    QString category3;
    QString category4;
    QString category5;
    QLabel* clock;
    QString background=":/usr/share/elroke/file/background/background.jpg";
    QDesktopWidget *desktop = QApplication::desktop();
    int desktop_width = desktop->width();
    int desktop_height = desktop->height();
    QString channel;
    QPushButton *button_lock_playlist;

    delay * m_worker;
//    QThread *m_thread ;
//    QTimer delay_timer;
    bool on_delay=false;
    Keyboard *keyboard;
    dbmanager *db;
//    dbmanager *db_playlist;

    QWidget *spacer;
    QPushButton *button_audio_channel;
    bool tampak=false;
    QAction *autosave_playlist;



private slots:
    void addToDatabase();
    void almostEnd();
    void createShortcut();
    void createWidgets();
    void errorHandling();
    void setBackground();
    void updateInterface();
    void moveItemUp();
    void moveItemDown();
    bool eventFilter(QObject *, QEvent *);
    QRegion getMaska();
    void openManagedb();
    void setVolStepDown();
    void setVolStepUp();
    void setMute();
    void setaudiochannelAuto();
    void setAudioChannelManual();
    void showClock();
    void addToPlaylist();
    void deleteItemPlaylist();
    void playPlayer();
    void clearPlaylist();
    void openAbout();
    void getCategory();
    void dialogSavePlaylist();
     void writePlaylist();
    void writePlaylist(const QString &);//write func
    void dialogLoadPlaylist();
    void loadPlaylist();//DEFAULT
    void loadPlaylist(const QString &);
     void delayHideWindow();
    void delayEvent();
    void tableRule();
    bool isKeyValid(int);
    void showKeyboard(bool);



protected :
    void keyPressEvent(QKeyEvent *event);

};

// this class hide the focus of qtableview (dotted rectangle on selected item)
class NoFocusDelegate : public QStyledItemDelegate{

protected:

            void paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const
                    {
                        QStyleOptionViewItem itemOption(option);
                        if (itemOption.state & QStyle::State_HasFocus)
                            itemOption.state = itemOption.state ^ QStyle::State_HasFocus;
                        QStyledItemDelegate::paint(painter, itemOption, index);
                    }
};

#endif // MAINWINDOW_H
