#include "Jahley.h"

const std::string APP_NAME = "HelloWorld";

class Application : public Jahley::App
{
 public:
    Application (DesktopWindowSettings settings = DesktopWindowSettings(), bool windowApp = false) :
        Jahley::App (settings, windowApp)
    {
        LOG (DBUG) << "Hello World!";
    }

 private:
};

Jahley::App* Jahley::CreateApplication()
{
    return new Application();
}
