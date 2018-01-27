#include "mainwindow.h"
#include <QImage>
#include <QPixmap>
#include <QImageReader>
#include <QFileDialog>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <map>
#include <vector>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent) {
    draw_widget = new QLabel("", this);

    hlayout = new QHBoxLayout();
    open_button = new QPushButton("Open Image", this);
    hlayout->addWidget(open_button);

    for (int i = 0; i < BUTTONS_COUNT; ++i) {
        buttons[i] = new QPushButton("Button " + QString::number(i + 1), this);
        hlayout->addWidget(buttons[i]);
    }

    vlayout = new QVBoxLayout();
    vlayout->addWidget(draw_widget);
    vlayout->addLayout(hlayout);

    QWidget *central = new QWidget(this);
    central->setLayout(vlayout);
    setCentralWidget(central);

    setFixedSize(800, 800);

    alpha = 3; //initializing alpha for sharpness

    connect(open_button, SIGNAL(clicked(bool)), this, SLOT(openImage()));
    connect(buttons[0], SIGNAL(clicked(bool)), this, SLOT(detect()));

    buttons[0]->setText("Detect");

}

MainWindow::~MainWindow() {
    delete open_button;
    delete draw_widget;
    for (int i = 0; i < BUTTONS_COUNT; ++i)
        delete buttons[i];
    delete hlayout;
    delete vlayout;
}


void MainWindow::openImage() {
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images (*.png *.bmp *.jpg)"));
    dialog.setViewMode(QFileDialog::Detail);
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "C:/",
                                                    tr("Images (*.png *.xpm *.jpg)"));
    image.setImage(filename);
    updateImage();
}

void MainWindow::updateImage() {
    draw_widget->setPixmap(QPixmap::fromImage(image));
}


// Sum of Absolute Divisions
double MainWindow::SAD(const int& x, const int& y,
                       const AQImage& number) {
    double sad = 0;
    for (int i = x; i < number.width() + x; ++i)
        for (int j = y; j < number.height() + y; ++j)
            sad += std::abs(qGray(image.pixel(i, j)) - qGray(number.pixel(i - x, j - y)));

    sad /= (number.width() * number.height());
    return sad;
}

// Sum of Squared Divisions
double MainWindow::SSD(const int& x, const int& y,
                       const AQImage& number) {
    double ssd = 0;
    for (int i = x; i < number.width() + x; ++i)
        for (int j = y; j < number.height() + y; ++j)
            ssd += (qGray(image.pixel(i, j)) - qGray(number.pixel(i - x, j - y))) *
                   (qGray(image.pixel(i, j)) - qGray(number.pixel(i - x, j - y)));

    ssd /= (number.width() * number.height());
    return ssd;
}


void MainWindow::detect() {
    std::vector<std::pair<double, std::vector<int>>> mins;
    const QString pathToDigitsFolder = "/Users/serg/Downloads/Data/digits/";
    AQImage number;

    for (int num = 0; num <= 9; ++num) {
        double min = 6000.0;
        int itx, ity, wi, he;

        for (int pic = 1; pic <= 100; ++pic) {
            // Set path to default at every iteration
            QString path = pathToDigitsFolder;

            // Generating a path to the number image (default + folder_number + picture_number)
            path += QString::number(num); path += "/";
            path += QString::number(pic); path += ".png";
            number.setImage(path);

            // Resizing each picture
            for (int siz = 0; siz <= 30; siz += 5) {
                AQImage resized_number(number.resize(1 + (siz / 100)));

                // Putting each resized picture to the each pixel
                for (int i = 8; i < image.width() - 20; i += 3)
                    for (int j = 5; j < image.height() - 30; j += 2) {

                        // Calculating metric
                        // Metric is needed to understand is to imaes equal
                        double metric = SSD(i, j, resized_number);

                        // Finding minimum metric value for each number (0...9)
                        // Also we store position (itx, ity) and size of number (wi, he) so we can border it then
                        if (metric < min) {
                            min = metric;
                            itx = i; ity = j;
                            wi = number.width();
                            he = number.height();
                        }
                    }
            }
        }

        // Here we have minimum sad/ssd value (min) for each number also coordinates(itx, ity) where minimum starts
        // Wi and hi is width and height of resized number_image with minimum sad/ssd (for beauty border drawing)

        if (min <= 4500) {
            std::vector<int> iiwh = {itx, ity, wi, he, num};
            mins.push_back(std::pair<double, std::vector<int>>(min, iiwh));
        }
    }


    std::sort(mins.begin(), mins.end());

    // Here we have numbers in right order (like in image)
    for (int it = 0; it < 6; ++it){
        int itx = mins[it].second[0];
        int ity = mins[it].second[1];
        int wi = mins[it].second[2];
        int he = mins[it].second[3];
        image.drawBorder(itx, ity, wi, he);
    }

    updateImage();
}
