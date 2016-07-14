/*-----------------------------------------------------------------------------
Name         optionmenu.cpp

Purpose      Dialog with user manipulated settings;

History		 11 Jul 16  AFB	Created
-----------------------------------------------------------------------------*/
#include "optionmenu.h"
#include "ui_optionmenu.h"

/*----------------------------------------------------------------------------
Name		OptionMenu

Purpose		Constructor;

History		11 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
OptionMenu::OptionMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionMenu)
{
    ui->setupUi(this);

    mDefaultSaveLoc.clear();

    connect(ui->pushButtonSetDefaultLoc
            , SIGNAL(clicked())
            , this
            , SLOT(getDefaultSaveLoc()));
    connect(ui->pushButtonSave
            , SIGNAL(clicked())
            , this
            , SLOT(save()));
    connect(ui->pushButtonCancel
            , SIGNAL(clicked())
            , this
            , SLOT(closeWindow()));

}

/*----------------------------------------------------------------------------
Name		OptionMenu

Purpose		Destructor;

History		11 Jul 16  AFB	Created
----------------------------------------------------------------------------*/
OptionMenu::~OptionMenu()
{
    delete ui;
}

/*----------------------------------------------------------------------------
Name		getDefaultSaveLoc

Purpose		Slot: when Select Default Save Location is clicked;

History		8 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
void OptionMenu::getDefaultSaveLoc()
{
    QFileDialog fileDialog;

    mDefaultSaveLoc = fileDialog.getExistingDirectory(
              this
             , tr("Choose default save directory")
             , QStandardPaths::displayName(QStandardPaths::DocumentsLocation));
}

/*----------------------------------------------------------------------------
Name		save

Purpose		Slot: when pushButtonSave is clicked;

History		8 Jun 16  AFB	Created
----------------------------------------------------------------------------*/
void OptionMenu::save()
{
    QCoreApplication::setOrganizationName("CPI");
    QCoreApplication::setApplicationName("Got");
    QSettings settings;

    // Test to see if we've received a new default location;
    if(!mDefaultSaveLoc.isEmpty())
    {
        // If so, set it;
        settings.setValue("DefaultSaveLoc", mDefaultSaveLoc);
    }

    this->close();
}

/*-----------------------------------------------------------------------------
Name         closeWindow

Purpose      Closes the dialog;

History		 11 Jul 16  AFB	Created
-----------------------------------------------------------------------------*/
void OptionMenu::closeWindow()
{
    this->close();
}
