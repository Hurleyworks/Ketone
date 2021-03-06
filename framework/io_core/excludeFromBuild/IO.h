// This header file was auto-generated by ClassMate++
// Created: 28 Feb 2018 8:30:14 am
// Copyright (c) 2018, HurleyWorks

#pragma  once

class IO
{

 public:
    IO() = default;
    ~IO() = default;

	void init(MessageService messengers, const PropertyService & properties);
	void tick(size_t frameNumber);
    void loadMesh (const std::string& path, MeshOptions options);

 private:
    static MessageService staticMessengers; // static because we need to call it from the thread function
	PropertyService properties;

	OIIO::thread_pool* threadPool = nullptr;

	void loadMeshFolder (const std::string& path, MeshOptions options);

	static void loadMesAsynch (int thread_id, const std::string& pathToMesh, MeshOptions options, RenderableData& data, bool loadFromScene);
    static void loadOBJ (const std::string& pathToMesh, MeshOptions options, RenderableData& data, bool loadFromScene = false);
    static void loadPLY (const std::string& pathToMesh, MeshOptions options, RenderableData& data, bool loadFromScene = false);
    static void loadGLTF (const std::string& pathToMesh, MeshOptions options, RenderableData& data, bool loadFromScene = false);

}; // end class IO

