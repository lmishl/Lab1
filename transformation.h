#pragma once
#include "image.h"
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include<iostream>
using namespace std;


class Transformation
{
public:
    Transformation();
    Transformation(vector<pair<KeyPoint, KeyPoint>> vec);
    float H(int i, int j) const
    {
        return h[i * 3 + j];
    }
private:
    float h[9];
};

