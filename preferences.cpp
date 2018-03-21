#include "preferences.h"
#include "resizer.h"
#include <QHBoxLayout>
#include <QPushButton>
//#include <QListView>
//#include <QStandardItemModel>
#include <QFontComboBox>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QDebug>
#include <QFileDialog>
#include <QListWidget>
#include <QSettings>
#include <QDirIterator>
#include <QFileInfo>
#include "liststringfileparser.h"


preferences::preferences(QWidget *parent) : QDialog(parent)
{
    readSetting();
    setCursor(Qt::WaitCursor);
    QHBoxLayout *main_layout = new QHBoxLayout;

    QVBoxLayout *layout_button = new QVBoxLayout;

    auto *button_background = new QPushButton(tr("Background"), this);
    auto *button_font = new QPushButton(tr("Font"), this);
    auto *button_theme = new QPushButton(tr("Theme"), this);
    auto *button_menu = new QPushButton(tr("Menu"), this);
    auto *button_system = new QPushButton(tr("System"), this);

    layout_button->addWidget(button_background);
    layout_button->addWidget(button_font);
    layout_button->addWidget(button_theme);
    layout_button->addWidget(button_menu);
    layout_button->addWidget(button_system);
    layout_button->setSpacing(0);
    layout_button->setMargin(0);
    layout_button->addStretch();

    auto *button_ok = new QPushButton(tr("Ok"), this);
    connect(button_ok,SIGNAL(pressed()),this,SLOT(ok()));
    auto *button_apply = new QPushButton(tr("Apply"), this);
    connect(button_apply,SIGNAL(pressed()),this,SLOT(apply()));
    auto *button_cancel = new QPushButton(tr("Cancel"), this);
    connect(button_cancel,SIGNAL(pressed()),this,SLOT(close()));

    layout_button->addWidget(button_ok);
    layout_button->addWidget(button_apply);
    layout_button->addWidget(button_cancel);

    stack = new QStackedLayout;

    QVBoxLayout *layout_background = new QVBoxLayout;

    QDirIterator it("/usr/share/elroke/file/background",QStringList()<<"*.jpg"<<"*.jpeg"<<"*.png",QDir::Files, QDirIterator::NoIteratorFlags);

    QFileInfo info;

    while(it.hasNext())
    {
        info.setFile(it.next());
        bg_list<<info.absoluteFilePath();
    }

    resizer *resizer_img = new resizer();
    resizer_img->setInput(bg_list);

    thread_resizer = new QThread(this);

    resizer_img->moveToThread(thread_resizer);
    qRegisterMetaType< QList<QImage>>( "QList<QImage>" );

    connect(thread_resizer,SIGNAL(started()),resizer_img,SLOT(start()));
    connect(resizer_img,SIGNAL(finished(QList<QImage>)),this,SLOT(handleImage(QList<QImage>)));
    thread_resizer->start();

    QListView *background_view  = new QListView(this);
    background_view->setViewMode(QListView::IconMode);
    background_view->setIconSize(QSize(320,180));

    model = new QStandardItemModel;
    background_view->setModel(model);

    connect(background_view, static_cast< void (QListView::*)(const QModelIndex &)>(&QListView::clicked),[this,background_view](const QModelIndex &index)
   {
       selected_background = background_view->model()->data(index,Qt::UserRole).toString();
    });

    auto *button_add = new QPushButton(QIcon::fromTheme("add"),"", this);

    layout_background->addWidget(background_view);
    layout_background->addWidget(button_add,0,Qt::AlignLeft);
    connect(button_add,&QPushButton::pressed,[this]()
    {
         QStringList f_list =  QFileDialog::getOpenFileNames(NULL,tr("Choose image"), QDir::homePath(),tr("Image Files (*.png *.jpg *.jpeg *.PNG *.JPG *.JPEG)"));
    });

    QGroupBox *group_background = new QGroupBox(tr("Background"), this);
    group_background->setLayout(layout_background);

    QGroupBox *group_font = new QGroupBox(tr("Font"), this);

    QVBoxLayout *layout_font = new QVBoxLayout;

    QFontComboBox *combo_font = new QFontComboBox(this);

    if(!selected_font.isEmpty())
        combo_font->setCurrentText(selected_font);

    connect(combo_font,static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::activated),[this](const QString &s_font)
    {
        selected_font = s_font;
    });

    QHBoxLayout *layout_sub_font = new QHBoxLayout;
    QSpinBox *spin_font_size = new QSpinBox(this);
    spin_font_size->setRange(14,48);//not tested
    spin_font_size->setValue(font_size);

    layout_sub_font->addWidget(combo_font);
    layout_sub_font->addWidget(spin_font_size);
    layout_font->addLayout(layout_sub_font);
    layout_font->addStretch();
    group_font->setLayout(layout_font);

    connect(spin_font_size,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[this](int val)
    {
        font_size = val;
    });

    auto *group_theme = new QGroupBox(tr("Themes"), this);
    auto *group_menu = new QGroupBox(tr("Menu"), this);

    QHBoxLayout *layout_menu = new QHBoxLayout;

    QListWidget* list_menu_all = new QListWidget(this);
    QVBoxLayout *layout_move_button = new QVBoxLayout;

    auto *button_right = new QPushButton(QIcon::fromTheme("go-next"),"",this);

    auto *button_left = new QPushButton(QIcon::fromTheme("go-previous"),"", this);

    layout_move_button->addStretch();
    layout_move_button->addWidget(button_right);
    layout_move_button->addWidget(button_left);
    layout_move_button->addStretch();

    QListWidget *list_menu_selected = new QListWidget(this);
    layout_menu->addWidget(list_menu_all,0,Qt::AlignVCenter);
    layout_menu->addLayout(layout_move_button);
    layout_menu->addWidget(list_menu_selected,0,Qt::AlignVCenter);
    group_menu->setLayout(layout_menu);

    list_menu_all->addItems(getLanguageGenre());

        connect(button_right,&QPushButton::clicked,[list_menu_all,list_menu_selected]()
        {
           list_menu_selected->addItem(list_menu_all->takeItem(list_menu_all->currentRow()));
        });

        connect(button_left,&QPushButton::clicked,[list_menu_all,list_menu_selected]()
        {
           list_menu_all->addItem(list_menu_selected->takeItem(list_menu_selected->currentRow()));
        });

    auto *group_system = new QGroupBox(tr("System"),this);
    QVBoxLayout *layout_system = new QVBoxLayout;

    check_startapp  = new QCheckBox(tr("Run at startup"), this);
    layout_system->addWidget(check_startapp);
    layout_system->addStretch();
    group_system->setLayout(layout_system);



    stack->addWidget(group_background);
    stack->addWidget(group_font);
    stack->addWidget(group_theme);
    stack->addWidget(group_menu);
    stack->addWidget(group_system);

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
    connect(button_menu,&QPushButton::pressed,[this]()
    {
        stack->setCurrentIndex(3);
    });
    connect(button_system,&QPushButton::pressed,[this]()
    {
        stack->setCurrentIndex(4);
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

//    connect(box,&QDialogButtonBox::accepted,[]()
//    {


//    });

}

QStringList preferences::getLanguageGenre()
{
    QStringList list ;
    list<< listStringFileParser::parse(app_dir+"/meta/language");
    list<<listStringFileParser::parse(app_dir+"/meta/category");
    return list;
}

void preferences::apply()
{
    QSettings setting("elroke","elroke");
    setting.beginGroup("Preferences");
    setting.setValue("font",selected_font);
    setting.setValue("background", selected_background);
    setting.setValue("font_size", font_size);
    setting.endGroup();
}
void preferences::ok()
{
    apply();
    accept();
}

void preferences::readSetting()
{
    QSettings setting("elroke","elroke");
    setting.beginGroup("Preferences");
    selected_font = setting.value("font").toString();
    selected_background = setting.value("background").toString();
    font_size = setting.value("font_size").toInt();
    setting.endGroup();
}

void preferences::handleImage(QList<QImage> imglist)
{
        for(int i =0;i<imglist.count();i++)
        {

            QStandardItem *item = new QStandardItem;
            item->setIcon(QPixmap::fromImage(imglist.at(i)));
            item->setData(bg_list.at(i),Qt::UserRole);
            model->setItem(i,0,item);
        }

    setCursor(Qt::ArrowCursor);
}

preferences::~preferences()
{
    thread_resizer->quit();
    thread_resizer->wait();
}
