# Get the GCC preinstalled image from Docker Hub
FROM gcc:latest

# Copy the current folder which contains C++ source code to the Docker image under /usr/src
#COPY . /usr/src/dockertest1

# Specify the working directory
WORKDIR /code

# add directory to image
VOLUME /code

# Use GCC to compile the Test.cpp source file
RUN apt-get update \
    && apt-get install -y libboost-all-dev

# Run the program output from the previous step
#CMD ["./SCR/tof"]
CMD ["bash"]