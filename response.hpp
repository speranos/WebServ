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
     char buffer[BUFF_SIZE];
      std::ifstream file;
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
    void    SetStatusCode(std::string _status)
    {
        _statuscode = _status;
    };
    void setContentLenght(request &req);
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
        this->_isOpen = isopen;
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
};
#endif