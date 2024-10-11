#ifndef ENROLL_H
#define ENROLL_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class Enroll; }
QT_END_NAMESPACE

class Enroll : public QWidget
{
    Q_OBJECT

public:
    ~Enroll();

    void loadConfig();

    static Enroll& getInstance();

    QTcpSocket* getTcpSocket();

signals:
    void uploadReady();

private slots:
    void showConnect();

    void recvMsg();


    void on_pushButton_register_clicked();

    void on_pushButton_enroll_clicked();

private:
    Enroll(QWidget *parent = nullptr);
    void upload();

    Ui::Enroll *ui;
    QTcpSocket *m_pSocket;
    QString m_strIp;
    quint16 m_usPort;
};
#endif // ENROLL_H
