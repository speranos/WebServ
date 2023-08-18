#include "response.hpp"
#include <sys/stat.h>


std::string str(int  num)
{
    std::ostringstream conv;
    conv << num;
    return(conv.str());
}

// bool fileExists(const char* path) {
//     struct stat fileInfo;
//     return (stat(path, &fileInfo) == 0);
// }

// bool writePerm(const char* filename)
// {
//     struct stat fileStat;
//     if (stat(filename, &fileStat) != 0)
//         return false;
//     return (fileStat.st_mode & S_IWUSR) != 0;
// }

// void    response::deleteMethod(request &req)
// {
//     std::string myLocation = req.getLocPath();
//     std::cout << "=====>" << myLocation << std::endl;
//     struct stat fileInfo;
//     // Check if it's a directory
//     if (stat(myLocation.c_str(), &fileInfo) == 0)
//     {
//         // if yes
//         if (S_ISDIR(fileInfo.st_mode))
//         {
//             if (myLocation[myLocation.size() - 1] != '/')
//                 std::cout << "409 Conflict" << std::endl;
//             else
//                 std::cout << "403 Permission denied" << std::endl;
//         }
//         // if it's a file
//         else
//         {
//             // Check if the file you want to delete exists be3da
//             if (fileExists(myLocation.c_str()))
//             {
//                 // Check if it has write permission
//                 if (writePerm(myLocation.c_str()))
//                 {
//                     // Delete the file
//                     int deleteResult = std::remove(myLocation.c_str());
//                     if (deleteResult == 0)
//                         // Successfully deleted
//                         std::cout << "204 No content" << std::endl;
//                     else
//                         // Error occured during the operation of deleting
//                         std::cout << "500 Internal Server Error" << std::endl;
//                 }
//                 else
//                     std::cout << "403 Permission denied" << std::endl;
//             }
//             else
//             {
//                 std::cout << "404 Not Found" << std::endl;
//             }
//         }
//     }
// }

void   response::GetMethod(request &req)
{
    std::string pat = req.getRoot()+req._loc.get_index();
    std::string  path =req.getLocPath();
    DIR *dir = opendir(path.c_str());
     if(dir)
     {    

        if(!req._loc.get_index().empty()) 
          { 

            if(req._loc.get_cgi() &&((path.find(".php") != std::string::npos || path.find(".py") != std::string::npos)))
            {
                std::string i = req._res->cgi_exec(req);
                req.op = 1;
                closedir(dir);
            }
            else if(req.getUri().empty() ||req.getUri().find("/")!=std::string::npos)
            {
                
                if(access(pat.c_str(),F_OK) == 0){
                req._res->SetStatusCode(200);
                req._res->set_get_con_type(req);
                req._res->setContentLenghtindex(req);
                req.op = 6;
                }
                else{
                    req.statuscode = 403;
                    req.SetErrorStatusCode(403);
                    req.setStatusCodePath(req);
                    req.op = 4;
                }

                closedir(dir);
            }
             else if(!access(req.getLocPath().c_str(),R_OK))
             {
                req._res->SetStatusCode(200);
                req._res->set_get_con_type(req);
                req._res->setContentLenght(req);
                req.op = 3;
                closedir(dir);
                
             }
          }
         else if(req._loc.get_auto_index())
         {
            req._res->autoindex(req);
            req.op = 2;
            closedir(dir);
         }
        else
        {
            req.statuscode = 403;
             req.SetErrorStatusCode(403);
            req.setStatusCodePath(req);
            req.op = 4;
            closedir(dir);
        }
     }
    else if(req._loc.get_cgi()  && ((path.find(".php") != std::string::npos || path.find(".py") != std::string::npos)))
     {
        std::string i = req._res->cgi_exec(req);
        req.op = 1;
     }
    else if(access(req.getLocPath().c_str(),R_OK) == 0)
    {   
        if(access(req.getLocPath().c_str(),F_OK) == 0)
        {
            req._res->SetStatusCode(200);
            req._res->set_get_con_type(req);
            req._res->setContentLenght(req);
            req.op = 3;
        }
        else
        {   
            req.statuscode = 403;
            req.SetErrorStatusCode(403);
            req.setStatusCodePath(req);
            req.op = 4;
        }
    
    }
    else{
            req.statuscode = 404;
            req.SetErrorStatusCode(404);
            req.setStatusCodePath(req);
            req.op = 4;
    }
        
}
std::string  response::send_response_body(request &req)
{
    std::string res;
    if(req.op == 1){
        res = req._res->serveCgi(req);
    
    }
    else if(req.op == 5)
    {
        res = "HTTP/1.1 301 Moved Permanently\r\n";
        res +="Location: " + req._loc.get_redir()+"\r\n";
        res += "Content-Length: 0\r\n";
        req._isDone = true; 
        return(res);
    }
    else if(req.op == 7)
    {
        res = "HTTP/1.1 201 Created\r\n\r\n";
        req._isDone = true; 
        return res;
    }
    else if(req.op == 6){
        res = req._res->indexfile(req);
        return(res);
    }
    else if(req.op == 2){
        res = req._res->get_autoindex();
        req._isDone = true; 
        return res;
    }
    else if(req.op == 3)
        res = req._res->FinalString(req);
     else if(req.op == 4){
        std::ifstream input_file;
         res = req.error_page(req,input_file);
         
     }
    return res;
}


