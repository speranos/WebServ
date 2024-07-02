# Webserv

## Introduction

This project involves creating an HTTP server in C++ 98 that complies with HTTP 1.1 standards. The server is capable of handling multiple CGI scripts, serving static websites, managing file uploads, and processing GET, POST, and DELETE requests. The project is developed with portability and robustness in mind, ensuring compatibility across different environments.

## Features

1. **HTTP 1.1 Compliant**: The server adheres to the HTTP 1.1 standard, ensuring proper communication with modern web browsers.
2. **Non-Blocking I/O**: Uses `select()` for handling I/O operations, ensuring the server remains responsive.
3. **Multiple CGI Support**: Handles multiple CGI scripts, enabling dynamic content generation.
4. **Static File Serving**: Capable of serving static HTML, CSS, JavaScript, and image files.
5. **File Uploads**: Supports file uploads via POST requests.
6. **Configuration File**: Uses a configuration file for setting server parameters like port, host, and routes.
7. **Error Handling**: Provides default error pages and accurate HTTP response status codes.
8. **Multi-Port Listening**: Can listen on multiple ports as specified in the configuration file.
9. **Stress Tested**: Ensures resilience and availability under high traffic conditions.

## Installation and Usage

### Prerequisites

- A Unix-like operating system (Linux, macOS, etc.).
- C++ compiler supporting C++ 98 standard.
- Make utility.

### Building the Project

1. Clone the repository:
   ```sh
   git clone https://github.com/speranos/WebServ.git
   cd WebServ
   ```

2. Build the project using the Makefile:
   ```sh
   make
   ```

### Running the Server

To run the server, use the following command:

```sh
./webserv webserv.conf
```

### Configuration File

The configuration file allows you to specify various server settings. Here is an example of configuration file:

```conf
server {
    listen 8080;
    server_name localhost;

    location / {
        root www;
        index index.html;
    }

    location /uploads {
        root www/uploads;
        client_max_body_size 10M;
    }

    error_page 404 /404.html;

    cgi /cgi-bin {
        extension .cgi;
    }
}
```

## Bonus Feature

The project also includes support for handling multiple CGI scripts, enhancing its capability to serve dynamic content.

## Conclusion

This web server project is a robust implementation of an HTTP server in C++ 98, capable of handling both static and dynamic content. The provided configuration file allows for flexible server setups, and the server's non-blocking I/O ensures it remains responsive under high load.

