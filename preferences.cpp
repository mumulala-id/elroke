#include "preferences.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QFontComboBox>
#include <QGroupBox>
#include <QDebug>
#include <QFileDialog>
#include <QListWidget>
#include <QSettings>
#include <QDirIterator>
#include <QFileInfo>
#include <QTextStream>
#include <QMessageBox>
#include <QApplication>
#include <QProcess>
#include <QRadioButton>
#include "liststringfileparser.h"
#include <QSlider>

preferences::preferences(QWidget *parent) : QDialog(parent)
{
    readSetting();
//    setCursor(Qt::WaitCursor);
    auto main_layout = new QHBoxLayout;

    auto layout_button = new QVBoxLayout;

    auto button_system = new QPushButton(tr("System"), this);
    auto button_font = new QPushButton(tr("Font"), this);
    auto button_theme = new QPushButton(tr("Theme"), this);
    auto button_menu = new QPushButton(tr("Favorite Group"), this);
    auto button_background = new QPushButton(tr("Background"), this);

    layout_button->addWidget(button_system);
    layout_button->addWidget(button_font);
    layout_button->addWidget(button_theme);
    layout_button->addWidget(button_menu);
    layout_button->addWidget(button_background);

    layout_button->setSpacing(0);
    layout_button->setMargin(0);
    layout_button->addStretch();

    auto button_ok = new QPushButton(tr("Save"), this);
    connect(button_ok,&QPushButton::pressed,this,&preferences::ok);

//    auto button_apply = new QPushButton(tr("Apply"), this);
//    connect(button_apply,&QPushButton::pressed,this,&preferences::apply);
    auto button_cancel = new QPushButton(tr("Cancel"), this);
    connect(button_cancel,&QPushButton::pressed,this,&preferences::close);

    layout_button->addWidget(button_ok);

//    layout_button->addWidget(button_apply);
    layout_button->addWidget(button_cancel);

   auto stack = new QStackedLayout;

    auto layout_background = new QVBoxLayout;

    QDirIterator it("/usr/share/elroke/background",QStringList()<<"*.jpg"<<"*.jpeg"<<"*.png",QDir::Files, QDirIterator::NoIteratorFlags);

    QFileInfo info;

    while(it.hasNext())
    {
        info.setFile(it.next());
        bg_list<<info.absoluteFilePath();
    }

    img_resizer = new resizer();
    img_resizer->setInputs(bg_list);

    thread_resizer = new QThread(this);

    img_resizer->moveToThread(thread_resizer);

    connect(thread_resizer,SIGNAL(started()),img_resizer,SLOT(start()));
    connect(img_resizer,&resizer::finished,this,&preferences::handleImage);
    thread_resizer->start();

    auto background_view  = new QListView(this);
    background_view->setViewMode(QListView::IconMode);
    background_view->setIconSize(QSize(320,180));

    model = new QStandardItemModel;
    background_view->setModel(model);

   QList<QStandardItem*>l = model->findItems(selected_background);
    if(!l.isEmpty()){
        background_view->setCurrentIndex(l.at(0)->index());
    }

    connect(background_view,&QListView::clicked,[this,background_view](const QModelIndex &index)
   {
       selected_background = background_view->model()->data(index,Qt::UserRole).toString();
    });

    auto button_add = new QPushButton(QIcon::fromTheme("add"),"", this);

    layout_background->addWidget(background_view);
    layout_background->addWidget(button_add,0,Qt::AlignLeft);
    connect(button_add,&QPushButton::pressed,[this]()
    {
         QStringList f_list =  QFileDialog::getOpenFileNames(nullptr,tr("Choose image"), QDir::homePath(),tr("Image Files (*.auto .auto .auto .auto .auto .JPEG)"));
    });

    auto group_system = new QGroupBox(tr("System"),this);
    auto layout_system = new QVBoxLayout;

    check_startapp  = new QCheckBox(tr("Run at startup"), this);
    check_startapp->setChecked(startup);
    layout_system->addWidget(check_startapp);

    connect(check_startapp,&QCheckBox::toggled,[this](bool d)
    {
        startup = d;
    });

    auto group_font = new QGroupBox(tr("Font"), this);

    auto layout_font = new QVBoxLayout;

    auto combo_font = new QFontComboBox(this);

    if(!selected_font.isEmpty())
    combo_font->setCurrentText(selected_font);
    connect(combo_font,static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::activated),[this](const QString &s_font)
    {
        selected_font = s_font;
    });

    auto layout_sub_font = new QHBoxLayout;

    auto spin_fontSize = new QSpinBox(this);
    spin_fontSize->setRange(12,48);//not tested
    spin_fontSize->setValue(fontSize);
