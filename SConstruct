#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["godot-extension/include","cpp/include"])
sources = Glob("godot-extension/src/*.cpp") + Glob("cpp/src/*.cpp").exclude("cpp/src/main*.cpp")

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "godot_demo/bin/libgdexample.{}.{}.framework/libgdexample.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            "godot_demo/bin/libgdexample.{}.{}.simulator.a".format(env["platform"], env["target"]),
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            "godot_demo/bin/libgdexample.{}.{}.a".format(env["platform"], env["target"]),
            source=sources,
        )
else:
    library = env.SharedLibrary(
        "godot_demo/bin/libgdexample{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
