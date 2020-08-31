#pragma once

using Eigen::Vector3f;
using Eigen::Vector2f;

struct makeGroundPlane
{
    makeGroundPlane (const Eigen::Vector2f& size = Eigen::Vector2f (1000.0f, 1000.0f)) :
        size (size)
    {
    }
    QmsID id = QmsID::MakeGroundPlane;
    ;
    Eigen::Vector2f size;
};
