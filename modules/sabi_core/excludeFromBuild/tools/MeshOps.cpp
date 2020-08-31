#include "MeshOps.h"
/*
    normal.cpp: Helper routines for computing vertex normals

    This file is part of the implementation of

        Instant Field-Aligned Meshes
        Wenzel Jakob, Daniele Panozzo, Marco Tarini, and Olga Sorkine-Hornung
        In ACM Transactions on Graphics (Proc. SIGGRAPH Asia 2015)

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#define GRAIN_SIZE 1024
#if defined(_WIN32)
#define RCPOVERFLOW_FLT 2.93873587705571876e-39f
#define RCPOVERFLOW_DBL 5.56268464626800345e-309
#else
#define RCPOVERFLOW_FLT 0x1p-128f
#define RCPOVERFLOW_DBL 0x1p-1024
#endif

#if defined(SINGLE_PRECISION)
#define RCPOVERFLOW RCPOVERFLOW_FLT
#else
#define RCPOVERFLOW RCPOVERFLOW_DBL
#endif

using wabi::fast_acos;

void MeshOps::generate_smooth_normals (const MatrixXu& F, const MatrixXf& V, MatrixXf& N, bool deterministic)
{
    std::atomic<uint32_t> badFaces (0);
    N.resize (V.rows(), V.cols());
    N.setZero();

    auto map = [&] (const tbb::blocked_range<uint32_t>& range) {
        for (uint32_t f = range.begin(); f != range.end(); ++f)
        {
            Vector3f fn = Vector3f::Zero();
            for (int i = 0; i < 3; ++i)
            {
                Vector3f v0 = V.col (F (i, f)),
                         v1 = V.col (F ((i + 1) % 3, f)),
                         v2 = V.col (F ((i + 2) % 3, f)),
                         d0 = v1 - v0,
                         d1 = v2 - v0;

                if (i == 0)
                {
                    fn = d0.cross (d1);
                    Float norm = fn.norm();
                    if (norm < RCPOVERFLOW)
                    {
                        badFaces++; /* degenerate */
                        break;
                    }
                    fn /= norm;
                }

                /* "Computing Vertex Normals from Polygonal Facets"
                    by Grit Thuermer and Charles A. Wuethrich, JGT 1998, Vol 3 */
                Float angle = fast_acos (d0.dot (d1) / std::sqrt (d0.squaredNorm() * d1.squaredNorm()));
                for (uint32_t k = 0; k < 3; ++k)
                    atomicAdd (&N.coeffRef (k, F (i, f)), fn[k] * angle);
            }
        }
    };

    tbb::blocked_range<uint32_t> range (0u, (uint32_t)F.cols(), GRAIN_SIZE);

    if (!deterministic)
        tbb::parallel_for (range, map);
    else
        map (range);

    tbb::parallel_for (
        tbb::blocked_range<uint32_t> (0u, (uint32_t)V.cols(), GRAIN_SIZE),
        [&] (const tbb::blocked_range<uint32_t>& range) {
            for (uint32_t i = range.begin(); i != range.end(); ++i)
            {
                Float norm = N.col (i).norm();
                if (norm < RCPOVERFLOW)
                {
                    N.col (i) = Vector3f::UnitX();
                }
                else
                {
                    N.col (i) /= norm;
                }
            }
        });
}

void MeshOps::centerVertices (MeshBuffersHandle mesh, const AlignedBox3f& modelBound, float scale)
{
    int pointCount = mesh->V.cols();
    Vector3f center = modelBound.center();
    for (int i = 0; i < pointCount; i++)
    {
        Vector3f pnt = mesh->V.col (i);
        pnt -= center;
        pnt *= scale;
        mesh->V.col (i) = pnt;
    }
}

void MeshOps::normalizeSize (MeshBuffersHandle mesh, const AlignedBox3f& modelBound, float& scale)
{
    Eigen::Vector3f edges = modelBound.max() - modelBound.min();
    float maxEdge = std::max (edges.x(), std::max (edges.y(), edges.z()));
    scale = 1.0f / maxEdge; // max
}
