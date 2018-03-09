/*
 *
 * this code adapted from ClipGrap project
 * ClipGrab³
    Copyright (C) Philipp Schmieder
    http://clipgrab.de
    feedback [at] clipgrab [dot] de
 */

#include "youtube_downloader.h"
#include <QVBoxLayout>
#include <QDomDocument>
#include <math.h>
#include <QWebPage>
#include <QWebFrame>
#include <QWebElement>
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QHeaderView>
#include <QProgressBar>
#include <QEventLoop>

YoutubeDownloader::YoutubeDownloader(QWidget *parent)
    : QWidget(parent)
{

    QHBoxLayout *main_layout = new QHBoxLayout;
    ////////////////////// BROWSER//////////////////////////////////////////
    QVBoxLayout *layout_browser = new QVBoxLayout;

    search_edit = new QLineEdit(this);
    connect(search_edit,SIGNAL(returnPressed()),this,SLOT(on_search()));
    QPushButton *button_search  = new QPushButton(tr("Search"), this);
    connect(button_search,SIGNAL(pressed()),this,SLOT(on_search()));

    QHBoxLayout *layout_edit = new QHBoxLayout;
    layout_edit->addStretch();
    layout_edit ->addWidget(search_edit);
    layout_edit->addWidget(button_search);
    layout_edit->addStretch();

    view = new QWebView(this);

    connect(view,SIGNAL(linkClicked(QUrl)),this,SLOT(on_searchWebView_linkClicked(QUrl)));

    searchNam = new QNetworkAccessManager();

     searchReply = searchNam->get(QNetworkRequest(QUrl("https://www.youtube.com/")));
     connect(searchReply, SIGNAL(finished()), this, SLOT(processSearchReply()));

    layout_browser->addLayout(layout_edit);
    layout_browser->addWidget(view);

    //////////////////////////// download control ////////////////////////////////

    QVBoxLayout *layout_control = new QVBoxLayout;

    QHBoxLayout *layout_output_path = new QHBoxLayout;

    dir_edit = new QLineEdit(this);
    dir_edit->setText(Dir);

    QPushButton *button_get_path = new QPushButton(tr("Browse"));
    connect(button_get_path,&QPushButton::pressed,[this](){
        Dir = QFileDialog::getExistingDirectory(this, tr("Choose working directory"), QDir::homePath(), QFileDialog::ShowDirsOnly);
        if(!Dir.isEmpty()){
        dir_edit->setText(Dir);
        }
    });

    layout_output_path->addWidget(new QLabel(tr("Ouputh Path")+" :", this));
    layout_output_path->addWidget(dir_edit);
    layout_output_path->addWidget(button_get_path);

    QHBoxLayout *layout_button_addition = new QHBoxLayout;

    auto *button_add = new QPushButton(QIcon(":/usr/share/elroke/file/icon/plus.png"), "", this);
    auto *button_delete = new QPushButton(QIcon(":/usr/share/elroke/file/icon/minus.png"), "", this);

    layout_button_addition->addWidget(button_add);
    layout_button_addition->addWidget(button_delete);
    layout_button_addition->addStretch();
    layout_button_addition->setMargin(0);
    layout_button_addition->setSpacing(0);

   //table
    tview = new QTableView(this);
    tview->setSelectionBehavior(QAbstractItemView::SelectRows);
    tview->setSelectionMode(QAbstractItemView::SingleSelection);
    tview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tview->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    model = new QStandardItemModel(0, 3, tview);
    model->setHorizontalHeaderLabels(QStringList()<<tr("Title")<<tr("Link")<<tr("Status"));
    tview->setModel(model);

    connect(button_delete,&QPushButton::pressed,[this](){
       model->removeRow(tview->currentIndex().row());
    });

    process  = new QProcess(this);
    process->setProcessChannelMode(QProcess::MergedChannels);
    process->setProgram("youtube-dl");

    connect(process,&QProcess::readyReadStandardOutput,[this](){
        QString output = process->readAllStandardOutput();

        QRegExp regex("(\\d+).*%");
        regex.setMinimal(true);

        QString numb;
        int pos = 0;

        while ((pos = regex.indexIn(output, pos)) != -1)
        {
            numb = regex.cap(0).remove(QChar('%'));
            pos += regex.matchedLength();
        }

        float cur = numb.toFloat();

        emit progress((int )cur);

    });

    QHBoxLayout *layout_button = new QHBoxLayout;

    QPushButton *button_download = new QPushButton(tr("Start"), this);
    connect(button_download,SIGNAL(pressed()),this,SLOT(beginDownload()));

    QPushButton *button_cancel = new QPushButton(tr("Cancel"), this);

    layout_button->addWidget(button_cancel);
    layout_button->addWidget(button_download);

    layout_control->addLayout(layout_output_path);
    layout_control->addLayout(layout_button_addition);
    layout_control->addWidget(tview);
    layout_control->addLayout(layout_button);
    layout_control->setSpacing(0);

    main_layout->addLayout(layout_browser);
    main_layout->addLayout(layout_control);


    setLayout(main_layout);

}

