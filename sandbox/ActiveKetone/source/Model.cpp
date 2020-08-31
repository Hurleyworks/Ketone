#include "Model.h"

using juce::File;
using juce::String;
using juce::StringArray;
using sabi::MeshOptions;

void Model::loadModelFromIcon (const std::string& iconPath)
{
    MeshOptions options = MeshOptions::CenterVertices | MeshOptions::NormalizeSize | MeshOptions::RestOnGround | MeshOptions::LoadStrategy;

    File f (iconPath);
    String modelName = f.getFileNameWithoutExtension();

    std::string commonFolder = properties->renderProps->getVal<std::string> (RenderKey::CommonFolder);
    std::string modelFolder = commonFolder + "/models";
    StringArray files;
    String wildCard ("*.gltf;*obj");
    FileServices::getFiles (modelFolder, files, wildCard);

    for (auto path : files)
    {
        File f (path);
        String fileName = f.getFileName();
        if (fileName.startsWithIgnoreCase (modelName))
        {
            framework.io.getMessenger().send (QMS::loadMesh (path.toStdString(), options));
            break;
        }
    }
}

void Model::onDrop (const std::vector<std::string>& fileList)
{
    MeshOptions options = MeshOptions::CenterVertices | MeshOptions::NormalizeSize | MeshOptions::RestOnGround | MeshOptions::LoadStrategy;
    for (auto p : fileList)
    {
        File f (p);
        std::string path = f.getFullPathName().toStdString();
        framework.io.getMessenger().send (QMS::loadMesh (path, options));
    }
}