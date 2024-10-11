#ifndef OPERATE_H
#define OPERATE_H

#include <QWidget>

namespace Ui {
class Operate;
}

class Operate : public QWidget
{
    Q_OBJECT

public:
    explicit Operate(QWidget *parent = nullptr);
    ~Operate();

private slots:
    void on_pushButton_upload_clicked();

private:
    Ui::Operate *ui;
};

#endif // OPERATE_H
