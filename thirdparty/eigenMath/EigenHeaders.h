#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/StdVector>

/* Application precision -- can be set to single or double precision */
#define SINGLE_PRECISION
#if defined(SINGLE_PRECISION)
typedef float Float;
#else
typedef double Float;
#endif

using MatrixXc = Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>;
using MatrixXf = Eigen::Matrix<Float, Eigen::Dynamic, Eigen::Dynamic>;
using MatrixXu = Eigen::Matrix<uint32_t, Eigen::Dynamic, Eigen::Dynamic>;
using Vector3u = Eigen::Matrix<uint32_t, 3, 1>;
using Matrix3f = Eigen::Matrix<Float, 3, 3>;
using Matrix43f = Eigen::Matrix<Float, 4, 3>;
using MatrixRowMajor34f = Eigen::Matrix<Float, 3, 4, Eigen::RowMajor>;
using Matrix4f = Eigen::Matrix<Float, 4, 4>;
using MatrixXu16 = Eigen::Matrix<uint16_t, Eigen::Dynamic, Eigen::Dynamic>;
using VectorXu = Eigen::Matrix<uint32_t, Eigen::Dynamic, 1>;
using VectorXb = Eigen::Matrix<bool, Eigen::Dynamic, 1>;

using Pose = Eigen::Affine3f;
using Scale = Eigen::Vector3f;
const float MIN_SCALE_F = std::numeric_limits<float>::min();
const float MIN_SCALE_D = std::numeric_limits<double>::min();

