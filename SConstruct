#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

godot_cpp_lib = env.StaticLibrary(
    target="lib/godot-cpp",
    source=Glob("godot-cpp/src/*.cpp")
)

Default(godot_cpp_lib)

if env['platform'] == 'windows':
    env.Append(LINKFLAGS=['/EXPORT:sim_library_init'])

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=[
    "godot-extension/include",
    "cpp/include",
    "external/include"])
# Collect sources from both folders
sources = Glob("godot-extension/src/*.cpp")

# Filter out main*.cpp from cpp/src
cpp_sources = [
    n for n in Glob("cpp/src/*.cpp")
    if not os.path.basename(str(n)).startswith("main")
]

# Combine
sources += cpp_sources
output_root = "lib" if not ARGUMENTS.get("demo_only") else "godot_demo/bin"

if env["platform"] == "macos":
    library = env.SharedLibrary(
        output_root + "/softbody.isc.{}.{}.framework".format(env["platform"], env["target"]            ),
        source=sources,
    )
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            output_root + "/softbody.isc.{}.{}.simulator.a".format(env["platform"], env["target"]),
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            output_root + "/softbody.isc.{}.{}.a".format(env["platform"], env["target"]),
            source=sources,
        )
else:
    library = env.SharedLibrary(
        output_root + "/softbody.isc{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
