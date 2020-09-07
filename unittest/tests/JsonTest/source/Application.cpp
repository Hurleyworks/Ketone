#include "Jahley.h"
#include <json.hpp>

const std::string APP_NAME = "JsonTest";

#ifdef CHECK
#undef CHECK
#endif

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

using json = nlohmann::json;



class Application : public Jahley::App
{

  public:
	  Application(DesktopWindowSettings settings = DesktopWindowSettings(), bool windowApp = false) :
		  Jahley::App(settings, windowApp)
	{
		doctest::Context().run();
	}

  private:
	
};

Jahley::App* Jahley::CreateApplication()
{
	return new Application();
}
