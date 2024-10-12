#ifndef OPERATE_H
#define OPERATE_H

#include <QWidget>

#include"upload.h"

namespace Ui {
class Operate;
}

class Operate : public QWidget
{
    Q_OBJECT

public:
    ~Operate();
    static Operate &getInstance();

private slots:
    void on_pushButton_upload_clicked();

private:
    explicit Operate(QWidget *parent = nullptr);

    Ui::Operate *ui;
    Upload *upWidegt;
};

#endif // OPERATE_H