std::string response::getExtensionFromURI(std::string uri) {
    size_t dotPos = uri.rfind('.');
    if (dotPos != std::string::npos && dotPos + 1 < uri.length()) {
        return uri.substr(dotPos + 1);
    } else {
        return "";
    }
}

void    response::set_get_con_type(request &req)
{
    std::string extension = getExtensionFromURI(req.getUri());
    if (extension == "html" || extension == "htm") {
        _Content_Type = "Content-Type: text/html\r\n";
    } else if (extension == "css") {
        _Content_Type = "Content-Type: text/css\r\n";
    } else if (extension == "js") {
        _Content_Type = "Content-Type: application/javascript\r\n";
    } else if (extension == "jpg" || extension == "jpeg") {
        _Content_Type = "Content-Type: image/jpeg\r\n";
    } else if (extension == "png") {
        _Content_Type = "Content-Type: image/jpeg\r\n";
    } else if (extension == "gif") {
        _Content_Type = "Content-Type: image/gif\r\n";
    } else if (extension == "txt") {
        _Content_Type = "Content-Type: text/plain\r\n";
    } else if (extension == "xml") {
        _Content_Type = "Content-Type: text/xml\r\n";
    } else if (extension == "mp3") {
        _Content_Type = "Content-Type: audio/mpeg\r\n";
    } else if (extension == "mp4") {
        _Content_Type = "Content-Type: video/mp4\r\n";
    } else if (extension == "avi") {
        _Content_Type = "Content-Type: video/x-msvideo\r\n";
    } else if (extension == "mpeg") {
        _Content_Type = "Content-Type: video/mpeg\r\n";
    } else if (extension == "pdf") {
        _Content_Type = "Content-Type: application/pdf\r\n";
    } else if (extension == "zip") {
        _Content_Type = "Content-Type: application/zip\r\n";
    } else if (extension == "gz") {
        _Content_Type = "Content-Type: application/x-gzip\r\n";
    } else if (extension == "tar") {
        _Content_Type = "Content-Type: application/x-tar\r\n";
    } else if (extension == "ico") {
        _Content_Type = "Content-Type: image/x-icon\r\n";
    } else {
        _Content_Type = "Content-Type: application/octet-stream\r\n";
    }
}

    void response::setContentLenght(request &req)
    {
        // std::map<std::string, std::string> con_type = req.getHeaders();
        // std::string Content_type =  con_type["content-length"];
        // _Content_Lenght += "Content-Length: " + Content_type + "\r\n";
        std::ifstream file(req.getLocPath().c_str(), std::ios::binary);
        file.seekg(0, std::ios::end);
        this->file_size = file.tellg();
        std::cout << "file size is : " << file_size << std::endl;
        file.close();
        this->_Content_Lenght += "Content-Length: " + str(file_size) + "\r\n";
    };
