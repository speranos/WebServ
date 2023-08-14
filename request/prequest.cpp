#include "prequest.hpp"

bool parseRequestLine(std::istringstream& stream, request& req) {
    std::string line;
    if (!req.getMethod().empty())
        return true;
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
            // std::transform(key.begin(), key.end(), key.begin(), ::tolower);
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
    // std::ofstream bodyFile;

    std::string filename;
    // int tmp = req.getContentLenght();
    // generate a unique filename for the body file depending on content type
    // std::cout << "storing body : " << req.getHttpV() << std::endl;
    std::cout << "***************** here *****************" << filename << std::endl;
    if(req.getBody().empty()){
        std::string extension = set_extension(req);
        std::ostringstream rnd_num;
        rnd_num << rand();
        std::ostringstream str_sck;
        str_sck << sck;
        filename = "request_body_" + rnd_num.str() + "_" + str_sck.str() + extension;
        req.setBody(filename);
    } else {
        filename = req.getBody();
    }

    std::ofstream bodyFile(filename.c_str(), std::ios::out | std::ios::binary | std::ios::app);
    if (!bodyFile) {
        std::cerr << "Error: Failed to create body file." << std::endl;
        return false;
    }

    // std::cout << "storing body : " << filename << std::endl;
        std::string buffer;
        int bytes = 0;
        std::string bff;
        bff.resize(1024);
        bff = stream.str();
        if (bff.find("\r\n\r\n") != std::string::npos) {
            bff = bff.substr(bff.find("\r\n\r\n") + 4);
        }
        bytes = bff.length();
        std::cout << "*******    bytes: " << bytes << std::endl;
        bodyFile.write(bff.data(), bytes);
        bff.clear();
    // }

    // Check if the entire body is successfully stored in the file

    std::cout << "buffer : " << bff << std::endl;
    std::cout << "body size: " << bodyFile.tellp() << std::endl;
    std::cout << "content len: " << req.getContentLenght() << std::endl;
    if (req.getContentLenght() == (unsigned long)bodyFile.tellp()) {
        std::cout << "Request body stored in file: " << filename << std::endl;
        // std::string ext = set_extension(req);
        // filename = filename.substr(0, filename.find(".bin")) + ext;
        bodyFile.close();
        req.setBody(filename);
        req.setIsDone(true);
        return false;
        // Store the filename in the request object
    }
    return true;
}
    // else {
    //     // If the stored body length doesn't match content length, remove the incomplete file
    //     // std::cout << "Error: Failed to store complete request body." << std::endl;
    //     // std::remove(filename.c_str());
    //     return false;
    // }
    // if (stream) {
    //     size_t bytesRead = static_cast<size_t>(stream.tellg());  // Get the actual number of bytes read
    //     std::cout << "** bytes read: " << bytesRead <<std::endl;  
    //     stream.read(&buffer[0], bytesRead);  // Read data into the buffer
    //     if (bytesRead > 0) {
    //         bodyFile.write(buffer.data(), bytesRead);  // Write the data to the file
    //     }
    // }
    // char ch;
    // int i = 0;
    // while (stream.get(ch) && i++) {
    //     bodyFile.put(ch);
    // }
    // std::cout << "*** stream lenght: " << i << std::endl;
    // std::cout << "*** new content lenght: " << req.getContentLenght() << std::endl;
    // unsigned long maxBytes = 1024;
    // // while (remainingBytes > 0) {
    //     std::cout << "buffer size: " << buffer.size() << std::endl;
        // unsigned long bytesToRead = std::min(maxBytes, (unsigned long)buffer.size());
        // std::cout << "bytes to read: " << bytesToRead << std::endl;
        // stream.read(&buffer[0], bytesToRead);
        // bodyFile.write(buffer.data(), bytesToRead);
        // buffer.resize(1024);
        // remainingBytes -= bytesToRead;
    // }

// New function to check if the request is complete based on Content-Length
// bool isRequestComplete(const std::string& buffer, request& req) {
//     // Check if the request headers have ended
//     size_t endOfHeadersPos = buffer.find("\r\n\r\n");
//     if (endOfHeadersPos == std::string::npos)
//         return false;

//     // If headers are complete, check if the "Content-Length" header is present
//     std::string contentLengthHeader = "Content-Length: ";
//     size_t contentLengthHeaderPos = buffer.find(contentLengthHeader);
//     if (contentLengthHeaderPos == std::string::npos) {
//         // No "Content-Length" header, headers are complete, and there is no body
//         req.setContentLenght(0);
//         return true;
//     }

