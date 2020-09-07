#include "Jahley.h"
#include <benchmark.h>

const std::string APP_NAME = "HelloBenchmark";


static void BM_StringCreation(benchmark::State& state) {
	for (auto _ : state)
		std::string empty_string;
}
// Register the function as a benchmark
BENCHMARK(BM_StringCreation);

// Define another benchmark
static void BM_StringCopy(benchmark::State& state) {
	std::string x = "hello";
	for (auto _ : state)
		std::string copy(x);
}
BENCHMARK(BM_StringCopy);


class Application : public Jahley::App
{

  public:
	Application(DesktopWindowSettings settings = DesktopWindowSettings(), bool windowApp = false)
		: Jahley::App(settings, windowApp)
	{
		int argc = 1;
		std::vector<char*> argv;
		argv.push_back("Hello Benchmark");

		benchmark::Initialize(&argc, &argv[0]);
		benchmark::TimeUnit::kMillisecond; //  how to set this???
		benchmark::RunSpecifiedBenchmarks();
	}

  private:
	
};

Jahley::App* Jahley::CreateApplication()
{
	return new Application();
}
