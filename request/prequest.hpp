#ifndef PREQUEST_HPP
#define PREQUEST_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <sys/types.h>
#include "../webserv.hpp"

//#include "../response.hpp"

class response;
enum StatusCode {
    OK = 200,
    NOT_FOUND = 404,
    INTERNAL_SERVER_ERROR = 500
};
class request{
    public:
        std::string _method;
        response *_res;
        std::string _uri;
        std::string _httpV;
        std::map<std::string, std::string> _headers;
        std::string _body;
        std::string _serverName;
        std::string _locPath;
        location_obj _loc; // location dialna
        int         _fd;
        unsigned long     _content_lenght;  
        StatusCode statusCode;
        bool _isdone;
        // response GetRes() const 
        // {
        //     return _res;
        // };


    public:
        request() {
        //_res = new response();
    }
        ~request(){
          //  delete _res;
        };

        std::string getMethod() const;
        std::string getUri() const;
        std::string getHttpV() const;
        std::map<std::string, std::string> getHeaders() const;
        std::string getBody() const;
        int         getStatusCode() const;
        std::string getServerName() const;
        std::string getLocPath() const;
        int         getFd() const;
        location_obj getLoc() const;
        unsigned long         getContentLenght() const;
        bool        getIsDone() const;

        void setMethod(const std::string& method);
        void setUri(const std::string& url);
        void setHttpV(const std::string& httpV);
        void setHeaders(const std::map<std::string, std::string>& headers);
        void setBody(const std::string& body);
        void setStatusCode(const StatusCode& statuscode);
        void setServerName(const std::string& serverName);
        void setFd(const int& fd);
        void setContentLenght(const unsigned long& content_lenght);
        void setLoc(const location_obj& loc);
        void setLocPath(const std::string& path);
        void setIsDone(const bool& isdone);
        int  analyzeRequest() const;
        void matchLocation(std::string uri, client clt, int sck);
        void clear();
};

request pRequest(std::string& buffer, client clt, int sck);
bool MethodPost(request& req, client clt);

#endif