//    auto spin_font_size = new QSpinBox(this);
//    spin_font_size->setRange(12,48);//not tested
//    spin_font_size->setValue(fontSize);

    layout_sub_font->addWidget(combo_font);
    layout_sub_font->addWidget(spin_fontSize);
    layout_sub_font->addStretch();
    layout_font->addLayout(layout_sub_font);
    layout_font->addStretch();
    group_font->setLayout(layout_font);

    connect(spin_fontSize,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[this](int val)
    {
        fontSize = val;
    });
///////////////////////////////////////////////THEME////////////////////////////////////////////////////////////////////////////////
    auto group_theme = new QGroupBox(tr("Themes"), this);

    auto layout_theme = new QVBoxLayout;


    auto radio_light = new QRadioButton(tr("Light"),this);
    radio_light->setChecked(true);
    theme.setLight();

    auto radio_dark = new QRadioButton(tr("Dark"),this);

    auto theme1 =new  ThemeWidget(this);
    auto theme2 = new ThemeWidget(this);
    theme2->setPrimaryColor("#FF9800");
    theme2->setSecondaryColor("#FFD180");

    auto theme3 = new ThemeWidget(this);
    theme3->setPrimaryColor("#E91E63");
    theme3->setSecondaryColor("#FF80AB");
    theme3->setButtonColor("#FF4081");
    theme3->setButtontextColor("#FFFFFF");

    auto theme4 = new ThemeWidget(this);
    theme4->setPrimaryColor("#000000");
    theme4->setSecondaryColor("#212121");
    theme4->setButtonColor("#212121");
    theme4->setButtontextColor("#FFFFFF");
