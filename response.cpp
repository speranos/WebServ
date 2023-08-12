#include "response.hpp"

void   response::GetMethod(request &req)
{
    // DIR *dir = opendir(req.getLocPath().c_str());
    //  if(dir)
    //  {
    //     if(req._loc.get_index().empty()) 
    //       {  
    //         if(req._loc.get_cgi() && !access(req.getLocPath().c_str(),X_OK))
    //         {

    //             std::cout << "cgu" << std::endl;
    //         }
    //         else if(!access(req.getLocPath().c_str(),R_OK))
    //          {
    //             //send index;
    //             req._res->SetStatusCode("HTTP/1.1 200 OK\r\n");
    //             req._res->set_get_con_type(req);
    //             req._res->setContentLenght(req);
    //             set_body(FinalString(req));
    //          }  
    //         else
        //  if(req._loc.get_auto_index())
        //  {
        //     //send autoindex
        //     std::cout << " boooooooooooool "<<req._res->_isDone<<std::endl;
        //     req._res->autoindex(req);
        //     req._res->bodyisDone(true);
        //     //std::cout << "auto index"<<get_autoindex() << std::endl;
            
            
        //  }
    //     else
    //     {
    //         closedir(dir);
    //         //send 403;
    //         //req._res->SetStatusCode("HTTP/1.1 403 Forbidden\r\n");
    //         set_body("403 Forbidden");
    //     }
    //  }
    //  }
    if(access(req.getLocPath().c_str(),F_OK) == 0)
    {
        if(!access(req.getLocPath().c_str(),R_OK))
        {
            //send file
            req._res->SetStatusCode("HTTP/1.1 200 OK\r\n");
            req._res->set_get_con_type(req);
            req._res->setContentLenght(req);
            //req._res->set_body(req._res->FinalString(req));

        }
        // else
        // {
        //     // send 404
        //     //req._res->SetStatusCode();
        //     set_body(req._res->Error_Body("HTTP/1.1 404 Not Found\r\n"));
        // }
    }
        
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
        std::ifstream file(req.getLocPath(), std::ios::binary);
        file.seekg(0, std::ios::end);
        this->file_size = file.tellg();
        std::cout << "file size is : " << file_size << std::endl;
        file.close();
        this->_Content_Lenght += "Content-Length: " + std::to_string(file_size) + "\r\n";
    };
int response::get_file_size()
{
    return this->file_size;
}
// void   response::setGet_Body(request &req)
// {
//     int fd;
//     fd = open(req.getLocPath().c_str(),O_RDONLY);
//     if(fd == -1)
//     {
//         perror("open");
//         exit(1);
//     }
//     this->buff = new char[1025]();
//     int ret;
//     ret = read(fd,buff,1024);
//     // if(ret > 0)
//         // this->_buffer += buff;
    
//     close(fd);
// }



std::string response::FinalString(request &req)
{
    std::string res;
    if(!this->_isOpen)
    {
        this->file.close();
        this->file.open(req.getLocPath().c_str(), std::fstream::binary);
        std::cout << "file path >>>> " << req.getLocPath() << std::endl;
        if(!file.is_open())
        {
            //set 403
            std::cout << "file not open" << std::endl;
            exit(1);
        }
        else
            this->isfileopen(true);
    }
    if(this->_isOpen)
    {
        if(this->headerSent == false){
       res = req._res->getStatusCode();
       res.append(req._res->getContentType());
       res.append(req._res->getContentLenght());
       res.append("\r\n");
        this->headerSent = true;  
        }
        
        if(this->headerSent == true && !this->_isDone){
            
            this->file.read(this->buffer, 1024);
            std::streamsize bytesRead = this->file.gcount();
             if(bytesRead > 0)
             { 
                std::string chunk = std::string(this->buffer,bytesRead);
                 res.append(chunk);
				this->isfileopen(true);
            }
            else
            {
                this->bodyisDone(true);
                this->file.close();
                this->isfileopen(false);
        
            }
        }
    }
    
    return res;
}

void response::Send(int sck,request &req)
 {
     if(req._res->_isDone == false){
    std::string res = req._res->FinalString(req);
    std::cout << "res >>>>>>>>> " << res << std::endl;
             send(sck,res.c_str(), res.size(), 0);
     }
}
void    response::set_body(std::string body)
{
    this->_body = body;
}

std::string response::get_body()
{
    return this->_body;
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
            
            index += "<li><a href=\"" + name + "\">" + name + "</a></li>";
        }
        closedir(dir);
    }
    else
    {
        //404
        exit(1);
    }
    this->_autoindex = "HTTP/1.1 200 OK\r\n";
    this->_autoindex += "Content-Type: text/html\r\n";
    this->_autoindex += "Content-Length: " + std::to_string(index.size()) + "\r\n\r\n";
    this->_autoindex += "<html><body><h1>Directory Listing</h1><ul>" + index + "</ul></body></html>";
    this->bodyisDone(true);
}
std::string response::get_autoindex()
{
    return this->_autoindex;
}
