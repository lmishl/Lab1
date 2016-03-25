#include <QCoreApplication>
#include <stdio.h>
#include <memory>
#include <limits>
#include <iostream>
#include <QImage>
#include <QtDebug>
#include <math.h>
#include <time.h>
#include <image.h>
#include <mask.h>
#include <maskfactory.h>
#include "pyramid.h"
#include <descriptorfactory.h>
using namespace std;

shared_ptr<Image> Sobel(const Image& _im, EdgeMode _mode)
{
    MaskFactory factory;
    shared_ptr<Image> gradX = _im.convolution(factory.SobelX(),_mode);
    shared_ptr<Image> gradY = _im.convolution(factory.SobelY(),_mode);

    int width = _im.getWidth();
    int height = _im.getHeight();
    shared_ptr<Image> result = make_shared<Image>(height, width);

    for(int i=0; i<height; i++)
        for(int j=0;j<width; j++)
        {
            float xPix = gradX->getPixel(i,j,_mode);
            float yPix = gradX->getPixel(i,j,_mode);
            float G = hypot(xPix,yPix);//sqrt(xPix*xPix + yPix*yPix);
            result->setPixel(i,j,G);
        }
    return result->normalize();

}

int findClosest(Descriptor _p, vector<Descriptor> _vec)
{
    int res = 0;
    float minDist = _p.dist(_vec[0]);
    for(int i = 1; i < _vec.size(); i++)
    {
        float dist = _p.dist(_vec[i]);
        if(dist < minDist)
        {
            minDist = dist;
            res = i;
        }
    }
    return res;
}

int findClosest2(Descriptor _p, vector<Descriptor> _vec)
{

    int close1 = findClosest(_p, _vec);


    int res = -1;
    float minDist = std::numeric_limits<float>::max();

    for(int i = 0; i < _vec.size(); i++)
    {
        if(i == close1)     //ближайший пропускаем
            continue;

        float dist = _p.dist(_vec[i]);
        if(dist < minDist)
        {
            minDist = dist;
            res = i;
        }
    }
    return res;
}



int main()
{
    QString fileName1 = "C:\\4\\q1.png";
    shared_ptr<Image> myIm1 = Image::fromFile(fileName1);
    myIm1 = myIm1->GaussFilterSep(0.8, EdgeMode::COPY);
    QString fileName2 = "C:\\4\\q2.png";
    shared_ptr<Image> myIm2 = Image::fromFile(fileName2);
     myIm2 = myIm2->GaussFilterSep(0.8, EdgeMode::COPY);


    vector<KeyPoint> points1 = myIm1->Moravec(0.02, 300);//myIm1->Harris(4, 100);
    vector<KeyPoint> points2 = myIm2->Moravec(0.02, 300);//myIm2->Harris(4, 100);

    //myIm->addPoints(vec).save("C:\\1\\MoravecL.tif");

    // vector<KeyPoint> vecH = myIm->Harris(4, 300);
    //myIm->addPoints(vecH).save("C:\\1\\HarrisL.tif");
    myIm1->addPoints(points1).save("C:\\4\\Pq1.tif");
    myIm2->addPoints(points2).save("C:\\4\\Pq2.tif");

    DescriptorFactory factory1(*myIm1);
    DescriptorFactory factory2(*myIm2);

    //получаем все дескрипторы
    vector<Descriptor> descs1;
    for(int i = 0; i < points1.size(); i++)
    {
        descs1.emplace_back(*factory1.get(points1[i]));
    }

    vector<Descriptor> descs2;
    for(int i = 0; i < points2.size(); i++)
    {
        descs2.emplace_back(*factory2.get(points2[i]));
    }


    QImage unIm = myIm1->Union(*myIm2);//save("C:\\4\\Un.png");
    QPainter painter;
    painter.begin(&unIm);
    painter.setBrush(Qt::cyan);
    painter.setPen(Qt::darkCyan);
    //ищем пары
    vector <pair<KeyPoint, KeyPoint> > pairs;
    for(int i = 0; i < descs1.size(); i++)
    {
        int close1 = findClosest(descs1[i], descs2);
        int close2 = findClosest2(descs1[i], descs2);

        float dist1 = descs1[i].dist(descs2[close1]);
        float dist2 = descs1[i].dist(descs2[close2]);

        if(dist1 / dist2 > 0.8)
            continue;       //ненадёжно


        KeyPoint left = descs1[i].getPoint();
        KeyPoint right = descs2[close1].getPoint();
        //кароче рисуем
        painter.drawLine(QPoint(left.y, left.x), QPoint(right.y + myIm1->getWidth(), right.x));
       // break;

    }
    painter.end();


    unIm.save("C:\\4\\Un.png");

    cout<<"\ngood";


    return 0;
}
