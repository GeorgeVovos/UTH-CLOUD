# UTH-CLOUD

## Prerequisites (Ububtu)
    sudo apt install gcc   
    sudo apt install mpich

## Buid-Run
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