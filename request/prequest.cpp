#include "prequest.hpp"

void pRequest(const std::string& input, client clt, int sck){

    std::string line;
    std::string method;
    std::string uri;
    std::string httpVersion;
    std::map<std::string, std::string> headers;
    std::string body;
    request req;
 
    std::istringstream stream(input);
    while(getline(stream, line) && line != "\r"){
        // pass first line to parseFirstLine
        if (line.find("HTTP") != std::string::npos){
            std::istringstream stream(line);
            stream >> method >> uri >> httpVersion;
            continue;
        }
        std::string::size_type pos = line.find(": ");
        
        if (pos != std::string::npos){
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
            if (headers.find(key) != headers.end()) {
                headers[key] += ", " + value;
            } else {
                headers[key] = value;
            }
        }
    }
    // set all variable as a request object
    req.setMethod(method);
    req.setUri(uri);
    req.setHttpV(httpVersion);
    req.setHeaders(headers);
    req.setBody(body);
    if(req.analyzeRequest()){
        std::cout << "Request is valid" << std::endl;
        req.matchLocation(uri ,clt, sck);
    }
    // else 
    //     exit(1);
    req.setIsDone(true);


    // // // print request object
    // std::cout << "Method: " << req.getMethod() << std::endl;
    // std::cout << "URL: " << req.getUri() << std::endl;
    // std::cout << "HTTP Version: " << req.getHttpV() << std::endl;
    // // printf headers map of object request

    // std::map<std::string, std::string> headersMap = req.getHeaders();
    // for(std::map<std::string, std::string>::iterator it = headersMap.begin(); it != headersMap.end(); ++it){
    //     std::cout << it->first << " => " << it->second << '\n';
    // }
    

    // std::cout << "Method: " << method << std::endl;
    // std::cout << "URL: " << url << std::endl;
    // std::cout << "HTTP Version: " << httpVersion << std::endl;
    // // printf headers map
    // for(std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); ++it){
    //     std::cout << it->first << " => " << it->second << '\n';
    // }
}

    