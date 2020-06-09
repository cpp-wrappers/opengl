#include <vector>
#include <string>
#include <algorithm>
#include "cpp_build/clang_driver.hpp"

using namespace std;

void build(vector<string> args) {
    if(!filesystem::exists("build"))
        filesystem::create_directory("build");

    clang_driver::executor comp("clang++");
    comp.std = &clang_driver::lang::std::cxx20;
    comp.include_path("./include");
    vector<string> files {
        "buffer", "core",
        "debug", "program",
        "shader", "texture",
        "vertex_array"
    };
    comp.args.push_back("-c");

    for_each(files.begin(), files.end(), [&](string src) {
        comp.input_files = {"src/"+src+".cpp"};
        comp.output = "build/"+src+".o";

        if(comp.execute())
            std::terminate();
    });

    program_executor ar("ar", {"-rv", "build/opengl-wrapper.a"});
    for_each(files.begin(), files.end(), [&](string src) {
        ar.args.push_back("build/"+src+".o");
    });
    ar.execute();
}