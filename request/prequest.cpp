#include "prequest.hpp"

bool parseRequestLine(std::istringstream& stream, request& req) {
    std::string line;
    if (!std::getline(stream, line) || line.empty())
        return false;

    if (line.find("HTTP") == std::string::npos)
        return false;

    std::istringstream lineStream(line);
    lineStream >> req._method >> req._uri >> req._httpV;
    return true;
}

bool parseHeaders(std::istringstream& stream, request& req) {
    std::string line;
    req._headers.clear();
    while (std::getline(stream, line) && !line.empty()) {
        std::string::size_type pos = line.find(": ");
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 2);
            // Trim leading and trailing whitespace from the key and value
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            // Convert the key to lowercase for access
            std::transform(key.begin(), key.end(), key.begin(), ::tolower);
            // Handle header duplication or other custom logic
            // For example, you can choose to append multiple values for the same header key
            if (req._headers.find(key) != req._headers.end()) {
                req._headers[key] += ", " + value;
            } else {
                req._headers[key] = value;
            }
        } else
            break;
    }
    return true;
}

std::string set_extension(request req){

    std::map <std::string, std::string> head = req.getHeaders();
    std::string contentType;
    if (head.find("content-type") == head.end()) {
        contentType = ".bin";
    } else {
        contentType = head["content-type"];
        std::cout << "Content type =============================>>>"<<contentType << std::endl;
    }
    std::string extension;
    size_t pos = contentType.find('\r');
    if (pos != std::string::npos) {
        contentType = contentType.substr(0, pos);
    }
    // std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>content type: " << contentType << std::endl;
    if (contentType == "application/x-www-form-urlencoded") {
        extension = ".txt";
    } else if (contentType == "application/json") {
        extension = ".json";
    } else if (contentType == "application/xml") {
        extension = ".xml";
    } else if (contentType == "text/plain") {
        extension = ".txt";
    }else if(contentType == "text/html"){
        extension = ".html";
    }
    else if(contentType == "image/jpeg"){
        extension = ".jpeg";
    }
    else if(contentType == "image/png"){
        extension = ".png";
    }
    else if(contentType == "image/gif"){
        extension = ".gif";
    }
    else if(contentType == "image/svg+xml"){
        extension = ".svg";
    }
    else if(contentType == "image/webp"){
        extension = ".webp";
    }
    else if(contentType == "image/tiff"){
        extension = ".tiff";
    }
    else if(contentType == "image/x-icon"){
        extension = ".ico";
    }
    else if(contentType == "image/bmp"){
        extension = ".bmp";
    }
    else if(contentType == "image/vnd.microsoft.icon"){
        extension = ".ico";
    }
    else if(contentType == "image/vnd.djvu"){
        extension = ".djvu";
    }
    else if(contentType == "image/vnd.dwg"){
        extension = ".dwg";
    }
    else if(contentType == "image/vnd.dxf"){
        extension = ".dxf";
    }
    else if(contentType == "image/vnd.fastbidsheet"){
        extension = ".fbs";
    }
    else if(contentType == "image/vnd.fpx"){
        extension = ".fpx";
    }
    else if(contentType == "image/vnd.fst"){
        extension = ".fst";
    }
    else if(contentType == "image/vnd.fujixerox.edmics-mmr"){
        extension = ".mmr";
    }
    else if(contentType == "image/vnd.fujixerox.edmics-rlc"){
        extension = ".rlc";
    }
    else if(contentType == "image/vnd.ms-modi"){
        extension = ".mdi";
    }
    else if(contentType == "image/vnd.net-fpx"){
        extension = ".npx";
    }
    else if(contentType == "image/vnd.wap.wbmp"){
        extension = ".wbmp";
    }
    else if(contentType == "image/vnd.xiff"){
        extension = ".xif";
    }
    else if(contentType == "image/vnd.zbrush.pcx"){
        extension = ".pcx";
    }
    else if(contentType == "image/webp"){
        extension = ".webp";
    }
    else if(contentType == "image/x-3ds"){
        extension = ".3ds";
    }
    else if  (contentType == "video/mp4"){
        extension = ".mp4";
    }
    else {
        extension = ".bin";
    }
    // std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>extension: " << extension << std::endl;
    return extension;
}

