
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "player.h"
#include "proxymodel.h"
#include "delay.h"
#include "keyboard.h"
#include "dbmanager.h"
#include "clineedit.h"
#include "opening.h"
#include "songitemwidget.h"
//#include "qtmaterialtextfield.h"
#include "QRoundProgressBar.h"
#include <QApplication>
#include <QDialog>
#include <QComboBox>
#include <QTableView>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QSlider>
#include <QKeyEvent>
#include <QDesktopWidget>
#include <QSqlTableModel>
#include <QPainter>
#include <QLabel>
#include <QListWidget>
class mainWindow : public QDialog
{
    Q_OBJECT

public:

    mainWindow(QWidget *parent = 0);
    ~mainWindow();

private :
    QComboBox *combo_cat;
    QTableView *table;
    ProxyModel *proxy_model;

    CLineEdit *le_search;

   QRoundProgressBar *circle_bar;
    QSlider *slider_vol;
    QPushButton *button_audio_mute;
    Player *video;
    QSqlTableModel *sql_model;

    QListWidget *playlist_widget;

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
    QString userID;
    QString password;
    QString group_auth = "Authentication";

    Player *effect_player;

    opening *cover;
    QWidget *spacer;
    QPushButton *button_audio_channel;
    bool main_window_visible=false;
    QAction *autosave_playlist;
    CLineEdit * le_userName;
    CLineEdit * le_password;
    CLineEdit *le_password_confirm;



private slots:
    void dialogAdmin();
    void dialogAddToDatabase();
    void checkAdmin();
    void createAdminAccount();
    void dialogCreateAdmin();
    void openingInstance();
    void dialogLogin();
    void login();
    void dialogNextSong();
    void createShortcut();
    void createWidgets();
    void errorHandling();
    void setBackground();
    void updateInterface();
    void moveItemUp();
    void moveItemDown();
    void moveItemToTop();
    bool eventFilter(QObject *, QEvent *);
    QRegion getMaska();
    void dialogEditDatabase();
    void setVolStepDown();
    void setVolStepUp();
    void setMute();
    void audioEffectInstance();
    void randomNumberInstance();
    void setaudiochannelAuto();
    void setAudioChannelManual();
    void showClock();
    void addToPlaylist();
    void deleteItemPlaylist();
    void playPlayer();
    void clearPlaylist();
    void dialogAbout();
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
    void videoInstance();
    void keyBoardInstance();
    void fontSetup();
    void updateLockButton(bool);
    void videoEnds();
    int getRandomNumber();
    void showHits();
    void moveItemToBottom();
    Song *extractSong();


protected :
    void keyPressEvent(QKeyEvent *event);

signals :
    void loginAccepted();
    void usernameCreated();

};

// this class hide the focus of qtableview (dotted rectangle on selected item)
class NoFocusDelegate : public QStyledItemDelegate{

protected:

            void paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const
                    {
                        QStyleOptionViewItem itemOption(option);
                        if (itemOption.state & QStyle::State_HasFocus)
                            itemOption.state = itemOption.state ^ QStyle::State_HasFocus;

                        if ((itemOption.state & QStyle::State_Selected) &&
                                (itemOption.state & QStyle::State_Active))
                                itemOption.palette.setColor(QPalette::Highlight, Qt::transparent);
                        QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &itemOption, painter, nullptr);

                        QStyledItemDelegate::paint(painter, itemOption, index);
                    }
};

#endif // MAINWINDOW_H
