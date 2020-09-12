// This source file was auto-generated by ClassMate++
// Created: 5 Sep 2020 6:49:18 pm
// Copyright (c) 2020, HurleyWorks

;
using juce::ChildProcess;
using juce::File;
using juce::StringArray;
using juce::Time;

// ctor
CudaCompiler::CudaCompiler()
{
}

// dtor
CudaCompiler::~CudaCompiler()
{
}

void CudaCompiler::compile (const PropertyService& properties)
{
    ScopedStopWatch sw (_FN_);

    String resourceFolder = properties.renderProps->getVal<std::string> (RenderKey::ResourceFolder);
    verifyPath (resourceFolder);

    String outputFolder = resourceFolder + File::getSeparatorString() + "ptx";
    File out (outputFolder);
    if (!out.exists())
    {
        out.createDirectory();
    }

    bool fileModified = false;

    // read a JSON file with last modification times
    String jsonFile = outputFolder + "/fileInfo.json";
    File js (jsonFile);
    if (js.existsAsFile())
    {
        std::ifstream i (jsonFile.toStdString());
        json j;
        i >> j;

        // size has to be even!
        jassert (j.size() % 2 == 0);

        for (int k = 0; k < j.size(); k += 2)
        {
            String path = j.at (k);
            String time = j.at (k + 1);
            fileInfo.insert (std::make_pair (path, time));
        }
    }

    String repoFolder = properties.renderProps->getVal<std::string> (RenderKey::RootFolder);
    verifyPath (repoFolder);

    String cudaFolder = repoFolder + File::getSeparatorString() + "framework/render_core/excludeFromBuild/kernels";
    verifyPath (cudaFolder);

    String thirdPartyFolder = repoFolder + File::getSeparatorString() + "thirdparty";
    verifyPath (thirdPartyFolder);

    // path to nvcc exe
    String exe ("C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.0/bin/nvcc.exe");

    StringArray cuFiles;
    String wildCard ("*.cu");
    FileServices::getFiles (cudaFolder, cuFiles, wildCard);

    for (auto path : cuFiles)
    {
        File f (path);

        Time lastModified = f.getLastModificationTime();

        auto it = fileInfo.find (path);
        if (it == fileInfo.end())
        {
            fileModified = true;
            fileInfo.insert (std::make_pair (path, lastModified.toString (true, true)));
        }
        else
        {
            // nothing to do if the file has not been modified
            if (it->second == lastModified.toString (true, true))
            {
                continue;
            }
            else
            {
                fileModified = true;
                it->second = lastModified.toString (true, true);
            }
        }

        StringArray args;
        args.add (exe);
        args.add (f.getFullPathName());
        //args.add("--verbose");
        args.add ("--ptx");
        args.add ("--use_fast_math");
        args.add ("--cudart");
        args.add ("shared");
        args.add ("--std");
        args.add ("c++17");
        args.add ("--machine");
        args.add ("64");
        args.add ("--gpu-architecture");
        args.add ("sm_75");
        args.add ("--compiler-bindir");
        args.add ("C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.27.29110/bin/Hostx64/x64");
        args.add ("--include-path");
        args.add ("C:/ProgramData/NVIDIA Corporation/OptiX SDK 7.1.0/include");
        args.add ("--include-path");
        args.add (thirdPartyFolder);
        args.add ("--include-path");
        args.add ("C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.0/include");
        args.add ("--output-file");
        args.add (outputFolder + File::getSeparatorString() + f.getFileNameWithoutExtension() + ".ptx");

        ChildProcess nvcc;
        std::stringstream os;
        if (nvcc.start (args))
            os << nvcc.readAllProcessOutput() << std::endl;
        else
            LOG (CRITICAL) << "ERROR: Could not run " << args[0];

        LOG (DBUG) << os.str();
    }

    // save the new info if a file has been modified
    if (fileModified)
    {
        json j;

        for (const auto& it : fileInfo)
        {
            j.push_back (it.first.toStdString());
            j.push_back (it.second.toStdString());
        }

        String config = outputFolder + "/fileInfo.json";
        std::ofstream o (config.toStdString());
        o << std::setw (4) << j << std::endl;
    }
}

void CudaCompiler::verifyPath (const String& path)
{
    File file (path);
    if (!file.exists())
        throw std::runtime_error ("Invalid path: " + path.toStdString());
}
