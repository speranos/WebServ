#ifndef PREQUEST_HPP
#define PREQUEST_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include "../webserv.hpp"
#define BUFF_SIZE 66000


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
        std::string port;
         int op;
         int done;
        unsigned int file_size;
        char    buffer[BUFF_SIZE];
        std::string _httpV;
        std::map<std::string, std::string> _headers;
        std::string _body;
        std::string _serverName;
        std::string _root;
        std::string _locPath;
         bool    _flag;
         bool    _isOpen;
        bool    headerSent;
        bool    _isDone;
        location_obj _loc; // location dialna
        int         _fd;
        unsigned long     _content_lenght;
         std::string       _Content_Lenght;
        std::string             _statuscode;
        int                 statuscode;
        bool _isdone;
        // response GetRes() const 
        // {
        //     return _res;
        // };


    public:
        request():_isOpen(false),headerSent(false),_isDone(false) {
        //_res = new response();
        };
        ~request(){
          //  delete _res;
        };
        request &operator= (request cpy)
        {
            this->_method = cpy._method;
            this->file_size = cpy.file_size;
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
            this->_statuscode= cpy._statuscode;
            this->statuscode=cpy.statuscode;
            this->op = cpy.op;
            this->_Content_Lenght= cpy._Content_Lenght;
            this->_isOpen = cpy._isOpen;
            this->headerSent = cpy.headerSent;
            this->_isDone = cpy._isDone;
            this->_isdone = cpy._isdone;
            return(*this);
        };
    void setstatuscodeint(int status)
    {
        this->statuscode = status;
    }
    int getstatuscodeint()
    {
        return this->statuscode;
    }

    std::string getErrorStatusCode()
    {
        return this->_statuscode;
    };
     void    SetErrorStatusCode(int _status)
    {
        if(_status == 200)
        this->_statuscode = "HTTP/1.1 200 OK\r\n";
        else if(_status == 201)
        this->_statuscode = "HTTP/1.1 201 Created\r\n";
        else if(_status == 301)
        this->_statuscode = "HTTP/1.1 301 Moved Permanently\r\n";
        else if(_status == 304)
        this->_statuscode = "HTTP/1.1 304 Not Modified\r\n";
        else if(_status == 400)
        this->_statuscode = "HTTP/1.1 400 Bad Request\r\n";
        else if(_status == 401)
        this->_statuscode = "HTTP/1.1 401 Unauthorized\r\n";
        else if(_status == 403)
        this->_statuscode = "HTTP/1.1 403 Forbidden\r\n";
        else if(_status == 404)
        this->_statuscode = "HTTP/1.1 404 Not Found\r\n";
        else if(_status == 405)
        this->_statuscode = "HTTP/1.1 405 Method Not Allowed\r\n";
        else if(_status == 500)
        this->_statuscode = "HTTP/1.1 500 Internal Server Error\r\n";
        else if(_status == 501)
        this->_statuscode = "HTTP/1.1 501 Not Implemented\r\n";
    };

        std::string error_page(request &req,std::ifstream &input_file);
        std::string setErrorContentLenght(std::string path);
        std::string setStatusCodePath(request &req);
        std::string to_str(int  num);
        std::string getMethod() const;
        std::string getUri() const;
        std::string getHttpV() const;
        std::map<std::string, std::string> getHeaders() const;
        std::string getBody() const;
        int         getStatusCode() const;
        std::string getServerName() const;
        std::string getLocPath() const;
        std::string getRoot() const;
        int         getFd() const;
        location_obj getLoc() const;
        unsigned long         getContentLenght() const;
        bool        getIsDone() const;
        bool        getFlag() const;

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
        void setRoot(const std::string& path);
        void setIsDone(const bool& isdone);
        void setFlag(const bool& isdone);
        void clear();
};

typedef std::map<int, request> requests;
void matchLocation(request& req, std::string uri, client_config clt, int sck);
int  analyzeRequest(request &req);
request pRequest(std::string& buffer, client_config clt, int sck, requests& map, int ret_read);

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

