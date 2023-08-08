#include "prequest.hpp"

// request::request(){
//     std::cout << "request constructor called" << std::endl;
// }

// request::~request(){
//     std::cout  << "destruct request" << std::endl;
// }

std::string request::getMethod() const{
    return _method;
}

std::string request::getUri() const{
    return _uri;
}

std::string request::getHttpV() const{
    return _httpV;
}

std::map<std::string, std::string> request::getHeaders() const{
    return _headers;
}

std::string request::getBody() const{
    return _body;
}

int request::getStatusCode() const{
    return statusCode;
}

std::string request::getServerName() const{
    return _serverName;
}

std::string request::getLocPath() const{
    return _locPath;
}

int request::getFd() const{
    return _fd;
}

unsigned long request::getContentLenght() const{
    return _content_lenght;
}

location_obj request::getLoc() const{
    return _loc;
}

bool request::getIsDone() const{
    return _isdone;
}

void request::setMethod(const std::string& method){
    _method = method;
}

void request::setUri(const std::string& url){
    _uri = url;
}

void request::setHttpV(const std::string& httpV){
    _httpV = httpV;
}

void request::setHeaders(const std::map<std::string, std::string>& headers){
    _headers = headers;
}

void request::setBody(const std::string& body){
    _body = body;
}

void request::setStatusCode(const StatusCode& statuscode){
    statusCode = statuscode;
}

void request::setServerName(const std::string& serverName){
    _serverName = serverName;
}

void request::setLocPath(const std::string& path){
    _locPath = path;
}

void request::setFd(const int& fd){
    _fd = fd;
}

void request::setContentLenght(const unsigned long& content_lenght){
    _content_lenght = content_lenght;
}

void request::setIsDone(const bool& isdone){
    _isdone = isdone;
}

void request::setLoc(const location_obj& loc){
    _loc = loc;
}

void request::clear(){
    _method.clear();
    _uri.clear();
    _httpV.clear();
    _headers.clear();
    _body.clear();
    _serverName.clear();
    _locPath.clear();
    _fd = 0;
    _content_lenght = 0;
    _isdone = false;
}


bool compare(std::string s1, std::string s2){
    return s1.size() < s2.size();
}

typedef std::vector<location_obj> location;
void request::matchLocation(std::string url, client clt, int sck){
    // match request url with location
    // setLocPath(std:npos);
    std::map<int, Server_obj>::iterator myserver = clt.find(sck);
    // if (myserver)
    //     std::cout << "server not found" << std::endl;
    Server_obj server = myserver->second;
    std::vector<location_obj> locations = server.get_location();


    location cpy_location = myserver->second.get_location();
    int loc_size = cpy_location.size();

    std::vector<std::string> loc_list;
    for (int it = 0; it < loc_size; it++){
        loc_list.push_back(cpy_location[it].get_location());
        std::cout << cpy_location[it].get_location() << std::endl;
    }
    // sort all location by lenght in loc_list in descending order
    sort(loc_list.begin(), loc_list.end(), compare);
    for(int it2 = 0; it2 < (int)loc_list.size(); it2++)
        std::cout << "location " << it2 << ">> " << loc_list[it2] << std::endl;
    // then match the uri starting from the top of list
    std::string root;
    // std::cout << "initial_root  >> "  << root << std::endl;
    if (std::find(loc_list.begin(), loc_list.end(), url) != loc_list.end()){
        std::cout << "match" << std::endl;
        // get the location object from cpy_location vector
        std::vector<std::string>::iterator it = std::find(loc_list.begin(), loc_list.end(), url);
        int index = std::distance(loc_list.begin(), it);
        // std::cout << "index >> " << index << std::endl;
        setLoc(cpy_location[index]);
        root = cpy_location[index].get_root();
        if (!root.empty()){
            std::string locPath = root + url;
            setLocPath(locPath);
            
        }else{
            std::string locPath = url;
            setLocPath(locPath);
        }
    }else
       std::cout << "404 not found" << std::endl;
}



int  request::analyzeRequest() const{
    const std::string& method = getMethod();

    // Method Validation
    if (method == "GET" || method == "POST" || method == "DELETE") {
        std::cout << "Method is valid" << std::endl;
    } else {
        std::cout << "invalid method" << std::endl;
    }
    // // http version validation
    if (_httpV == "HTTP/1.1") {
        std::cout << "HTTP version is valid" << std::endl;
    } else {
        std::cout << "HTTP version is invalid" << std::endl;
    }

    // search for  transfer encoding header if it exists  then check it if it is chunked
    std::map<std::string, std::string> headers = getHeaders();
    std::map<std::string, std::string>::iterator it = headers.find("Transfer-Encoding");
    if(it != headers.end()){
        // std::cout << "Transfer-Encoding header exists" << std::endl;
        if(it->second == "chunked"){
            // std::cout << "Transfer-Encoding is chunked" << std::endl;
            it = headers.find("Content-Length");
            if(it != headers.end() && method != "POST"){
                std::cout << "400 Bad Request" << std::endl;
            }
            // else{
            //     std::cout << "Content-Length :" << it->second << std::endl;
            // }
        }
        else{
            std::cout << "501 Not Implemented" << std::endl;
        }
    }
    // // uri if it contain invalid characters
    if(_uri.find_first_of(" \t\n\v\f\r") != std::string::npos){
        std::cout << "400 Bad Request" << std::endl;
    }
    // // check uri lenght if it is more than 2048
    if(_uri.length() > 2048){
        std::cout << "414 Request-URI Too Long" << std::endl;
    }
    return (1);
}




