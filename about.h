#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsItem>

namespace Ui {
class About;
}

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = 0);
    ~About();

private:
    Ui::About *ui;
    QGraphicsTextItem* mTextItem;
    QGraphicsScene* mScene;
};

#endif // ABOUT_H