//    theme4->setTextColor("#FFFFFF");
//    theme4->setBackgroundColor("#424242");

    list_theme = new QListWidget(this);
    list_theme->setFlow(QListView::LeftToRight);
    list_theme->setFixedHeight(78);
    list_theme->setSpacing(10);


    auto item = new QListWidgetItem;
    list_theme->addItem(item);
    list_theme->setItemWidget(item, theme1);
    item->setSizeHint(theme1->size());

    auto item1 = new QListWidgetItem;
    list_theme->addItem(item1);
    list_theme->setItemWidget(item1, theme2);
    item1->setSizeHint(theme2->size());

    auto item2 = new QListWidgetItem;
    list_theme->addItem(item2);
    list_theme->setItemWidget(item2, theme3);
    item2->setSizeHint(theme3->size());

    auto item3 = new QListWidgetItem;
    list_theme->addItem(item3);
    list_theme->setItemWidget(item3, theme4);
    item3->setSizeHint(theme4->size());

    auto layout_colormode = new QHBoxLayout;

    layout_colormode->addWidget(radio_light);
    layout_colormode->addWidget(radio_dark);

    auto layout_transparent = new QHBoxLayout;

    auto slider_transparent = new QSlider(Qt::Horizontal,this);
    slider_transparent->setRange(0,255);
    slider_transparent->setFocusPolicy(Qt::NoFocus);
    slider_transparent->setInvertedAppearance(true);

    connect(list_theme,&QListWidget::clicked,[this,slider_transparent](QModelIndex index){
        auto tw = qobject_cast<ThemeWidget*>(list_theme->indexWidget(index));
        slider_transparent->setValue(tw->theme().backgroundTransparency());
    });

    layout_transparent->addWidget(new QLabel(tr("Background Tranparency"), this));
    layout_transparent->addWidget(slider_transparent);



    layout_transparent->addWidget(slider_transparent);



    auto preview = new previewer(this);
    preview->setFixedSize(64,64);

    connect(radio_light,&QRadioButton::toggled,[this,preview,slider_transparent](){
     theme.setLight();
     preview->setColor(theme.backgroundColor());
     slider_transparent->setValue(theme.backgroundTransparency());
    });

    connect(radio_dark,&QRadioButton::toggled,[this,preview,slider_transparent](){
        theme.setDark();
        preview->setColor(theme.backgroundColor());
             slider_transparent->setValue(theme.backgroundTransparency());
    });
    connect(slider_transparent,&QSlider::sliderMoved,[this,preview](int value){
        QColor c = theme.backgroundColor();
        c.setAlpha(value);
        theme.setBackgroundColor(c);
        preview->setColor(c);

    });
    //when theme clicked
    connect(list_theme,&QListWidget::clicked,[this,slider_transparent,preview,radio_dark](QModelIndex index){
        auto tw = qobject_cast<ThemeWidget*>(list_theme->indexWidget(index));
        theme.setBasic(tw->theme());

    });


    layout_theme->addLayout(layout_colormode);
    layout_theme->addWidget(list_theme);
    layout_theme->addLayout(layout_transparent);
    layout_theme->addWidget(preview);

    layout_theme->addStretch();

    group_theme->setLayout(layout_theme);

    auto group_menu = new QGroupBox(tr("Favorite Group"), this);

    auto layout_menu_main = new QVBoxLayout;
    auto layout_menu = new QHBoxLayout;

    auto list_menu_all = new QListWidget(this);
    auto layout_move_button = new QVBoxLayout;

    auto button_right = new QPushButton(QIcon::fromTheme("go-next"),"",this);

    auto button_left = new QPushButton(QIcon::fromTheme("go-previous"),"", this);

    layout_move_button->addStretch();
    layout_move_button->addWidget(button_right);
    layout_move_button->addWidget(button_left);
    layout_move_button->addStretch();

    list_menu_selected = new QListWidget(this);
    list_menu_selected->addItems(favGroup);

    layout_menu->addWidget(list_menu_all,0,Qt::AlignVCenter);
    layout_menu->addLayout(layout_move_button);
    layout_menu->addWidget(list_menu_selected,0,Qt::AlignVCenter);


    layout_menu_main->addLayout(layout_menu);

    group_menu->setLayout(layout_menu_main);

    list_menu_all->addItems(getLanguageGenre());
    auto toRight = [this,list_menu_all]()
    {
       list_menu_selected->addItem(list_menu_all->takeItem(list_menu_all->currentRow()));
    };

    connect(button_right,&QPushButton::clicked,toRight);
    connect(list_menu_all,&QListWidget::doubleClicked,toRight);
    auto toLeft =[this,list_menu_all]()
    {
       list_menu_all->addItem(list_menu_selected->takeItem(list_menu_selected->currentRow()));
    };

    connect(button_left,&QPushButton::clicked,toLeft);
    connect(list_menu_selected,&QListWidget::doubleClicked,toLeft);


    auto group_background = new QGroupBox(tr("Background"), this);
    group_background->setLayout(layout_background);

    auto combo_language = new QComboBox(this);
    combo_language->addItem("English");
    combo_language->addItem("Bahasa Indonesia");
    combo_language->setCurrentIndex(language_index);
    connect(combo_language,static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[this](int index)
    {
       language_index=index;

    });

    auto layout_language = new QHBoxLayout;
    layout_language->addWidget(new QLabel(tr("Language"),this));
    layout_language->addWidget(combo_language);
    layout_system->addLayout(layout_language);

    auto layout_limit = new QHBoxLayout;
    monthRange = new QSpinBox(this);
    monthRange->setRange(1,12);
    monthRange->setValue(newEntriesLimit);

    layout_limit->addWidget(new QLabel("Month Limit New Entries", this));
    layout_limit->addWidget(monthRange);

    layout_system->addLayout(layout_limit);
    layout_system->addStretch();
    group_system->setLayout(layout_system);

    stack->addWidget(group_system);
    stack->addWidget(group_font);
    stack->addWidget(group_theme);
    stack->addWidget(group_menu);
    stack->addWidget(group_background);

    //create connection
    auto con = [stack](QPushButton* b,int x){
        connect(b,&QPushButton::pressed,[stack,x]()
        {
            stack->setCurrentIndex(x);
        });
    };
    con(button_system,0);
    con(button_font,1);
    con(button_theme,2);
    con(button_menu,3);
    con(button_background,4);


    QPalette palet;
    palet.setColor(QPalette::Base, Qt::white);
    palet.setColor(QPalette::Window, Qt::white);
    palet.setColor(QPalette::Text, QColor(0,0,0,128));
    palet.setColor(QPalette::WindowText, QColor(0,0,0,128));
    palet.setColor(QPalette::Button, palette().dark().color());
    palet.setColor(QPalette::ButtonText, Qt::white);

    setPalette(palet);

    main_layout->addLayout(layout_button);
    main_layout->addLayout(stack);
    setLayout(main_layout);

}

