#include "listwidget.h"

ListWidget::ListWidget(QWidget *parent) :
    QListWidget(parent){

}

void ListWidget::dragWithButton(){
    QListWidgetItem *item = currentItem();

    if(item){
       QDrag *drag = new QDrag(this);

    drag->exec((Qt::MoveAction));
    }

}
