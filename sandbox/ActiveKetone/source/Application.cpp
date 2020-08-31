#include "Jahley.h"
#include "Model.h"
#include "Controller.h"
#include "View.h"

const std::string APP_NAME = "ActiveKetone";

using Eigen::Vector3f;
using sabi::PerspectiveCam;
using sabi::PixelBuffer;

class Application : public Jahley::App
{
 public:
    Application (DesktopWindowSettings settings, bool windowApp = false) :
        Jahley::App (settings, windowApp),
        model(),
        view (properties),
        controller (properties)
    {
        // store the resource folder for this project
        std::string resourceFolder = getResourcePath (APP_NAME).toStdString();
        properties.renderProps->setValue (RenderKey::ResourceFolder, resourceFolder);

        // store the resource folder shared by all projects
        std::string commonFolder = getResourcePath ("Common").toStdString();
        properties.renderProps->setValue (RenderKey::CommonFolder, commonFolder);

        // ptx files are stored in the resource folder
        properties.renderProps->setValue (RenderKey::PtxFolder, resourceFolder);

        // create the default camera
        camera = PerspectiveCam::create();
        float aspect = (float)settings.width / (float)settings.height;
        camera->setPerspective (DEFAULT_FOV_DEGREES, aspect, DEFAULT_NEAR_PLANE, DEFAULT_FAR_PLANE);
        camera->lookAt (DEFAULT_CAMERA_POSIIION, DEFAULT_CAMERA_TARGET);

        // setup the camera's float PixelBuffer
        ImageInfo spec;
        spec.dataType = ImageDataType::FLOAT_DATA;
        spec.width = DEFAULT_DESKTOP_WINDOW_WIDTH;
        spec.height = DEFAULT_DESKTOP_WINDOW_HEIGHT;
        spec.channels = 4;

        PixelBuffer& buffer = camera->getPixelBuffer();
        buffer.init (spec);

        model.initializeFramework (&properties);
    }

    ~Application() override
    {
        model.shutdown();
    }

    void onInit() override
    {
        // use Motion Blur ot not
        properties.renderProps->setValue (RenderKey::MotionBlur, false);
        model.initializeRenderEngine (camera);

        // create the gui layer
        NanoguiLayer* const gui = new NanoguiLayer (window->glfw(), properties);
        view.create (gui, APP_NAME);

        nanoguiLayer = RenderLayerRef (gui);
        pushOverlay (nanoguiLayer, true);

        // create the Optix layer
        optixLayer = std::make_shared<OptixLayer> (properties, camera);
        pushLayer (optixLayer, true);

        // View to Model connections
        connect (view, &View::emitGroundPlane, model, &Model::createGroundPlane);
        connect (view, &View::emitModelPath, model, &Model::loadModelFromIcon);
        connect (view, &View::emitClearScene, model, &Model::onClearScene);
        connect (view, &View::emitInstanceClump, model, &Model::createInstanceClump);
      
        // Controller connections
        connect (controller, &Controller::emitDeselectAll, model, &Model::onDeselectAll);
    }

    void onRender() override
    {
    }

    void update() override
    {
        model.renderNextFrame (camera);
    }

    void onDrop (const std::vector<std::string>& fileList) override
    {
        model.onDrop (fileList);
    }

    void onWindowResize (int w, int h) override
    {
        // recalcs the aspect ration now too
        camera->setScreenSize (Vector2f ((float)w, (float)h));

        // set the dirty flag now so the opengl renderer
        // does not try to access the pixelbuffer that
        // is being resized on another thread
        camera->getPixelBuffer().spec.dirty = true;
        camera->setDirty (true);
    }

    void onInput (InputEventRef& input) override
    {
        // no need to process move moves is there?
        if (input->getType() == InputEvent::Move) return;

        controller.onInput (input, camera);

        model.onInput (input);
    }

 private:
    RenderLayerRef nanoguiLayer = nullptr;
    RenderLayerRef optixLayer = nullptr;

    Model model;
    View view;
    Controller controller;
};

Jahley::App* Jahley::CreateApplication()
{
    DesktopWindowSettings settings;
    settings.name = APP_NAME;
    settings.refreshRate = 0;
    settings.resizable = false;

    return new Application (settings, true);
}