QStringList preferences::getLanguageGenre()
{
    QStringList list ;
    list<< listStringFileParser::parse(app_dir+"/meta/language");
    list<<listStringFileParser::parse(app_dir+"/meta/genre");

    for(int i=0;i<favGroup.size();i++)
    {
        list.removeAll(favGroup.at(i));
    }

    return list;
}

void preferences::apply()
{

    QDir dir(QDir::homePath()+"/.config/autostart");
    QFile file(dir.path()+"/elroke.desktop");
   if(startup)
   {
       if(!dir.exists())
       {
          QDir().mkdir(dir.path());
       }

       QTextStream stream(&file);
       if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
           qDebug()<<"unable open"+dir.path()+"/.config/autostart/elroke.desktop";
       stream<<
               "[Desktop Entry]\n"
               "Version=1.0\n"
               "Terminal=false\n"
               "Icon=elroke\n"
               "Type=Application\n"
                "Name=Elroke\n"
                "GenericName=Karaoke Entertainment\n"
               "Categories=Video\n"
               "Exec=elroke";
   file.close();
   }
   else
   {
       file.remove();
   }

favGroup.clear();
  for(int i=0;i<list_menu_selected->count();i++){
      favGroup<< list_menu_selected->item(i)->text();

  }


   QSettings setting("elroke","elroke");
    setting.beginGroup("Preferences");
    setting.setValue("fontName",selected_font);
    setting.setValue("background", selected_background);
    setting.setValue("fontSize", fontSize);
    setting.setValue("startup", startup);
    setting.setValue("language", language_index);
    setting.setValue("favGroup", favGroup);
    setting.setValue("monthRange", monthRange->value());
//    setting.setValue("themeMode",themeMode);
    setting.setValue("themeColor", theme.toList());

    setting.endGroup();
}
void preferences::ok()
{

    apply();
    QMessageBox msg(this);
    msg.setInformativeText(tr("All change will be applied after app restarted."));
    msg.addButton(tr("CLOSE"), QMessageBox::RejectRole);
    msg.addButton(tr("RESTART"), QMessageBox::AcceptRole);
    //RESTART
    connect(&msg,&QMessageBox::accepted,[this](){
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());

    });
    msg.setWindowFlag(Qt::FramelessWindowHint);
    msg.exec();

    accept();
}

void preferences::readSetting()
{
    QSettings setting("elroke","elroke");
    setting.beginGroup("Preferences");
    selected_font = setting.value("fontName").toString();
    selected_background = setting.value("background").toString();

    fontSize = setting.value("fontSize").toInt();
    startup = setting.value("startup").toBool();
    language_index = setting.value("language").toInt();
    favGroup = setting.value("favGroup").toStringList();
//    themeColors = setting.value("themeColor").toList();
    newEntriesLimit = setting.value("monthRange").toInt();
    setting.endGroup();
}

void preferences::handleImage(QImage img)
{
    int c =model->rowCount();
    auto item = new QStandardItem;
    item->setIcon(QPixmap::fromImage(img));
    item->setData(bg_list.at(c),Qt::UserRole);
    model->setItem(c,0,item);
}



preferences::~preferences()
{
    thread_resizer->quit();
    thread_resizer->wait();
    delete img_resizer;
}
