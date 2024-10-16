#ifndef UPLOAD_H
#define UPLOAD_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QImage>
#include <QPixmap>
#include <QList>
#include <QBuffer>


namespace Ui {
class Upload;
}

class Upload : public QWidget
{
    Q_OBJECT

public:
    explicit Upload(QWidget *parent = nullptr);
    ~Upload();

    QImage convertToBinary(const QImage &image);

    void sendGray(QImage &image, QString fName);

private slots:
    void on_pushButton_chose_clicked();

    void on_pushButton_fill_clicked();

    void on_pushButton_judgment_clicked();

    void on_pushButton_ok_clicked();

    void sendExam();

private:
    Ui::Upload *ui;

    QImage choseGray, fillGray, judgmentGray;
    int count;
};

#endif // UPLOAD_H
