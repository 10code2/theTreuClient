#include "operate.h"
#include "ui_operate.h"
// #include "upload.h"

Operate::Operate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Operate)
{
    ui->setupUi(this);
}

Operate::~Operate()
{
    delete ui;
}

Operate &Operate::getInstance()
{
    static Operate instance;
    return instance;
}

void Operate::on_pushButton_upload_clicked()
{
    upWidegt = new Upload;
    upWidegt->show();
}
