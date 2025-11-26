# SoftbodiesSimulation_ISC
This project aims to develop a library for the simulation of softbodies. This project is part of the semester project (303.1) for the Bachelor's degree in Computer Science and Communication Systems (Informatique et Systèmes de Communication ISC).

## Code launch instruction

From the root of your project (where the `CMakeLists.txt` file is located):

```bash
cmake -S . -B build
cmake --build build
```

Then depending of the compilator used looking for the `my_project.exe` file and run it.

```bash
.\build\Debug\my_project.exe
```

## Python visualisation

After running `main.cpp` you should have a **CSV** file. First step, you need to have the virtual environement to run the python script.

```bash
uv venv
```

The you can run the `animation.py` using;

```bash
uv run python ./visuals/animation.py
```

