# tof_control
A Python-layered C++ script for the IRMA Matrix sensor. Driven through Docker on MacOS, this is a simple utility for pulling count data.

On MacOS, use the standard Linux Docker container. Use the following standard:
```
docker build -t lesa .
docker run --volume $(pwd):/code:consistent -it --name "lesa1" lesa
docker start lesa1
docker attach lesa1
```

Otherwise, run as a standard Makefile on Linux & others. 

Datastream is written to files through the Python wrapper.

