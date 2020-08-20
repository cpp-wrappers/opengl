#include <vector>
#include <string>
#include "cxx_exec/gcc_like_driver.hpp"
#include "cxx_exec/environment.hpp"
#include "clap/gnu_clap.hpp"
#include "cxx_exec/build/configuration.hpp"
#include "cxx_exec/build/build.hpp"
#include <ranges>

using namespace std;
using namespace filesystem;
using namespace gcc_like_driver;

void exec(vector<string> args) {
	string conf_name;
	gnu::clap clap;
	clap.option("configuration", conf_name);
	clap.parse(args);

	configuration conf = configuration::by_name(conf_name);

    auto cc = environment::cxx_compile_command_builder().std(cxx20).include("include");
	conf.apply(cc);
	
	path build_conf = "build/"+conf_name;
	path objects = build_conf/"objects";

	sources{ ranges::subrange{directory_iterator{"src"}, directory_iterator{}} }
		.compile_to_objects(objects, cc)
		.to_thin_static_lib(build_conf, "opengl-wrapper");
}