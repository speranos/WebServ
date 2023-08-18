#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <fcntl.h>
#include "webserv.hpp"
//  class request;
#define BUFF_SIZE 66000
// #include "request/prequest.hpp"
class request;

class response
{
    public:
        std::vector<std::string>    _env;
        pid_t pid;
        int status;
        int op;
        int statuscode;
     char buffer[BUFF_SIZE];
      std::ifstream file;
      std::ifstream file_;
      std::string _buffer;
      std::string _body;
      std::string _autoindex;
     bool _isOpen;
    bool            headerSent;
    std::string     _headers;
    std::string     _Content_Lenght;
    std::string     _Content_Type;
    std::string             _statuscode;
     int            file_size;
     bool           _isDone;
    // std::ifstream _file;
    

    public:
    response():_isOpen(false),headerSent(false),_isDone(false){};
    ~response(){};
    void    SetStatusCode(int _status)
    {
        if(_status == 200)
        _statuscode = "HTTP/1.1 200 OK\r\n";
        else if(_status == 201)
        _statuscode = "HTTP/1.1 201 Created\r\n";
        else if(_status == 301)
        _statuscode = "HTTP/1.1 301 Moved Permanently\r\n";
        else if(_status == 304)
        _statuscode = "HTTP/1.1 304 Not Modified\r\n";
        else if(_status == 400)
        _statuscode = "HTTP/1.1 400 Bad Request\r\n";
        else if(_status == 401)
        _statuscode = "HTTP/1.1 401 Unauthorized\r\n";
        else if(_status == 403)
        _statuscode = "HTTP/1.1 403 Forbidden\r\n";
        else if(_status == 404)
        _statuscode = "HTTP/1.1 404 Not Found\r\n";
        else if(_status == 405)
        _statuscode = "HTTP/1.1 405 Method Not Allowed\r\n";
        else if(_status == 500)
        _statuscode = "HTTP/1.1 500 Internal Server Error\r\n";
        else if(_status == 501)
        _statuscode = "HTTP/1.1 501 Not Implemented\r\n";
    };
    void methodDelete(request& req);
    std::string error_page(request &req);
    void MethodPost(request& req);
    std::string setStatusCodePath(request &req);
    void setContentLenght(request &req);
    std::string  send_response_body(request &req);
    void    set_get_con_type(request &req);
    std::string getExtensionFromURI(std::string uri);
    std::string FinalString(request &req);
    std::string get_Body();
    int get_file_size(); 
    void Send(int sck,request &req);
    void    set_body(std::string body);
    std::string get_body();
    void    autoindex(request &req);
    std::string get_autoindex();
    std::string setErrorContentLenght(std::string path);
    void setContentType(request &req)
    {
        std::map<std::string, std::string> con_type = req.getHeaders();
        std::string Content_type =  con_type["content-type"];
        _Content_Type += "Content-type: " + Content_type + "\r\n";
    };

    std::string getContentLenght()
    {
        return _Content_Lenght;
    };
    std::string getContentType()
    {
        return _Content_Type;
    };
    std::string getStatusCode()
    {
        return this->_statuscode;
    };
    
    // bool getIsDone()
    // {
    //     return this->_isDone;
    // };
    void isfileopen(bool isopen)
    {
        this->_isOpen = isopen;
    }
     void isheadersdone(bool isopen)
    {
        this->headerSent = isopen;
    }
     void bodyisDone(bool isDone)
    {
        this->_isDone = isDone;
    }

// void    setGet_Body(std::string &body)
// {
//     this->_buffer += body;
// }
void    GetMethod(request &req);
 void    SendResponse(int sck);
 std::string indexfile(request &req);
 void setContentLenghtindex(request &req);
        std::string  setStatusCodePath(int status);
        void setContentLenghtCgi(std::string body);
        void setContentTypeCgi(std::string body); 
        void    setStatusCodeCgi(std::string body);
        void    setLocationCgi(std::string body);
        void    setCookieCgi(std::string body);
        void    setBodyCgi(std::string body);
        std::string getStatusCodeCgi();
        std::string getLocationCgi();
        std::string getCookieCgi();
        std::string getContentTypeCgi();
        std::string getContentLenghtCgi();
        std::string getBodyCgi();

       // void    setBodyCgi(std::string &body);
        std::string getBody();
        void    setEnv(request & req);
        void get_cgi_body(std::string &res,request &req);
        std::string serveCgi(request &req);
        char** env_to_char();
        std::string    cgi_exec(request & req);
        std::string set_cgi_executable(request &req);


};
#endif