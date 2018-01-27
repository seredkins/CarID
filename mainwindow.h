#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <vector>

#include "aqimage.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    static const int BUTTONS_COUNT = 1;
    QLabel* draw_widget;
    QHBoxLayout *hlayout;
    QVBoxLayout *vlayout;
    QPushButton *open_button;
    QPushButton *buttons[BUTTONS_COUNT];

    AQImage image;

    double alpha;
    void updateImage();

    double SAD(const int&, const int&,
               const AQImage&);

    double SSD(const int&, const int&,
               const AQImage&);

private slots:
    void openImage();
    void detect();
};

#endif // MAINWINDOW_H