void YoutubeDownloader::on_search(){
//qDebug()<<"on search";
    QString keywords = search_edit->text();
    if (!keywords.isEmpty())
    {

        if (searchReply)
    {
        searchReply->abort();
        searchReply->deleteLater();
    }
        searchReply = searchNam->get(QNetworkRequest(QUrl("https://www.youtube.com/results?search_query=" + keywords.replace(QRegExp("[&\\?%\\s]"), "+"))));
        connect(searchReply, SIGNAL(finished()), this, SLOT(processSearchReply()));
    }
    else
    {
        searchReply = searchNam->get(QNetworkRequest(QUrl("https://www.youtube.com/")));
        connect(searchReply, SIGNAL(finished()), this, SLOT(processSearchReply()));
    }

    connect(searchReply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(errorHandler(QNetworkReply::NetworkError)));

}

void YoutubeDownloader::processSearchReply(){

    QWebPage *page = new QWebPage();
    QString replies = searchReply->readAll();

    page->mainFrame()->setHtml(replies);
    page->triggerAction(QWebPage::Stop);
    page->deleteLater();

    QWebElementCollection entries = page->mainFrame()->findAllElements(".yt-lockup");

    int limit = 30;
    if (searchReply->url().toString() == "https://www.youtube.com") {
        limit = 8;
    }

    QString searchHtml;
    searchHtml.append("<style>body {margin:0;padding:0;width:100%;left:0px;top:0px;font-family:Roboto, Ubuntu, sans-serif} img{position:relative;top:-22px;left:-15px} .entry{display:block;position:relative;width:50%;float:left;height:100px;} a{color:#1a1a1a;text-decoration:none;} span.title{display:block;font-weight:bold;font-size:14px;position:absolute;left:140px;top:16px;} span.duration{display:block;font-size:11px;position:absolute;left:140px;top:70px;color:#aaa}  span.thumbnail{width:120px;height:68px;background:#00b2de;display:block;overflow:hidden;position:absolute;left:8px;top:16px;} a:hover{background: #00b2de;color:#fff}</style>");
    searchHtml.append("<body>");

    int i=0;
    foreach (QWebElement entry, entries)
    {
        QString duration = entry.findFirst(".video-time").toPlainText();
        QString thumbnail = entry.findFirst(".yt-thumb img").attribute("data-thumb");
        if (thumbnail.isEmpty())
        {
            thumbnail = entry.findFirst(".yt-thumb img").attribute("src");
        }
        if (thumbnail.startsWith("//"))
        {
            thumbnail.prepend("https:");
        }

        QString link = entry.findFirst("a.spf-link").attribute("href", "");
        if (!link.startsWith("/watch")) {
            continue;
        }

        searchHtml.append("<a href=\"https://www.youtube.com" + link + "\" class=\"entry\">");
        searchHtml.append("<span class=\"title\">" + entry.findFirst("h3 a.spf-link").toPlainText() + "</span>");
        searchHtml.append("<span class=\"duration\">" + duration + "</span>");
        searchHtml.append("<span class=\"thumbnail\"><img src=\"" + thumbnail + "\"  /></span");
        searchHtml.append("</a>");

        i++;
        if (i == limit) {
            break;
        }
    }

    searchHtml.append("</body>");
    view->setHtml(searchHtml);
    view->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

}

