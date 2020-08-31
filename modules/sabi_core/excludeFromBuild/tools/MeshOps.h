#pragma once

using Eigen::AlignedBox3f;
using sabi::MeshBuffersHandle;

struct MeshOps
{
    static void centerVertices (MeshBuffersHandle mesh, const AlignedBox3f& modelBound, float scale);
    static void normalizeSize (MeshBuffersHandle mesh, const AlignedBox3f& modelBound, float& scale);

    static void generate_smooth_normals (const MatrixXu& F, const MatrixXf& V, MatrixXf& N,
                                         bool deterministic);
};