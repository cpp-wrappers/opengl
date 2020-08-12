#include <vector>
#include <string>
#include <algorithm>
#include "cxx_exec/gcc_like_driver.hpp"
#include "cxx_exec/environment.hpp"
#include "clap/gnu_clap.hpp"
#include "cxx_exec/ext/build/configuration.hpp"
#include "cxx_exec/ar.hpp"

using namespace std;
using namespace filesystem;
using namespace gcc_like_driver;

void exec(vector<string> args) {
	string conf_name;
	gnu::clap clap;
	clap.option("configuration", conf_name);
	clap.parse(args.begin(), args.end());

	configuration conf = get_configuration(conf_name);

    auto cc = environment::cxx_compile_command_builder();
    cc.std(cxx20);
    cc.include("include");
    vector<string> files {
        "buffer", "core",
        "debug", "program",
        "shader", "texture",
        "vertex_array"
    };
    cc.out_type(object_file);
	
	path build_conf = "build/"+conf_name;
	path objects_p = build_conf/"objects";
	create_directories(objects_p);
	
	vector<path> objects;

    for(auto src : files) {
		path in = ("src/"+src+".cpp");
		path out = objects_p/(src+".o");

        cc.in(in);
        cc.out(out);
		conf.apply(cc);

       	environment::execute(cc);
    	cc.clear_inputs();
		objects.push_back(out);
	}
   	
	environment::execute(
   		ar::insert().to_archive(build_conf/"libopengl-wrapper.a").create_if_not_exists()
		.members(objects.begin(), objects.end())
	);
}
