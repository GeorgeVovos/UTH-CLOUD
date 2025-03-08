# UTH-CLOUD

## Prerequisites (Ubuntu)
    sudo apt install gcc   
    sudo apt install mpich

## Build-Run
    mpicc fileName.c   
    mpirun -n 4 ./a.out 


## VSCode
To remove squiggles include your mpi path in "includePath":   
.vscode/c_cpp_properties.json   
e.g

    {
        "configurations": [
            {
                "name": "Linux",
                "includePath": [
                    "${workspaceFolder}/**",
                    "/usr/include/x86_64-linux-gnu/mpich"
                ],
                "defines": [],
                "compilerPath": "/usr/bin/gcc",
                "cStandard": "c17",
                "cppStandard": "c++98",
                "intelliSenseMode": "linux-gcc-x64"
            }
        ],
        "version": 4
    }
    // If you installed openmpi , the path will probably be "/usr/lib/x86_64-linux-gnu/openmpi/include"