#ifndef AQIMAGE_H
#define AQIMAGE_H

#include <QImage>
#include "boxfilter.h"

class AQImage : public QImage{
private:
    QImage buffer_image;
    int shift;
public:
    // Constructors
    AQImage();
    AQImage(const QImage& image) : QImage(image){}
    AQImage(const int& height, const int& width, QImage::Format format) : QImage(height, width, format){}

    // Setters
    void setImage(const QString&);
    void setImage(const QImage&);

    // Converters
    QImage resize(const double&);
    void drawBorder(const int&, const int&,
                    const int&, const int&);


    // Filters
    void gaussianBlur(const int&, const double&);
    void makeSmooth(const int&, const double&);


    // Functions for working with extended image
    /*
    QImage initBufferImage(const int&);
    void acceptBoxFilter(const QImage&);
    void extractResult(const QImage&);
    */
};

#endif // AQIMAGE_H
