#include "aqimage.h"
#include <QImageReader>
#include <QImage>
#include <math.h>

// Constructors
AQImage::AQImage(){
    QImage _buff(1, 1, this->format());
    AQImage buff(_buff);
    *this = buff;
}


// Setters
void AQImage::setImage(const QString& filename) {

    QImageReader reader(filename);
    reader.setAutoTransform(true);
    QImage image = reader.read();

    // Copying newImage to image with RGB format (newImage cold be grayscale)
    QImage _new_image(image.width(), image.height(), QImage::Format_RGB32);
    for (int i = 0; i < _new_image.width(); ++i)
        for (int j = 0; j < _new_image.height(); ++j)
            _new_image.setPixel(i, j, image.pixel(i, j));

    AQImage new_image(_new_image);
    *this = _new_image;

}

void AQImage::setImage(const QImage& image) {
    QImage _new_image(image.width(), image.height(), QImage::Format_RGB32);
    for (int i = 0; i < _new_image.width(); ++i)
        for (int j = 0; j < _new_image.height(); ++j)
            _new_image.setPixel(i, j, image.pixel(i, j));

    AQImage new_image(_new_image);
    *this = new_image;
}


// Converters
QImage AQImage::resize(const double& coeff) {
QImage _buffer_image(this->width()*coeff, this->height()*coeff, this->format());

for (int i = 0; i < _buffer_image.width(); ++i)
    for (int j = 0; j < _buffer_image.height(); ++j){
        int avRed = 0;
        int avGreen = 0;
        int avBlue = 0;

        double a = i/coeff - floor(i/coeff);
        double b = j/coeff - floor(j/coeff);

        avRed += qRed(this->pixel(floor(i/coeff), floor(j/coeff)))*(1 - a)*(1 - b);
        avRed += qRed(this->pixel(ceil(i/coeff), floor(j/coeff)))*(1 - a)*b;
        avRed += qRed(this->pixel(floor(i/coeff), ceil(j/coeff)))*a*(1 - b);
        avRed += qRed(this->pixel(ceil(i/coeff), ceil(j/coeff)))*a*b;

        avGreen += qGreen(this->pixel(floor(i/coeff), floor(j/coeff)))*(1 - a)*(1 - b);
        avGreen += qGreen(this->pixel(ceil(i/coeff), floor(j/coeff)))*(1 - a)*b;
        avGreen += qGreen(this->pixel(floor(i/coeff), ceil(j/coeff)))*a*(1 - b);
        avGreen += qGreen(this->pixel(ceil(i/coeff), ceil(j/coeff)))*a*b;

        avBlue += qBlue(this->pixel(floor(i/coeff), floor(j/coeff)))*(1 - a)*(1 - b);
        avBlue += qBlue(this->pixel(ceil(i/coeff), floor(j/coeff)))*(1 - a)*b;
        avBlue += qBlue(this->pixel(floor(i/coeff), ceil(j/coeff)))*a*(1 - b);
        avBlue += qBlue(this->pixel(ceil(i/coeff), ceil(j/coeff)))*a*b;

        _buffer_image.setPixel(i, j, qRgb(avRed, avGreen, avBlue));

    }

    return _buffer_image;
}

void AQImage::drawBorder(const int& itx, const int& ity,
                         const int& wi, const int& he) {
    for (int a = itx; a < wi + itx; ++a){
        this->setPixel(a, ity, qRgb(255, 0, 0));
        this->setPixel(a, ity + he, qRgb(255, 0, 0));
    }

    for (int a = ity; a < he + ity; ++a){
        this->setPixel(itx, a, qRgb(255, 0, 0));
        this->setPixel(itx + wi, a, qRgb(255, 0, 0));
    }
}


// Filters
void AQImage::gaussianBlur(const int& size, const double& sigma) {
    box.set_size(size);
    shift = size / 2;

    for (int i = 0; i < box.size(); ++i)
        for (int j = 0; j < box.size(); ++j)
            box.box[i][j] = pow(M_E, ( -((i - shift)*(i - shift) + (j - shift)*(j - shift)) / (2 * sigma * sigma)) ) /
                                        (2 * M_PI * sigma * sigma);         //normal distribution formula

    buffer_image = initBufferImage(size);
    acceptBoxFilter(buffer_image);
    extractResult(buffer_image);
}

void AQImage::makeSmooth(const int& size, const double& sigma) {
    AQImage _buffer_image(this->width(), this->height(), this->format());
    _buffer_image = *this;

    gaussianBlur(size, sigma);
    for (int i = 0; i < this->width(); ++i)
        for (int j = 0; j < this->height(); ++j) {

            int gray = qGray(_buffer_image.pixel(i, j)) - qGray(this->pixel(i, j));
            if (gray > 255) gray = 255;
            if (gray < 0) gray = 0;

            this->setPixel(i, j, qRgb(gray, gray, gray));
        }
}


// Functions for working with extended image
/*
void AQImage::acceptBoxFilter(const QImage& _image) {
    QImage _buffer_image;
    _buffer_image = _image;

    for (int i = shift; i < _buffer_image.width() - shift - 1; ++i)
        for (int j = shift; j < _buffer_image.height() - shift - 1; ++j){
            double red = 0, green = 0, blue = 0;


            for (int m = 0; m < box.size(); ++m)
                for (int k = 0; k < box.size(); ++k){

                        red += qRed(_buffer_image.pixel(i + m - shift, j + k - shift))*box.box[m][k];
                        green += qGreen(_buffer_image.pixel(i + m - shift, j + k - shift))*box.box[m][k];
                        blue += qBlue(_buffer_image.pixel(i + m - shift, j + k - shift))*box.box[m][k];

                }

            _buffer_image.setPixel(i, j, qRgb(red, green, blue));
        }

    buffer_image =  _buffer_image;
}

void AQImage::extractResult(const QImage& _image) {
    for (int i = 0; i < _image.width(); ++i)
        for (int j = 0; j < _image.height(); ++j)
            this->setPixel(i, j, _image.pixel(i + shift, j + shift));
}

QImage AQImage::initBufferImage(const int& box_size) {
    box.set_size(box_size);
    shift = box.size() / 2;

    QImage _buffer_image(this->width() + 2*shift, this->height() + 2*shift, this->format());

    for (int i = 0; i < _buffer_image.width(); ++i)
        for (int j = 0; j < _buffer_image.height(); ++j) {
            if (!(i < shift || j < shift || i > _buffer_image.width() - 1 - shift || j > _buffer_image.height() - 1 - shift)) // if not edges
                _buffer_image.setPixel(i, j, this->pixel(i - shift, j - shift)); //copying image pixels
            else
                _buffer_image.setPixel(i, j, this->pixel(abs(i - pow((-1),(i < shift))*shift -
                                                                 pow((-1),(i < shift))*2),

                                                         abs(j - pow((-1),(j < shift))*shift -
                                                                 pow((-1),(j < shift))*2)));
        }
    return _buffer_image;
}
*/
