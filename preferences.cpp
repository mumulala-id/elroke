#include "preferences.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QListView>
#include <QStandardItemModel>
#include <QFontComboBox>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QDir>
#include <QDebug>

preferences::preferences(QWidget *parent) : QDialog(parent)
{
    QHBoxLayout *main_layout = new QHBoxLayout;

    QVBoxLayout *layout_button = new QVBoxLayout;

    auto *button_background = new QPushButton(tr("Background"), this);
    auto *button_font = new QPushButton(tr("Font"), this);
    auto *button_theme = new QPushButton(tr("Theme"), this);
    auto *button_menu = new QPushButton(tr("Menu"), this);

    layout_button->addWidget(button_background);
    layout_button->addWidget(button_font);
    layout_button->addWidget(button_theme);
    layout_button->addWidget(button_menu);
    layout_button->setSpacing(0);
    layout_button->setMargin(0);
    layout_button->addStretch();

    QDialogButtonBox *box = new QDialogButtonBox(Qt::Vertical,this);
    box->addButton(tr("Cancel"), QDialogButtonBox::RejectRole);
    connect(box,SIGNAL(rejected()),this,SLOT(close()));
    box->addButton(tr("Save"), QDialogButtonBox::AcceptRole);
    layout_button->addWidget(box);


    stack = new QStackedLayout;

    QVBoxLayout *layout_background = new QVBoxLayout;

    QString path ="/usr/share/elroke/file/background";
    QDir dir(path);
    QStringList list = dir.entryList(QStringList()<<"*.jpg"<<"*.jpeg"<<"*.png");

    QStandardItemModel *model = new QStandardItemModel;
    QList<QStandardItem*> listitem;

    foreach (QString s, list) {
        QStandardItem *item = new QStandardItem;
        QPixmap pix(path+"/"+s);
        pix = pix.scaled(160,90, Qt::KeepAspectRatio, Qt::FastTransformation);
        item->setIcon(QIcon(pix));
        listitem<<item;
        }

    model->appendColumn(listitem);

    QListView *view  = new QListView(this);
    view->setViewMode(QListView::IconMode);
    view->setIconSize(QSize(320,180));
    view->setModel(model);

    auto *button_add = new QPushButton(QIcon::fromTheme("add"),"", this);

    layout_background->addWidget(view);
    layout_background->addWidget(button_add,0,Qt::AlignLeft);

    QGroupBox *group_background = new QGroupBox(tr("Background"), this);
    group_background->setLayout(layout_background);

    QGroupBox *group_font = new QGroupBox(tr("Font"), this);

    QVBoxLayout *layout_font = new QVBoxLayout;

    QFontComboBox *combo_font = new QFontComboBox(this);
    layout_font->addWidget(combo_font);

    connect(combo_font,static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::activated),[this](const QString &s_font)
    {
        selected_font = s_font;
    });

    layout_font->addStretch();
    group_font->setLayout(layout_font);

    stack->addWidget(group_background);
    stack->addWidget(group_font);

    connect(button_background,&QPushButton::pressed,[this]()
    {
        stack->setCurrentIndex(0);
    });

    connect(button_font,&QPushButton::pressed,[this]()
    {
       stack->setCurrentIndex(1);
    });

    connect(button_theme,&QPushButton::pressed,[this]()
    {
        stack->setCurrentIndex(2);
    });

    QPalette palet;
    palet.setColor(QPalette::Base, palette().dark().color());
    palet.setColor(QPalette::Window, Qt::black);
    palet.setColor(QPalette::Text, palette().light().color());
    palet.setColor(QPalette::WindowText, palette().light().color());
    palet.setColor(QPalette::Button, palette().dark().color());
    palet.setColor(QPalette::ButtonText, palette().light().color());

    setPalette(palet);

    main_layout->addLayout(layout_button);
    main_layout->addLayout(stack);
    setLayout(main_layout);

    connect(box,&QDialogButtonBox::accepted,[]()
    {


    });

}
