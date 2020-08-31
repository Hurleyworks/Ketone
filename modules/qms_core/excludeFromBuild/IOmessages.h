#pragma once

using sabi::LoadData;
using sabi::MeshOptions;

struct processLoadData
{
    processLoadData (const LoadData&& loadData) :
        loadData (loadData)
    {
    }
    QmsID id = QmsID::ProcessLoadData;
    LoadData loadData;
};

struct loadMesh
{
    loadMesh (const std::string& path, MeshOptions options = MeshOptions()) :
        path (path),
        options (options)
    {
    }
    QmsID id = QmsID::LoadMesh;
    std::string path;
    MeshOptions options = sabi::MeshOptions::Invalid;
};
