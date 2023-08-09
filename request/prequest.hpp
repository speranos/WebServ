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
        };
        ~request(){
          //  delete _res;
        };
        request &operator= (request cpy)
        {
            this->_method = cpy._method;
            this->_res = cpy._res;
            this->_uri = cpy._uri;
            this->_httpV = cpy._httpV;
            this->_headers = cpy._headers;
            this->_body = cpy._body;
            this->_serverName = cpy._serverName;
            this->_locPath = cpy._locPath;
            this->_loc = cpy._loc;
            this->_fd = cpy._fd;
            this->_content_lenght = cpy._content_lenght;
            this->statusCode = cpy.statusCode;
            this->_isdone = cpy._isdone;
            return(*this);
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
        void matchLocation(std::string uri, client_config clt, int sck);
        void clear();
};

request pRequest(std::string& buffer, client_config clt, int sck);
bool MethodPost(request& req, client_config clt);

class client
{
    request rq_obj;
    public:
    void    sett_rq_object(request rq_obj){this->rq_obj = rq_obj;};
    // void    sett_res_obj(response res_obj){this->res_obj = res_obj;};
    request &get_rq_object(){return(rq_obj);};
};

typedef std::map<int, client> new_client;
#endif

