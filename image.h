#pragma once

#include <memory>
#include <limits>
#include <QImage>
#include <QtDebug>
#include <math.h>
#include <time.h>
#include <mask.h>
using namespace std;

enum class EdgeMode
{
    ZEROS = 1,
    MIRROR = 2,
    COPY = 3
};

class Image
{
public:
    Image(int h, int w);

    static shared_ptr<Image> fromFile(const QString &fileName);
    static shared_ptr<Image> fromQImage(const QImage &picture);
    shared_ptr<Image> convolution(const Mask &mask, EdgeMode mode) const;
    shared_ptr<Image> convolution(const Mask& row, const Mask& column, EdgeMode mode) const;
    shared_ptr<Image> DownScale() const;

    QImage toQImage() const;
    bool toFile(const QString &fileName)const;

    float getPixel(int i, int j, EdgeMode mode=EdgeMode::ZEROS) const;
    float setPixel(int i, int j, float value);

    int getHeight() const;
    int getWidth() const;\

    void normalize();


private:
    const unique_ptr<float[]> image;
    const int height;
    const int width;
};