int response::get_file_size()
{
    return this->file_size;
}


std::string response::FinalString(request &req)
{
    std::string res;
    if(!req._isOpen)
    {
        this->file.close();
        this->file.open(req.getLocPath().c_str(), std::fstream::binary);
        if(!file.is_open())
        {
            req.statuscode = 403;
            req.SetErrorStatusCode(403);
            req.setStatusCodePath(req);
            req.op = 4;
            req._isDone = false;
            std::cout << "--file- not open" << std::endl;
            //exit(1);
        }
        else
            req._isOpen = true;
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
            
            this->file.read(this->buffer, sizeof(this->buffer));
            std::streamsize bytesRead = this->file.gcount();
             if(bytesRead > 0)
             { 
                std::string chunk = std::string(this->buffer,bytesRead);
                 res.append(chunk);
				req._isOpen = true;
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

void response::Send(int sck,request &req)
 {
    std::string res;
    
     if( req._isDone == false){
        res = req._res->send_response_body(req);
         int count = send(sck,res.c_str(), res.size(), 0);
         if(count == -1)
         {
             std::cerr << "send failed" << std::endl;
            req._isDone = true;
         }
         if(count == 0)
         {
             req._isDone = true;
         }
     }
}


void    response::autoindex(request &req)
{
    std::string index;
    std::string path = req.getLocPath();
    DIR *dir;
    struct dirent *entry;
    if((dir = opendir(path.c_str())) != NULL)
    {
        while((entry = readdir(dir)) != NULL)
        {
            std::string name = entry->d_name;
            if (entry->d_type == DT_DIR) 
                index += "<li><a href=\"" + name + "/\">" + name + "/</a></li>";
            else 
                index += "<li><a href=\"" + name + "\">" + name + "</a></li>";
           
        }
        closedir(dir);
    }
    else
    {
       req.statuscode = 404;
            req.setStatusCodePath(req);
            req.op = 4;
        exit(1);
    }
    this->_autoindex = "HTTP/1.1 200 OK\r\n";
    this->_autoindex += "Content-Type: text/html\r\n";
    this->_autoindex += "Content-Length: " + str(index.size()) + "\r\n\r\n";
    this->_autoindex += "<html><body><h1>Directory Listing</h1><ul>" + index + "</ul></body></html>";
    //this->bodyisDone(true);
}
std::string response::get_autoindex()
{
    return this->_autoindex;
}

std::string request::setStatusCodePath(request &req)
{
    std::string res;
    if(req.statuscode == 204){
        res = "/nfs/homes/maboulho/Desktop/last_push/www/html/error_pages/204.html";
        return res;
    }
    else if(req.statuscode == 400){
        res = "/nfs/homes/maboulho/Desktop/last_push/www/html/error_pages/400.html";
        return res;
    }
    else if(req.statuscode == 403){
        res = "/nfs/homes/maboulho/Desktop/last_push/www/html/error_pages/403.html";
        return res;
    }
    else if(req.statuscode == 404){
        res = "/nfs/homes/maboulho/Desktop/last_push/www/html/error_pages/404.html";
        return res;
    }
    else if(req.statuscode == 405){
        res = "/nfs/homes/maboulho/Desktop/last_push/www/html/error_pages/405.html";
        return res;
    }
    else if(req.statuscode == 500){
        res = "/nfs/homes/maboulho/Desktop/last_push/www/html/error_pages/500.html";
        return res;
    }
    else if(req.statuscode == 501){
        res = "/nfs/homes/maboulho/Desktop/last_push/www/html/error_pages/501.html";
        return res;
    }
    else if(req.statuscode == 409){
        res = "/nfs/homes/maboulho/Desktop/last_push/www/html/error_pages/505.html";
        return res;
    }
    else if(req.statuscode == 413){
        res = "/nfs/homes/maboulho/Desktop/last_push/www/html/error_pages/413.html";
        return res;
    }
    else if(req.statuscode == 414){
        res = "/nfs/homes/maboulho/Desktop/last_push/www/html/error_pages/414.html";
        return res;
    }
    else
        return ("/nfs/homes/maboulho/Desktop/last_push/www/html/error_pages/400.html");

}
std::string request::error_page(request &req,std::ifstream &input_file)
{
    std::string res;
    std::cout << "file not open" << std::endl;
    std::string path = req.setStatusCodePath(req);
    if(!req._isOpen)
    {
        input_file.close();
        input_file.open(path.c_str(), std::fstream::binary);
        if(!input_file.is_open())
        {
           // req._res->statuscode = 403;
            //req._res->setStatusCodePath(req);
            //req.op = 4;
            
            //exit(1);
        }
        else
            req._isOpen = true;
    }
    if(req._isOpen)
    {
        if(req.headerSent == false){
       res = req.getErrorStatusCode();
       res.append("Content-Type: text/html\r\n");
       res.append(req.setErrorContentLenght(path));
       res.append("\r\n");
        req.headerSent = true;  
        }
        
        if(req.headerSent == true && !req._isDone){
            
            input_file.read(req.buffer, sizeof(req.buffer));
            std::streamsize bytesRead = input_file.gcount();
             if(bytesRead > 0)
             { 
                std::string chunk = std::string(req.buffer,bytesRead);
                 res.append(chunk);
				req._isOpen = true;
            }
            else
            {
                req._isDone = true;
                input_file.close();
                req._isOpen = false;
            }
        }
    }
    return res;
}
std::string request::setErrorContentLenght(std::string path)
    {
        // std::map<std::string, std::string> con_type = req.getHeaders();
        // std::string Content_type =  con_type["content-length"];
        // _Content_Lenght += "Content-Length: " + Content_type + "\r\n";
        
        std::ifstream file(path.c_str(), std::ios::binary);
        file.seekg(0, std::ios::end);
        this->file_size = file.tellg();
        std::cout << "file size is : " << file_size << std::endl;
        file.close();
        this->_Content_Lenght += "Content-Length: " + str(file_size) + "\r\n";
        return(this->_Content_Lenght);
    }

    std::string response::indexfile(request &req)
{
    std::string res;
    std::string  path =req.getLocPath() + "/" + req._loc.get_index();
    if(!req._isOpen)
    {
        this->file.close();
        this->file.open(path.c_str(), std::fstream::binary);
        if(!file.is_open())
        {
            req.statuscode = 403;
            req.SetErrorStatusCode(403);
            req.setStatusCodePath(req);
            req.op = 4;
            // req._isDone = false;
            //exit(1);
        }
        else
            req._isOpen = true;
    }
    if(req._isOpen)
    {
        if(req.headerSent == false){
       res = req._res->getStatusCode();
       res.append("Content-Type: text/html\r\n");
       res.append(req._res->getContentLenght());
       res.append("\r\n");
        req.headerSent = true;  
        }
        
        if(req.headerSent == true && !req._isDone){
            
            this->file.read(this->buffer, sizeof(this->buffer));
            std::streamsize bytesRead = this->file.gcount();
             if(bytesRead > 0)
             { 
                std::string chunk = std::string(this->buffer,bytesRead);
                 res.append(chunk);
				req._isOpen = true;
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
void response::setContentLenghtindex(request &req)
    {
        std::string path = req.getLocPath() + req._loc.get_index();
        // std::map<std::string, std::string> con_type = req.getHeade;rs();
        // std::string Content_type =  con_type["content-length"];
        // _Content_Lenght += "Content-Length: " + Content_type + "\r\n";
        std::ifstream file(path.c_str(), std::ios::binary);
        file.seekg(0, std::ios::end);
        this->file_size = file.tellg();
        std::cout << "file size is : " << file_size << std::endl;
        file.close();
        this->_Content_Lenght += "Content-Length: " + str(file_size) + "\r\n";
    };