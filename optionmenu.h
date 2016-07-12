#ifndef OPTIONMENU_H
#define OPTIONMENU_H

#include <QDialog>
#include <QSettings>
#include <QFileDialog>
#include <QStandardPaths>

namespace Ui {
class OptionMenu;
}

class OptionMenu : public QDialog
{
    Q_OBJECT

public:
    explicit OptionMenu(QWidget *parent = 0);
    ~OptionMenu();

private slots:
    void getDefaultSaveLoc();
    void save();
    void closeWindow();

private:
    Ui::OptionMenu *ui;
    QString mDefaultSaveLoc;
};

#endif // OPTIONMENU_H
