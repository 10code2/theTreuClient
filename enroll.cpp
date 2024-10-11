#include "enroll.h"
#include "ui_enroll.h"
#include "Protocol.h"

Enroll::Enroll(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Enroll)
{
    ui->setupUi(this);
    loadConfig();   // 加载配置文件
    qDebug() << "配置文件：" << m_strIp << m_usPort;

    m_pSocket = new QTcpSocket;  // 创建Socket对象

    connect(m_pSocket, &QTcpSocket::connected, this, &Enroll::showConnect);
    connect(m_pSocket, &QTcpSocket::readyRead, this, &Enroll::recvMsg);

    // 连接服务器
    m_pSocket->connectToHost(QHostAddress(m_strIp), m_usPort);
}

void Enroll::upload()
{
    qDebug() << "准备发送数据";
    emit uploadReady();  // 发送信号
}

Enroll::~Enroll()
{
    delete ui;
}

void Enroll::loadConfig()
{
    QFile file("://client.config");
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray Ba = file.readAll();
        file.close();
        QString Str = Ba.toStdString().c_str();
        Str.replace("\r\n", " ");
        QStringList SL = Str.split(' ');

        m_strIp = SL.at(0);
        m_usPort = SL.at(1).toUShort();
    }
    else
    {
        QMessageBox::critical(this, "打开配置文件", "打开配置文件失败");
    }
}

Enroll &Enroll::getInstance()
{
    static Enroll instance;
    return instance;
}

QTcpSocket *Enroll::getTcpSocket()
{
    return m_pSocket;
}

void Enroll::showConnect()
{
    QMessageBox::information(this, "连接服务器","连接成功");
}

void Enroll::recvMsg()
{
    uint uiPDULen = m_pSocket->bytesAvailable();  // 接受信息
    m_pSocket->read((char*)&uiPDULen, sizeof (uint));
    uint uiMsgLen = uiPDULen - sizeof(PDU);
    PDU* pdu = mkPDU(uiMsgLen);
    m_pSocket->read((char*)pdu + sizeof (uint), uiPDULen - sizeof(uint));

    switch (pdu->uiMsgType) {
    case MSG_TYPE_ORDINARY_RESPOND:
    {
        QMessageBox::information(this, "提示", pdu->caData);
    }
    case MSG_TYPE_ENROLL_RESPOND:
    {
        if(0 == strcmp(pdu->caData, ENROLL_OK))
        {
            QMessageBox::information(this, "登录", "登录成功");
            this->hide();
            // 登录成功进入教师操作界面
            // opeWidget::getInstance().show();
        }
        else if(0 == strcmp(pdu->caData, ENROLL_FAILED))
        {
            QMessageBox::information(this, "登录", pdu->caData);
        }
        break;
    }
    case MSG_TYPE_CHECK_STUDENT_RESPOND:
    {  // qt如何发送信号？
        uploadReady();
    }

    default: break;
    }
    free(pdu);
    pdu = NULL;
}

void Enroll::on_pushButton_register_clicked()
{
    QString name = ui->lineEdit_name->text();
    QString pwd = ui->lineEdit_pwd->text();

    if(!name.isEmpty() && pwd.size() > 5)
    {
        PDU* pdu = mkPDU(0);
        pdu->uiMsgType = MSG_TYPE_REGIST_REQUEST;
        memcpy(pdu->caData, name.toStdString().c_str(), 32);
        memcpy(pdu->caData + 32, pwd.toStdString().c_str(), 32);
        m_pSocket->write((char*)pdu, pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
    else
    {
        QMessageBox::critical(this, "注册失败", "账号或密码不符合");
    }
}

void Enroll::on_pushButton_enroll_clicked()
{
    QString name = ui->lineEdit_name->text();
    QString pwd = ui->lineEdit_pwd->text();

    if(!name.isEmpty() && pwd.size() > 5)
    {
        PDU* pdu = mkPDU(0);
        pdu->uiMsgType = MSG_TYPE_ENROLL_REQUEST;
        memcpy(pdu->caData, name.toStdString().c_str(), 32);
        memcpy(pdu->caData + 32, pwd.toStdString().c_str(), 32);
        m_pSocket->write((char*)pdu, pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
    else
    {
        QMessageBox::critical(this, "登录失败", "账号或密码不符合");
    }
}
