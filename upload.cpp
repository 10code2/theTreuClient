#include "upload.h"
#include "ui_upload.h"
#include "Protocol.h"
#include "enroll.h"

Upload::Upload(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Upload)
{
    ui->setupUi(this);
    connect(&Enroll::getInstance(), &Enroll::uploadReady,
            this, &Upload::sendExam);
}

Upload::~Upload()
{
    delete ui;
}

QImage Upload::convertToBinary(const QImage &image) // 将图片转化为二值图片
{
    QImage scaledImage = image.scaled(448, 448, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QImage grayImage = scaledImage.convertToFormat(QImage::Format_Grayscale8);
    QImage binaryImage = grayImage.convertToFormat(QImage::Format_Mono);
    return binaryImage;
}




void Upload::sendGray(QImage &image, QString fName)
{
    QByteArray receBuffer; //创建接收字节流
    QBuffer bufferDevice(&receBuffer);//
    bufferDevice.open(QIODevice::WriteOnly);//打开缓冲区，写入字节流
    image.save(&bufferDevice,"png");//图像保存为指定类别

    qDebug() << "文件："<< fName << "，大小：" << receBuffer.size();
    PDU *pdu = mkPDU(receBuffer.size() + 1);  // 这里假设image不大于4k
    pdu->uiMsgType = MSG_TYPE_SEND_FILE_REQUEST;

    strcpy(pdu->caData, fName.toStdString().c_str());
    memcpy(pdu->caMsg, receBuffer, receBuffer.size());

    Enroll::getInstance().getTcpSocket()->write((char*)pdu, pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}

void Upload::on_pushButton_chose_clicked()
{
    QString chosePath = QFileDialog::getOpenFileName(this, tr("Open image"), QDir::homePath(), tr("(*.jpg)\n(*.bmp)\n(*.png)"));
    QImage image = QImage(chosePath);
    if(image.isNull()){
        QMessageBox::warning(this, "打开图片", "打开图片失败");
    }
    else{
        choseGray = convertToBinary(image);
        qDebug() << choseGray.size() << "|||" << sizeof(choseGray);
        ui->label_3->setPixmap(QPixmap::fromImage(choseGray));
    }
}

void Upload::on_pushButton_fill_clicked()
{
    QString fillPath = QFileDialog::getOpenFileName(this, tr("Open image"), QDir::homePath(), tr("(*.jpg)\n(*.bmp)\n(*.png)"));
    QImage imge = QImage(fillPath);
    if(imge.isNull()){
        QMessageBox::warning(this, "打开图片", "打开图片失败");
    }
    else{
        fillGray = convertToBinary(imge);
        ui->label_4->setPixmap(QPixmap::fromImage(fillGray));
    }
}

void Upload::on_pushButton_judgment_clicked()
{
    QString judgmentPath = QFileDialog::getOpenFileName(this, tr("Open image"), QDir::homePath(), tr("(*.jpg)\n(*.bmp)\n(*.png)"));
    QImage imge = QImage(judgmentPath);
    if(imge.isNull()){
        QMessageBox::warning(this, "打开图片", "打开图片失败");
    }
    else{
        judgmentGray = convertToBinary(imge);
        ui->label_5->setPixmap(QPixmap::fromImage(judgmentGray));
    }
}

void Upload::on_pushButton_ok_clicked()
{
    count = 0;
    QString ID = ui->lineEdit_ID->text();
    QString name = ui->lineEdit_name->text();
    if(ID.isEmpty() || name.isEmpty()){
        QMessageBox::warning(this, "信息", "信息不全");
        return;
    }
    if(choseGray.isNull() || fillGray.isNull() || judgmentGray.isNull()){
        QMessageBox::warning(this, "信息", "试卷不全");
        return;
    }

    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = MSG_TYPE_CHECK_STUDENT_REQUEST;
    strncpy(pdu->caData, ID.toStdString().c_str(), 32);
    strncpy(pdu->caData + 32, name.toStdString().c_str(), 32);
    Enroll::getInstance().getTcpSocket()->write((char*)pdu, pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}

void Upload::sendExam()
{
    if(count > 2) return;  // 因为只上传三次
    qDebug() << "开始传输图片";
    QString ID = ui->lineEdit_ID->text();
    QString name = ui->lineEdit_name->text();
    QString pre = QString("exam/%1-%2-").arg(ID).arg(name);  // 路径前缀
    QList<QImage> lis({choseGray, fillGray, judgmentGray});

    QString path = pre + QString::number(count) + ".png";
    sendGray(lis[count], path);
    count ++;
}
