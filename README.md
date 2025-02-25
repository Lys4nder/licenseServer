# Content Based Image Retrieval

## Overview

The Content Based Image Retrieval project is a client-server application designed to manage and distribute software licenses. The project uses modern C++ standards and several libraries, including Qt6, gRPC, Protobuf, OpenCV, and Matplot++. The project is divided into two main components: the Client and the Server.

## Project Structure

```
content-based-image-retrieval/
├── Client/
│   ├── include/
│   │   ├── ConfigImporter.h
│   │   ├── MainWindow.h
│   │   ├── SecondaryWindow.h
│   │   └── ServerConnection.h
│   ├── proto/
│   │   └── proto.proto
│   ├── src/
│   │   ├── ConfigImporter.cc
│   │   ├── MainWindow.cc
│   │   ├── SecondaryWindow.cc
│   │   └── ServerConnection.cc
│   ├── test/
│   ├── CMakeLists.txt
│   └── main.cc
├── Server/
│   ├── include/
│   │   ├── ClientConnection.h
│   │   ├── ConfigImporter.h
│   │   ├── ImageConvertor.h
│   │   └── ImageProcessing.h
│   ├── proto/
│   │   └── proto.proto
│   ├── src/
│   │   ├── ConfigImporter.cc
│   │   ├── ImageConvertor.cc
│   │   └── ImageProcessing.cc
│   ├── ssl_files/
│   │   ├── generate_ssl.sh
│   │   └── openssl.cnf
│   ├── test/
│   ├── CMakeLists.txt
│   └── main.cc
├── CMakeLists.txt
└── README.md
```

## Dependencies

The project relies on the following dependencies:

- CMake 3.31 or higher
- Qt6 (Widgets, Core, Gui)
- Protobuf
- gRPC
- OpenCV
- Matplot++
- GoogleTest (for testing)

## Building the Project

### Prerequisites

Ensure you have the necessary dependencies installed on your system. You can install them using your package manager or by following the instructions on their respective websites.

### Build Instructions

1. **Clone the Repository:**

   ```sh
   git clone https://github.com/Lys4nder/content-based-image-retrieval
   cd content-based-image-retrieval
   ```

2. **Create a Build Directory:**

   ```sh
   mkdir build
   cd build
   ```

3. **Run CMake:**

   ```sh
   cmake ..
   ```

4. **Build the Project:**

   ```sh
   cmake --build .
   ```

## Running the Project

### Generating SSL Certificates

Before running the server, you need to generate SSL certificates. Navigate to the `Server/ssl_files` directory and run the `generate_ssl.sh` script:

```sh
cd ../Server/ssl_files
bash generate_ssl.sh
```

### Running the Server

Navigate to the build directory and run the server executable:

```sh
cd ../build/Server
./ServerAppExecutable
```

### Running the Client

Navigate to the build directory and run the client executable:

```sh
cd ../build/Client
./ClientAppExecutable
```

## Testing

The project includes unit tests using GoogleTest. To run the tests, navigate to the build directory and execute the test binaries:

```sh
cd ../build/Client/test
cd ../build/Server/test
test
```

## License

This project is licensed under the MIT License.