#include "prequest.hpp"

std::string request::to_str(int  num)
{
    std::ostringstream conv;
    conv << num;
    return(conv.str());
}
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

std::string request::getServerName() const{
    return _serverName;
}

std::string request::getLocPath() const{
    return _locPath;
}

std::string request::getRoot() const{
    return _root;
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

bool request::getFlag() const{
    return _flag;
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


void request::setServerName(const std::string& serverName){
    _serverName = serverName;
}

void request::setLocPath(const std::string& path){
    _locPath = path;
}

void request::setRoot(const std::string& path){
    _root = path;
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
void request::setFlag(const bool& flag){
    _flag = flag;
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

int find(std::vector<std::string> list, std::string url){
    int i = 0;
    for (std::vector<std::string>::iterator it = list.begin(); it != list.end(); it++, i++){
        if (list[i].find(url) != std::string::npos)
            return i;
    }
    return -1;
}


// Partition the vector for quicksort
int partition(std::vector<std::string>& arr, int low, int high) {
    size_t pivot = arr[high].length();
    int i = (low - 1);

    for (int j = low; j <= high - 1; ++j) {
        if (arr[j].length() < pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

// Quicksort algorithm
void quickSort(std::vector<std::string>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Custom sort function
void customSort(std::vector<std::string>& arr) {
    quickSort(arr, 0, arr.size() - 1);
}

typedef std::vector<location_obj> location;
void matchLocation(request& req,std::string url, client_config clt, int sck){

    std::map<int, Server_obj>::iterator myserver = clt.find(sck);

    Server_obj server = myserver->second;
    if(req.getContentLenght() > server.getBodySize() && server.getBodySize_string() != "-1"){
        // std::cout << "body_size========== " << server.getBodySize() << std::endl;
        std::cout << "LARGE BODY CONTENT" << std::endl;
        req.statuscode = 413;
        req.SetErrorStatusCode(413);
        req.setStatusCodePath(req);
        req.op = 4;
    }
    std::vector<location_obj> locations = server.get_location();


    location cpy_location = myserver->second.get_location();
    int loc_size = cpy_location.size();

    std::vector<std::string> loc_list;
    std::vector<std::string> unsorted_list;
    for (int it = 0; it < loc_size; it++){
        loc_list.push_back(cpy_location[it].get_location());
        unsorted_list.push_back(cpy_location[it].get_location());
    }
    // sort all location by lenght in loc_list in descending order
    customSort(loc_list);

    // then match the uri starting from the top of list
    std::string root;
    std::string tmp_url = url;
    std::string locPath;
    while(1){

        int x = find(loc_list, url);
        if(url.empty())
            url = "/";
        if (x >= 0){
            int index = find(unsorted_list, loc_list[x]);
            if(index == -1){

                req.statuscode = 405;
                req.SetErrorStatusCode(400);
                req.setStatusCodePath(req);
                req.op = 4;
            }
            if(loc_list[x] != unsorted_list[index]){
                for(size_t r = 0; r < unsorted_list.size(); r++){
                    if(unsorted_list[r] == "/"){
                        index = r;
                    }
                }
            }
            std::list<std::string> mylist;
             req.done = 0;
            mylist = locations[index].get_method_list();
            std::list<std::string>::iterator t = mylist.begin();
            for(; t != mylist.end() ; t++){
                if(req.getMethod() == *t)
                    req.done = 1;
            }
            if(!req.done){
                // Method not allowed in location
                std::cout << "Method not allowed in location" << std::endl;
                req.statuscode = 405;
                req.SetErrorStatusCode(405);
                req.setStatusCodePath(req);
                req.op = 4;

                 break;
            }
            req.setLoc(cpy_location[index]);
            root = cpy_location[index].get_root();
            if (!root.empty()){
                req.setRoot(root);
                locPath = root + tmp_url;
                req.setLocPath(locPath);
                break;
                
            }else{
                locPath = tmp_url;
                req.setLocPath(locPath);
                break;
            }
        }
        if (locPath.empty())
            url = url.substr(0, url.find_last_of("/"));
        else if (url.empty())
            break;

    }
    // std::cout << "loc_path >>" << req.getLocPath() << std::endl;       
}



int  analyzeRequest(request& req){
    const std::string& method = req.getMethod();

    // Method Validation
    if (method != "GET" && method != "POST" && method != "DELETE")
    {
        req.statuscode = 405;
        req.SetErrorStatusCode(405);
        req.setStatusCodePath(req);
        req.op = 4;
    }
    // // http version validation
    if (req._httpV == "HTTP/1.1");
    else {
        req.statuscode = 405;
        req.SetErrorStatusCode(405);
        req.setStatusCodePath(req);
        req.op = 4;
    }

    // search for  transfer encoding header if it exists  then check it if it is chunked
    std::map<std::string, std::string> headers = req.getHeaders();
    std::map<std::string, std::string>::iterator it = headers.find("Transfer-Encoding");
    if(it != headers.end()){
        // std::cout << "Transfer-Encoding header exists" << std::endl;
        if(it->second == "chunked"){
            // std::cout << "Transfer-Encoding is chunked" << std::endl;
            it = headers.find("Content-Length");
            if(it != headers.end() && method != "POST"){
                req.statuscode = 400;
                req.SetErrorStatusCode(400);
                req.setStatusCodePath(req);
                req.op = 4;
            }
        }
        else{
            req.statuscode = 501;
            req.SetErrorStatusCode(501);
            req.setStatusCodePath(req);
            req.op = 4;
        }
    }
    // // uri if it contain invalid characters
    if(req._uri.find_first_of(" \t\n\v\f\r") != std::string::npos){
        req.statuscode = 400;
        req.SetErrorStatusCode(400);
        req.setStatusCodePath(req);
        req.op = 4;
    }
    // // check uri lenght if it is more than 2048
    if(req._uri.length() > 2048){
        req.statuscode = 414;
        req.SetErrorStatusCode(414);
        req.setStatusCodePath(req);
        req.op = 4;
    }
    return (1);
}

