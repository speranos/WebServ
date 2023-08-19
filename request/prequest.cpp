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
    if (head.find("Content-Type") == head.end()) {
        contentType = ".bin";
    } else {
        contentType = head["Content-Type"];
        // std::cout << "Content type =============================>>>"<<contentType << std::endl;
    }
    std::string extension;
    size_t pos = contentType.find('\r');
    if (pos != std::string::npos) {
        contentType = contentType.substr(0, pos);
    }
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
    return extension;
}

size_t min(size_t s1, size_t s2){
    if(s1 > s2)
        return s2;
    return s1; 
}

// Function to store the request body in a file
void storeRequestBody(std::istringstream& stream, request& req, int sck, int ret_read) {

    std::string filename;
    if(req.getBody().empty()){
        std::string extension = set_extension(req);
        std::ostringstream rnd_num;
        rnd_num << rand();
        std::ostringstream str_sck;
        str_sck << sck;
        filename = "request_body_" + rnd_num.str() + "_" + str_sck.str() + extension;
        req.setBody(filename);
    } 
    else 
        filename = req.getBody();

    std::ofstream bodyFile(filename.c_str(), std::ios::out | std::ios::binary | std::ios::app);
    if (!bodyFile)
        std::cerr << "Error: Failed to create body file." << std::endl;
    std::string buffer;
    size_t bytes = 0;
    std::string bff;
    bff.resize(1024);
    bff = stream.str();
    if (bff.find("\r\n\r\n") != std::string::npos) {
        bff = bff.substr(bff.find("\r\n\r\n") + 4);
    }
    bytes = bff.size();
    bytes = min(bytes, ret_read);
    bodyFile.write(bff.data(), bytes);
    bff.clear();
    // Check if the entire body is successfully stored in the file
    if (req.getContentLenght() <= (unsigned long)bodyFile.tellp()) {
        // std::cout << "Request body stored in file: " << filename << std::endl;
        bodyFile.close();
        req.setBody(filename);
        req.setIsDone(true);
        if(req.getContentLenght() != (unsigned long)bodyFile.tellp())
            req.setFlag(true);     
    }
}

// Main parsing function
request pRequest(std::string& buffer, client_config clt, int sck, requests& map, int ret_read) {

    requests::iterator search = map.find(sck);
    int flag_req = 0;
    if (search != map.end()) {
        flag_req = 1;
    }
    if(flag_req == 0){
        map.insert(std::make_pair(sck, request()));
    }
    requests::iterator second = map.find(sck);
    request &req = second->second;

    req.setIsDone(false);
    std::istringstream stream(buffer);
    if (!parseRequestLine(stream, req) && !req.getMethod().empty()) {
        std::cerr << "Error 400 bad request: Invalid request line." << std::endl;
        return req;
    }

    if (!parseHeaders(stream, req)) {
        std::cerr << "Error 400 bad request: Invalid request headers." <<  std::endl;
        return req;
    }
    std::string bff = stream.str();
    if((req.getMethod() != "POST") && bff.find("\n\r\n\r")){
        req.setIsDone(true);        
    }
    std::map<std::string, std::string>::iterator it = req._headers.find("Content-Length");
    if (flag_req == 0 && it != req._headers.end()){
        unsigned long len;
        std::istringstream (it->second) >> len;
        req.setContentLenght(len);  
            // std::cout << "parsing request ****** sck" << sck << std::endl;
    }

    if(req.getMethod() == "POST"){
        storeRequestBody(stream, req, sck, ret_read);
    }

    if (req.getIsDone() == true){
        // std::cout << "**************" << std::endl;
        analyzeRequest(req);
        matchLocation(req, req.getUri() ,clt, sck);
    }
    
    std::map<int, Server_obj>::iterator myserver = clt.find(sck);
    req.setServerName(myserver->second.get_host());
    req.setFd(sck);
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

    return req;
}
