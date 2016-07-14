/*-----------------------------------------------------------------------------
Name         howto.h

Purpose      Dialog explaining how to run the calculation;

History		 11 Jul 16  AFB	Created
-----------------------------------------------------------------------------*/
#ifndef HOWTO_H
#define HOWTO_H

#include <QDialog>

namespace Ui {
class HowTo;
}

class HowTo : public QDialog
{
    Q_OBJECT

public:
    explicit HowTo(QWidget *parent = 0); // Constructor;

    ~HowTo(); // Destructor;

private slots:
    void closeWindow(void); // Closes the dialog;

private:
    Ui::HowTo *ui; // UI object;
};

#endif // HOWTO_H
