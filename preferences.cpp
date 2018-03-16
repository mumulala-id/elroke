#include "preferences.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QListView>
#include <QStandardItemModel>
#include <QFontComboBox>
#include <QGroupBox>
#include <QDialogButtonBox>

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
    box->addButton(tr("Save"), QDialogButtonBox::AcceptRole);
    layout_button->addWidget(box,0);


    stack = new QStackedLayout;

    QVBoxLayout *layout_background = new QVBoxLayout;

    QStandardItemModel *model = new QStandardItemModel;
    QList<QStandardItem*> listitem;

    QStandardItem *item1 = new QStandardItem;
    item1->setIcon(QIcon::fromTheme("add"));

    QStandardItem *item2 = new QStandardItem;
    item2->setIcon(QIcon::fromTheme("add"));

    listitem<<item1<<item2;

    model->appendColumn(listitem);

    QListView *view  = new QListView(this);
    view->setViewMode(QListView::IconMode);
//    view->setIconSize();
    view->setModel(model);

    layout_background->addWidget(view);

    QGroupBox *group_background = new QGroupBox(tr("Background"), this);
    group_background->setLayout(layout_background);

    QGroupBox *group_font = new QGroupBox(tr("Font"), this);

    QVBoxLayout *layout_font = new QVBoxLayout;

    QFontComboBox *combo_font = new QFontComboBox(this);
    layout_font->addWidget(combo_font,0);
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


    main_layout->addLayout(layout_button);
    main_layout->addLayout(stack);
    setLayout(main_layout);

}
