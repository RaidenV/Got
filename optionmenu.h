/*-----------------------------------------------------------------------------
Name         optionmenu.h

Purpose      Dialog with user manipulated settings;

History		 11 Jul 16  AFB	Created
-----------------------------------------------------------------------------*/
#ifndef OPTIONMENU_H
#define OPTIONMENU_H

#include <QDialog> // ISA - QDialog;
#include <QSettings> // USES - to save global parameters;
#include <QFileDialog> // USES - to obtain default save location;
#include <QStandardPaths> // USES - to find default locations cross platform;

namespace Ui {
class OptionMenu;
}

class OptionMenu : public QDialog
{
    Q_OBJECT

public:
    explicit OptionMenu(QWidget *parent = 0); // Constructor;
    ~OptionMenu(); // Destructor;

private slots:
    void getDefaultSaveLoc(); // Uses QFileDialog to get default save location;
    void save(); // Saves all settings entered by the user;
    void closeWindow(); // Closes the dialog;

private:
    Ui::OptionMenu *ui; // UI object;
    QString mDefaultSaveLoc; // QString of the default save location;
};

#endif // OPTIONMENU_H
