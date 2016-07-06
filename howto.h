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
    explicit HowTo(QWidget *parent = 0);
    ~HowTo();

private slots:
    void closeWindow(void);

private:
    Ui::HowTo *ui;
};

#endif // HOWTO_H
