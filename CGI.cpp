// #include "CGI.hpp"


// CGI::CGI()
// {
// }

// CGI::~CGI()
// {
// }   

// CGI::CGI(CGI const & src)
// {
//     *this = src;
// }

// CGI & CGI::operator=(CGI const & src)
// {
//     if (this != &src)
//     {
//         // copy attributes here...
//     }
//     return *this;
// }

// void    CGI::setEnv(request &req)
// {
//     std::map<std::string, std::string>    headers = req.getHeaders();
//     if(req.getMethod() == "GET")
//     {

//         _env.push_back(std::string("REQUEST_METHOD=GET"));
//         // _env.push_back("SERVER_PORT" + req.g);
//         _env.push_back(std::string("SCRIPT_NAME=") + req.getLocPath());
//         _env.push_back(std::string("PATH_INFO=") + req.getLocPath());
//         _env.push_back(std::string("REDIRECT_STATUS=200"));
//         _env.push_back(std::string("SERVER_HOST=") + headers["Host"]);

        

//     }
    
// }