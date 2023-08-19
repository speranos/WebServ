#include "response.hpp"
#include <sys/types.h>
#include <sys/wait.h>

std::string to_str(int  num)
{
    std::ostringstream conv;
    conv << num;
    return(conv.str());
}
std::string get_query_string(std::string uri) {
    size_t questionMarkPos = uri.find('?');
    if (questionMarkPos != std::string::npos && questionMarkPos + 1 < uri.length()) {
        return uri.substr(questionMarkPos + 1);
    } else {
        return "";
    }
}
void    response::setEnv(request &req)
{
    std::map<std::string, std::string>    headers = req.getHeaders();
    std::string path = req.getLocPath();
    std::string server_name = req.getServerName();
    this->_env.push_back(std::string("REQUEST_METHOD=") + req.getMethod());
    this->_env.push_back("SERVER_PORT=" + req.port);
    this->_env.push_back(std::string("HTTP_PORT=") + req.port);
    this->_env.push_back(std::string("SCRIPT_NAME=") + path);
    this->_env.push_back(std::string("SCRIPT_FILENAME=") + path);
    this->_env.push_back(std::string("PATH_INFO=") + path);
    this->_env.push_back(std::string("QUERY_STRING=") + get_query_string(req.getUri()));
    this->_env.push_back(std::string("REDIRECT_STATUS=200"));
    this->_env.push_back(std::string("SERVER_HOST=") + server_name);
    this->_env.push_back(std::string("HTTP_HOST=") + server_name);
    this->_env.push_back(std::string("SERVER_PROTOCOL=HTTP/1.1"));
    this->_env.push_back(std::string("USER_AGENT=") + headers["User-Agent"]);
    this->_env.push_back(std::string("GATEWAY_INTERFACE=CGI/1.1"));

    if(req.getMethod() == "POST")
    {
        this->_env.push_back(std::string("CONTENT_TYPE=") + headers["Content-Type"]);
        this->_env.push_back(std::string("CONTENT_LENGTH=") + req.to_str(req.getContentLenght()));
    }
    
}

char** response::env_to_char()
{
    char **env = new char*[this->_env.size() + 1];
    int i = 0;

    for (std::vector<std::string>::iterator it = this->_env.begin(); it != this->_env.end(); it++)
    {
        env[i] = new char[(*it).size() + 1];
        env[i] = strcpy(env[i], (*it).c_str());
        i++;
    }
    env[i] = NULL;
    return env;
}
std::string getExtension(std::string uri) {
    size_t dotPos = uri.rfind('.');
    if (dotPos != std::string::npos && dotPos + 1 < uri.length()) {
        return uri.substr(dotPos + 1);
    } else {
        return "";
    }
}
std::string response::set_cgi_executable(request &req)
{
    std::string path = req.getLocPath();
    std::string res;
    std::string ext = getExtension(path);
    if(ext == "php"){
        res = "/usr/bin/php-cgi";
        return res;
    }
    else if(ext == "py"){
        res = "/usr/bin/python3";
        return res;
    }
    else
        res = "";
    return res;

}
    std::string generateRandomFileName(const std::string& extension) {
    const char* characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const int length = 10; // Change the length as needed
    std::string randomName;

    srand(static_cast<unsigned int>(time(NULL)));

    for (int i = 0; i < length; ++i) {
        randomName += characters[rand() % (sizeof(characters) - 1)];
    }

    randomName += extension;
    return randomName;
}

std::string  response::cgi_exec(request &req)
{
    int fd_in = 0;
    int fd_out;
    std::string res;
    // req.getLoc().get_upload()
    std::string local = req.getLoc().get_upload() + req.getBody();
    std::string cgi =  "./upload/"+generateRandomFileName(".txt");
    if (req.getMethod() == "POST")
        fd_in = open(local.c_str(), O_RDWR | O_CREAT| O_TRUNC, 0666);
    else if (req.getMethod() == "GET")
        fd_out = open(cgi.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666);
    setEnv(req);
    char **env = env_to_char();
    lseek(fd_in, 0, SEEK_SET);
    std::string path = set_cgi_executable(req);
    this->pid = fork();
    if(this->pid == -1)
    {
        perror("fork");
        exit(1);
    }    
     else if(this->pid == 0)
    {
        std::string pathloc = req.getLocPath();
        char *arg[3]= {(char *)path.c_str(),(char *)pathloc.c_str(), NULL};
        if(!arg[0] || !arg[1])
        {
            req.statuscode = 500;
            req.SetErrorStatusCode(500);
            req.setStatusCodePath(req);
            req.op = 4;
                exit(1);
        }
		 if(dup2(fd_in, 0) == -1)
        {
            perror("dup2_in");
            exit(1);
        }
        if(dup2(fd_out, 1) == -1)
        {
            perror("dup2_out");
            exit(1);
        }
        close(fd_out);
        if (fd_in != 0)
            close(fd_in);
        execve(path.c_str(), arg, env);
            perror("execve");
            exit(1);
    }
    else
    {
        
        std::string line;
        waitpid(this->pid, NULL, 0);

        std::ifstream inputfile;
        inputfile.open(cgi.c_str());
        res = "HTTP/1.1 200 OK\r\n";
        std::string ext = getExtension(req.getLocPath());
        if (ext == "py")
        {
            res += "Content-Type: text/html\r\n";
            res += "\r\n";
        }

        while(getline(inputfile,line))
        {
            res += line;
            res += "\n";
        }
        inputfile.close();
        if (fd_in != 0)
            close(fd_in);
        std::ofstream output_file;
        output_file.open(cgi.c_str());
        output_file << res;
        output_file.close();
    }
    close(fd_out);
    for (size_t i = 0; i < _env.size(); i++)
        delete [] env[i];
    delete [] env;
    if (this->pid == 0)
        exit(0);
    
    return cgi;
    
}


std::string response::serveCgi(request &req)
{
    
   std::string res;
    if(!req._isOpen)
    {
        std::string path = req._res->cgi_exec(req);
        this->file.open(path.c_str(), std::ios::in);
        if(!file.is_open())
        {
            req.statuscode = 403;
            req.SetErrorStatusCode(403);
            req.setStatusCodePath(req);
            req.op = 4;
        }
        else
            req._isOpen= true;
    }
    if(req._isOpen)
    {
        if(req.headerSent == false){
       res = req._res->getStatusCode();
       res.append(req._res->getContentType());
       res.append(req._res->getContentLenght());
       res.append("\r\n");
        req.headerSent = true;  
        }
        
        if(req.headerSent == true && !req._isDone){
            
            this->file.read(this->buffer, 1024);
            std::streamsize bytesRead = this->file.gcount();
             if(bytesRead > 0)
             { 
                std::string chunk = std::string(this->buffer,bytesRead);
                 res.append(chunk);
				req._isOpen= true;
            }
            else
            {
                req._isDone = true;
                this->file.close();
                req._isOpen = false;
        
            }
        }
    }
    
    return res;
}