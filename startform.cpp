#include "startform.h"
#include "ui_startform.h"
#include <QDesktopWidget>

StartForm::StartForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartForm)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);

    QRect rect = this->frameGeometry();
    rect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(rect.topLeft());
}

StartForm::~StartForm()
{
    delete ui;
}
