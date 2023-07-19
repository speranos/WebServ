#ifndef PREQUEST_HPP
#define PREQUEST_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <sys/types.h>
#include "../webserv.hpp"

enum StatusCode {
    OK = 200,
    NOT_FOUND = 404,
    INTERNAL_SERVER_ERROR = 500
};

class request{
    private:
        std::string _method;
        std::string _uri;
        std::string _httpV;
        std::map<std::string, std::string> _headers;
        std::string _body;
        std::string _serverName;
        std::string _locPath;
        int         _fd;
        // double     _content_lenght;  
        StatusCode statusCode;
        bool _isdone;


    public:
        request();
        ~request();

        std::string getMethod() const;
        std::string getUri() const;
        std::string getHttpV() const;
        std::map<std::string, std::string> getHeaders() const;
        std::string getBody() const;
        int         getStatusCode() const;
        std::string getServerName() const;
        std::string getLocPath() const;
        int         getFd() const;
        bool        getIsDone() const;

        void setMethod(const std::string& method);
        void setUri(const std::string& url);
        void setHttpV(const std::string& httpV);
        void setHeaders(const std::map<std::string, std::string>& headers);
        void setBody(const std::string& body);
        void setStatusCode(const StatusCode& statuscode);
        void setServerName(const std::string& serverName);
        void setFd(const int& fd);
        void setLocPath(const std::string& path);
        void setIsDone(const bool& isdone);
        int  analyzeRequest() const;
        void matchLocation(std::string uri, client clt, int sck);
};

void pRequest(const std::string& buffer, client clt, int sck);

#endif
