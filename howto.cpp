/*-----------------------------------------------------------------------------
Name         howto.cpp

Purpose      Dialog explaining how to run the calculation;

History		 11 Jul 16  AFB	Created
-----------------------------------------------------------------------------*/
#include "howto.h"
#include "ui_howto.h"

/*-----------------------------------------------------------------------------
Name         HowTo

Purpose      Constructor;

History		 11 Jul 16  AFB	Created
-----------------------------------------------------------------------------*/
HowTo::HowTo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HowTo)
{
    ui->setupUi(this);
    connect(ui->pushButtonClose, SIGNAL(clicked()), this, SLOT(closeWindow()));
}

/*-----------------------------------------------------------------------------
Name         HowTo

Purpose      Destructor;

History		 11 Jul 16  AFB	Created
-----------------------------------------------------------------------------*/
HowTo::~HowTo()
{
    delete ui;
}

/*-----------------------------------------------------------------------------
Name         closeWindow

Purpose      Closes the dialog;

History		 11 Jul 16  AFB	Created
-----------------------------------------------------------------------------*/
void HowTo::closeWindow()
{
    this->close();
}
