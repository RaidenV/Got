#include "howto.h"
#include "ui_howto.h"

HowTo::HowTo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HowTo)
{
    ui->setupUi(this);
    connect(ui->pushButtonClose, SIGNAL(clicked()), this, SLOT(closeWindow()));
}

HowTo::~HowTo()
{
    delete ui;
}

void HowTo::closeWindow()
{
    this->close();
}
