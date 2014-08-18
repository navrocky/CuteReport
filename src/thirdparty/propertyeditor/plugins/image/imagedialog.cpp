
#include <QFileDialog>
#include <QImageReader>
#include <QDebug>
#include "imagedialog.h"

#if QT_VERSION < 0x050000
ImageDialog::ImageDialog(QWidget* parent, Qt::WFlags fl) : QDialog(parent, fl), Ui::imageDialog()
#else
ImageDialog::ImageDialog(QWidget* parent, Qt::WindowFlags fl) : QDialog(parent, fl), Ui::imageDialog()
#endif

{
	setupUi(this);
	QGraphicsScene * scene = new QGraphicsScene(this);
	scene->clear();
	graphicsView->setScene(scene);
}

void ImageDialog::setImage(const QImage & image)
{
    m_image = image;
	graphicsView->scene()->clear();
    graphicsView->scene()->addPixmap(QPixmap::fromImage(m_image));
}

QImage ImageDialog::image()
{
    return m_image;
}

void ImageDialog::on_clearButton_clicked()
{
    m_image = QImage();
	graphicsView->scene()->clear();
    graphicsView->scene()->addPixmap(QPixmap::fromImage(m_image));
}

void ImageDialog::on_chooseButton_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    QImageReader reader(fileName);
    QImage img;
    if (reader.read(&img)) {
        m_image = img;
		graphicsView->scene()->clear();
        graphicsView->scene()->addPixmap(QPixmap::fromImage(img));
    } //else
//        qDebug() << "can't load file:" << fileName << endl << reader.errorString();
}