void YoutubeDownloader::on_searchWebView_linkClicked(QUrl _url)
{
    url = _url.toString();

    if (searchReply)
    {
        searchReply->abort();
        searchReply->deleteLater();
    }

    searchReply = searchNam->get(QNetworkRequest(url));
    connect(searchReply, &QNetworkReply::finished, [this](){
        QString reply = searchReply->readAll();
            QRegExp expression = QRegExp("<meta name=\"title\" content=\"(.*)\"");
            expression.setMinimal(true);
            if (expression.indexIn(reply) !=-1)
            {
               video_title = QString(expression.cap(1)).replace("&amp;quot;", "\"").replace("&amp;amp;", "&").replace("&#39;", "'").replace("&quot;", "\"").replace("&amp;", "&");
            }
            else
            {
                video_title = "Can't read video title";
             }

            emit titleAnalized();

    });

    QEventLoop loop;
    connect(this,SIGNAL(titleAnalized()),&loop,SLOT(quit()));
    loop.exec();


        int count = model->rowCount();
        model->setRowCount(count+1);

        QStandardItem *item_title = new QStandardItem;
        item_title->setText(video_title);
        model->setItem(count,0, item_title);

        QStandardItem *item_url  = new QStandardItem;
        item_url->setText(url);
        model->setItem(count,1,item_url);
}

void YoutubeDownloader::beginDownload(){

    for(int i =0; i<model->rowCount();i++){
        if(model->data(model->index(i,2)).toString()!="Succeed"){
            currentRow =i;
            break;
        }
    }

    connect(process,SIGNAL(finished(int)),this,SLOT(download(int)));

    progressbar = new QProgressBar;
    progressbar->setMaximum(100);
    connect(this,SIGNAL(progress(int)),progressbar,SLOT(setValue(int)));
    download(0);
}

void YoutubeDownloader::download(int exit){

    if(exit){
        QStandardItem *item_failed =new QStandardItem;
        item_failed->setText("Failed");
        item_failed->setTextAlignment(Qt::AlignCenter);
        model->setItem(currentRow-1, 2, item_failed);
    }
    else{
        //download succeed
        QStandardItem *item_succeed =new QStandardItem;
        item_succeed->setText("Succeed");
        item_succeed->setTextAlignment(Qt::AlignCenter);
        model->setItem(currentRow-1, 2, item_succeed);
    }
qDebug()<<currentRow<<model->rowCount();
    if(currentRow==model->rowCount()){

        //finish
       progressbar->hide();
       emit finished();
        //BUG, FIX ME
        return ;

    }

    tview->selectRow(currentRow);
     tview->setIndexWidget(model->index(currentRow,2), progressbar);
     QString currenUrl = model->data(model->index(currentRow,1)).toString();
     qDebug()<<currenUrl;

     QStringList arguments;
     qDebug()<<arguments;
     arguments<<"-f" << "bestvideo[ext=mp4]+bestaudio[ext=m4a]/best[ext=mp4]/best" << "-o" << Dir + "/%(title)s.%(ext)s" << currenUrl;

     process->setArguments(arguments);
     process->start();

    currentRow++;
}

void YoutubeDownloader::errorHandler(QNetworkReply::NetworkError err){

    QString error = searchReply->errorString();
qDebug()<<error;
//view->
    view->setHtml("<body>test</body>");
    view->update();
}

YoutubeDownloader::~YoutubeDownloader(){

    delete progressbar;
}
