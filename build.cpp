#include <vector>
#include <string>
#include <algorithm>
#include "cpp_build/clang_driver.hpp"

using namespace std;

void build(vector<string> args) {
    clang_driver::executor comp;

    comp.name = "clang++";
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
}