#ifndef YOUTUBEDOWNLOADER_H
#define YOUTUBEDOWNLOADER_H

#include <QWidget>
#include <QLineEdit>
#include <QtWebKitWidgets/QWebView>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTableView>
#include <QStandardItemModel>
#include <QProgressBar>
#include <QProcess>
#include <QDir>

class YoutubeDownloader: public QWidget
{
    Q_OBJECT

public:
    YoutubeDownloader(QWidget *parent = nullptr);
    ~YoutubeDownloader();

private :
    QLineEdit *search_edit;
    QWebView *view;
    QNetworkAccessManager *searchNam;
    QNetworkReply *searchReply;
    QTableView *tview;
    QStandardItemModel *model;
    QLineEdit *dir_edit;
    QString Dir = QDir::homePath();
    int currentRow=0;
    QProgressBar *progressbar;
    QString url;
    QString video_title;
    QProcess *process;

private slots:

    void on_search();
    void processSearchReply();
    void create_item(QUrl url);
    void beginDownload();
    void download(int exit);
    void urlCheck(const QString &url);
    void addByUrl();

public slots:
    QString getPath()const{
        return Dir;
    }

signals :
    void finished();
    void titleAnalized();
    void progress(int);
    void urlChecked(bool);

};

#endif // YOUTUBEDOWNLOADER_H