// Function to store the request body in a file
bool storeRequestBody(std::istringstream& stream, request& req, int sck) {
    if (req.getContentLenght() == 0) {
        return true; // No body to store
    }
    // generate a unique filename for the body file depending on content type
    std::string filename;
    std::string extension = set_extension(req);
    filename = "request_body_" + std::to_string(sck) + extension;

    std::ofstream bodyFile(filename.c_str(), std::ios::out | std::ios::binary);
    if (!bodyFile) {
        std::cerr << "Error: Failed to create body file." << std::endl;
        return false;
    }
    // Read the entire body and store it in the file
    std::string buffer;
    buffer.resize(1024);
    unsigned long remainingBytes = req.getContentLenght();
    while (remainingBytes > 0) {
        unsigned long bytesToRead = std::min(remainingBytes, (unsigned long)buffer.size());
        stream.read(&buffer[0], bytesToRead);
        bodyFile.write(buffer.data(), bytesToRead);
        buffer.resize(1024);
        remainingBytes -= bytesToRead;
    }

    // Check if the entire body is successfully stored in the file
    // std::cout << "content lenght: " << req.getContentLenght() << std::endl;
    // std::cout << "body size: " << bodyFile.tellp() << std::endl;
    if ((unsigned long)bodyFile.tellp() == req.getContentLenght()) {
        std::cout << "Request body stored in file: " << filename << std::endl;
        bodyFile.close();
        req.setBody(filename); 
        // Store the filename in the request object
        return true;
    } else {
        // If the stored body length doesn't match content length, remove the incomplete file
        std::cout << "Error: Failed to store complete request body." << std::endl;
        std::remove(filename.c_str());
        return false;
    }
}

// New function to check if the request is complete based on Content-Length
bool isRequestComplete(const std::string& buffer, request& req) {
    // Check if the request headers have ended
    size_t endOfHeadersPos = buffer.find("\r\n\r\n");
    if (endOfHeadersPos == std::string::npos)
        return false;

    // If headers are complete, check if the "Content-Length" header is present
    std::string contentLengthHeader = "Content-Length: ";
    size_t contentLengthHeaderPos = buffer.find(contentLengthHeader);
    if (contentLengthHeaderPos == std::string::npos) {
        // No "Content-Length" header, headers are complete, and there is no body
        req.setContentLenght(0);
        return true;
    }

    // Find the value of the "Content-Length" header
    size_t contentLengthValuePos = contentLengthHeaderPos + contentLengthHeader.size();
    size_t endOfContentLengthPos = buffer.find("\r\n", contentLengthValuePos);
    if (endOfContentLengthPos == std::string::npos)
        return false;

    std::string contentLengthValueStr = buffer.substr(contentLengthValuePos, endOfContentLengthPos - contentLengthValuePos);
    req.setContentLenght(std::stoul(contentLengthValueStr));

    // Calculate the expected request size (headers + body)
    size_t requestSize = endOfHeadersPos + 4 /* \r\n\r\n size */ + req.getContentLenght();
    return buffer.size() >= requestSize;
}



// Main parsing function
request pRequest(std::string& buffer, client clt, int sck) {
    static std::string requestBuffer; // Use a static buffer to accumulate the request data
    request req; // Use a static request object to store the parsed data across calls

    requestBuffer += buffer;

    // Check if the request is complete
    //  && requestBuffer.find("Content-Length") != std::string::npos
    if (!isRequestComplete(requestBuffer, req)) {
        std::cout << "Incomplete request. Waiting for more data..." << std::endl;
        return req;
    } 

    std::istringstream stream(requestBuffer);
    if (!parseRequestLine(stream, req) && !req.getMethod().empty()) {
        std::cerr << "Error: Invalid request line." << std::endl;
        return req;
    }

    if (!parseHeaders(stream, req)) {
        std::cerr << "Error: Invalid request headers." <<  std::endl;
        return req;
    }
    if (req.analyzeRequest()){
        std::cout << "Request is valid" << std::endl;
        req.matchLocation(req.getUri() ,clt, sck);
    }
    
    std::map<int, Server_obj>::iterator myserver = clt.find(sck);
    req.setServerName(myserver->second.get_host());
    req.setFd(sck);

    if(req.getMethod() == "POST"){
        if (req.getHeaders().find("content-length") == req.getHeaders().end()) {
            std::cerr << "Error: Content-Length header not found." << std::endl;
            return req;
        }
        if (!storeRequestBody(stream, req, sck)) {
            return req;
        }
    }

    else if(req.getMethod() == "GET")
    {
       std::cout << "get_index=================>"<< req._loc.get_index() << std::endl;
    }
    // else if(req.getMethod() == "DELETE")
    // {
    //     //hna delete
    // }
    // else
    // {
    //     std::cerr << "Error: Invalid request method." << std::endl;
    //     return req;
    // }

    // set the server name

    // Now you have all the information in the 'req' object.
    // You can use the getter functions to access the parsed data.
    // std::cout << "Name: " << req.getServerName() << std::endl;
    // std::cout << "FD: " << req.getFd() << std::endl;
    // std::cout << "Method: " << req.getMethod() << std::endl;
    // std::cout << "URI: " << req.getUri() << std::endl;
    // std::cout << "HTTP Version: " << req.getHttpV() << std::endl;
    // std::cout << "Headers: " << std::endl;
    // std::map<std::string, std::string> headers = req.getHeaders();
    // for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it) {
    //     std::cout << it->first << ": " << it->second << std::endl;
    // }
    // std::cout << "locPath : " << req.getLocPath() << std::endl;

    // if (MethodPost(req, clt);
    // Clear the requestBuffer after processing the complete request
    requestBuffer.clear();
    // delete &req;
    return req;
}
