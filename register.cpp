#include "register.h"
#include "ui_register.h"
#include "enroll.h"
#include "Protocol.h"

Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);

    connect(ui->radioButton, &QRadioButton::toggled, this, &Register::setNote);
    connect(ui->radioButton_2, &QRadioButton::toggled, this, &Register::setNote);
    connect(ui->pushButton_cancel, &QPushButton::clicked, this, &Register::cancel);
    connect(ui->pushButton_ok, &QPushButton::clicked, this, &Register::enroll);
}

Register::~Register()
{
    delete ui;
}

void Register::setNote()
{
    if(ui->radioButton->isChecked()){
        ui->label_note->setText("学科:");
    }
    else{
        ui->label_note->setText("班级:");
    }
}

void Register::cancel()
{
    this->close();
}

void Register::enroll()
{
    QString name = ui->lineEdit_name->text();
    QString pwd = ui->lineEdit_pwd->text();
    QString note = ui->lineEdit_note->text();
    if(name == NULL){
        QMessageBox::warning(this, "信息不全", "账号未填");
    }
    else if(pwd == NULL){
        QMessageBox::warning(this, "信息不全", "密码未填");
    }
    else if(note == NULL){
            QMessageBox::warning(this, "信息不全", ui->label_note->text());
    }
    else{  // 发送注册信息
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = MSG_TYPE_REGIST_REQUEST;
        strncpy(pdu->caData, name.toStdString().c_str(), 32);
        strncpy(pdu->caData + 32, pwd.toStdString().c_str(), 32);
        strcpy((char*)pdu->caMsg, note.toStdString().c_str());

        Enroll::getInstance().getTcpSocket()->write((char*)pdu, pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
}