//     // Find the value of the "Content-Length" header
//     size_t contentLengthValuePos = contentLengthHeaderPos + contentLengthHeader.size();
//     size_t endOfContentLengthPos = buffer.find("\r\n", contentLengthValuePos);
//     if (endOfContentLengthPos == std::string::npos)
//         return false;

//     std::string contentLengthValueStr = buffer.substr(contentLengthValuePos, endOfContentLengthPos - contentLengthValuePos);
//     req.setContentLenght(std::stoul(contentLengthValueStr));

//     // Calculate the expected request size (headers + body)
//     size_t requestSize = endOfHeadersPos + 4 /* \r\n\r\n size */ + req.getContentLenght();
//     return buffer.size() >= requestSize;
// }

// std::map<int, request> requests;

// Main parsing function
request pRequest(std::string& buffer, client_config clt, int sck) {
    request req;

    req.setIsDone(false);
    std::cout << "**************starting parsing**************" << std::endl;
    std::cout << "parsing request ****** sck" << sck << std::endl;
    std::cout << "parsing request ****** sck" << sck << std::endl;
    std::istringstream stream(buffer);
    if (!parseRequestLine(stream, req) && !req.getMethod().empty()) {
        std::cerr << "Error: Invalid request line." << std::endl;
        return req;
    }

        if (!parseHeaders(stream, req)) {
            std::cerr << "Error: Invalid request headers." <<  std::endl;
            return req;
        }
        std::map<std::string, std::string>::iterator it = req._headers.find("content-length");

        // if (it != req.getHeaders().end() && req.getMethod() == "POST"){
            unsigned long len = strtoul(it->second.c_str(), NULL, 10);
            std::cout << "content lenght: " << len << std::endl;
            req.setContentLenght(len); 
            storeRequestBody(stream, req, sck);
        // }
        // if (req.getMethod() != "POST" || req.getIsDone() == true){
        // if (req.analyzeRequest()){
            std::cout << "Method: " << req.getMethod() << std::endl;
            std::cout << "URI: " << req.getUri() << std::endl;
            std::cout << "Request is valid" << std::endl;
            req.analyzeRequest();
            req.matchLocation(req.getUri() ,clt, sck);
        // }
        // }
        std::map<int, Server_obj>::iterator myserver = clt.find(sck);
        req.setServerName(myserver->second.get_host());
        req.setFd(sck);
    // }
    // std::cout << "parsing request ****** " << client_config. << std::endl;


    if(req.getMethod() == "POST"){
        if (req.getHeaders().find("content-length") == req.getHeaders().end()) {
            std::cerr << "Error: Content-Length header not found." << std::endl;
            return req;
        }
    }

    else if(req.getMethod() == "GET")
    {
       std::cout << "get_index=================>"<< req._loc.get_index() << std::endl;
    }
    // else if(req.getMethod() == "DELETE")
    // {
    //     std::string myLocation = req.getLocPath();
    //     std::cout << "=====>" << myLocation << std::endl;
    //     struct stat fileInfo;

    //     // Check if it's a directory
    //     if (stat(myLocation.c_str(), &fileInfo) == 0)
    //     {
    //         // if yes
    //         if (S_ISDIR(fileInfo.st_mode))
    //         {
    //             if (myLocation[myLocation.size() - 1] != '/')
    //                 std::cout << "409 Conflict" << std::endl;
    //             else
    //                 std::cout << "403 Permission denied" << std::endl;
    //         }
    //         // if it's a file
    //         else
    //         {
    //             // Check if the file you want to delete exists be3da
    //             if (fileExists(myLocation.c_str()))
    //             {
    //                 // Check if it has write permission
    //                 if (writePerm(myLocation.c_str()))
    //                 {
    //                     // Delete the file
    //                     int deleteResult = std::remove(myLocation.c_str());
    //                     if (deleteResult == 0)
    //                         // Successfully deleted
    //                         std::cout << "204 No content" << std::endl;
    //                     else
    //                         // Error occured during the operation of deleting
    //                         std::cout << "500 Internal Server Error" << std::endl;
    //                 }
    //                 else
    //                     std::cout << "403 Permission denied" << std::endl;
    //             }
    //             else
    //             {
    //                 std::cout << "404 Not Found" << std::endl;
    //             }
    //         }
    //     }
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
    // std::cout << "HTTP Version: " << req.getHttpV() << std::endl;
    // std::cout << "Headers: " << std::endl;
    // std::map<std::string, std::string> headers = req.getHeaders();
    // for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it) {
    //     std::cout << it->first << ": " << it->second << std::endl;
    // }
    // std::cout << "locPath : " << req.getLocPath() << std::endl;

    // if (MethodPost(req, clt);
    // Clear the requestBuffer after processing the complete request
    // requestBuffer.clear();
    // delete &req;
    return req;
}
