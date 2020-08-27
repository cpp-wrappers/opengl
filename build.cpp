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
	string gl_header;
	string before_header_def;
	gnu::clap{}
		.value("configuration", conf_name)
		.value("header", gl_header)
		.value("before-header-def", before_header_def)
		.parse(args);

	configuration conf = configuration::by_name(conf_name);

    auto cc = environment::cxx_compile_command_builder().std(cxx20).include("include");
	if(not gl_header.empty()) {
		gl_header="\\\""+gl_header+"\\\"";
		cc.definition({"GL_ALL_INCLUE_HEADER", gl_header});
	}
	if(not before_header_def.empty())
		cc.definition({before_header_def});
	cc.include("../unified-math/include");
	conf.apply(cc);
	
	path build_conf = "build/"+conf_name;
	path objects = build_conf/"objects";

	source_set{ ranges::subrange{directory_iterator{"src"}, directory_iterator{}} }
		.compile_to_objects(objects, cc)
		.to_thin_static_lib(build_conf, "opengl-wrapper");
}