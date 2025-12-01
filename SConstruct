#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["godot-extension/include","cpp/include"])
# Collect sources from both folders
sources = Glob("godot-extension/src/*.cpp")

# Filter out main*.cpp from cpp/src
cpp_sources = [
    n for n in Glob("cpp/src/*.cpp")
    if not os.path.basename(str(n)).startswith("main")
]

# Combine
sources += cpp_sources


if env["platform"] == "macos":
    library = env.SharedLibrary(
        "godot_demo/bin/softbody.isc.{}.{}.framework/libgdexample.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            "godot_demo/bin/softbody.isc.{}.{}.simulator.a".format(env["platform"], env["target"]),
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            "godot_demo/bin/softbody.isc.{}.{}.a".format(env["platform"], env["target"]),
            source=sources,
        )
else:
    library = env.SharedLibrary(
        "godot_demo/bin/softbody.isc{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
