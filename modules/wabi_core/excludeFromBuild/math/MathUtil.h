#pragma once

template <class T>
static bool isOrthogonal (const Eigen::Matrix<T, 3, 3>& m)
{
    // to test whether a matrix is orthogonal, mutliply the matrix by
    // it's transform and compare to identity matrix
    return (m * m.transpose()).isIdentity();
}

inline bool reOrthogonalize (Eigen::Matrix3f& m)
{
    // http://stackoverflow.com/questions/23080791/eigen-re-orthogonalization-of-rotation-matrix

    Eigen::Matrix3f mo = m;

    Eigen::Vector3f x = mo.row (0);
    Eigen::Vector3f y = mo.row (1);
    Eigen::Vector3f z = mo.row (2);

    float error = x.dot (y);

    Eigen::Vector3f x_ort = x - (error / 2) * y;
    Eigen::Vector3f y_ort = y - (error / 2) * x;
    Eigen::Vector3f z_ort = x_ort.cross (y_ort);

    mo.row (0) = x_ort.normalized();
    mo.row (1) = y_ort.normalized();
    mo.row (2) = z_ort.normalized();

    if (isOrthogonal (mo))
    {
        m = mo;
        return true;
    }
    else
    {
        return false;
    }
}

// from Instant Meshes
inline float fast_acos (float x)
{
    float negate = float (x < 0.0f);
    x = std::abs (x);
    float ret = -0.0187293f;
    ret *= x;
    ret = ret + 0.0742610f;
    ret *= x;
    ret = ret - 0.2121144f;
    ret *= x;
    ret = ret + 1.5707288f;
    ret = ret * std::sqrt (1.0f - x);
    ret = ret - 2.0f * negate * ret;
    return negate * (float)M_PI + ret;
}