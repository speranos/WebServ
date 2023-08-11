#include "response.hpp"

// void   response::GetMethod(location_obj &location,request &req)
// {
//     DIR *dir = opendir(req.getLocPath().c_str());
//      if(dir)
//      {
//         if(location.get_index().empty()) 
            
//             if(location.get_cgi() && !access(req.getLocPath().c_str(),X_OK))
//             {

//                 //  send cgi
//             }
//             else if(!access(req.getLocPath().c_str(),R_OK))
//              {
//                 //send index;
//                 req._res->SetStatusCode("HTTP/1.1 200 OK\r\n");
//                 req._res->set_get_con_type(req);
//                 req._res->setContentLenght(req);
//              }   
//          else if(location.get_auto_index())
//          {
//             //send autoindex
//             req._res->SetStatusCode("HTTP/1.1 200 OK\r\n");
//          }
//         else
//         {
//             closedir(dir);
//             //send 403;
//             req._res->SetStatusCode("HTTP/1.1 403 Forbidden\r\n");
//         }
//      }
//     if(access(req.getLocPath().c_str(),F_OK) == 0)
//     {
//         if(!access(req.getLocPath().c_str(),R_OK))
//         {
//             //send file
//             req._res->SetStatusCode("HTTP/1.1 200 OK\r\n");
//             req._res->set_get_con_type(req);
//             req._res->setContentLenght(req);
//         }
//         else
//         {
//             // send 404
//             req._res->SetStatusCode("HTTP/1.1 404 Not Found\r\n");
//         }
//     }
        
// }

// void    SendResponse(int sck,request &req)
// {
//   std::map<std::string, std::string> con_type = req.getHeaders();
//   std::string Content_type =  con_type['Contnt-type'];
// }

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
   // std::cout << "file is open : " << this->_isOpen << std::endl;
    if(this->_isOpen)
    {
       // std::cout << "headerSent===============>"<< this->headerSent << std::endl;
        if(this->headerSent == false){
       res = req._res->getStatusCode();
       res.append(req._res->getContentType());
       res.append(req._res->getContentLenght());
       res.append("\r\n");
        this->headerSent = true;  
        //std::cout << "headersent ============> : " << this->headerSent << std::endl;  
        }
        
        if(this->headerSent == true && !this->_isDone){
            
            this->file.read(this->buffer, 1024);
           // std::cout << "-------------------------------------------------------------------" << std::endl;
            // std::cout << "buffer : " << this->buffer << std::endl;
           // std::cout << "-------------------------------------------------------------------" << std::endl;
            std::streamsize bytesRead = this->file.gcount();

            std::cout << "bytesRead >>>>>>>>>>> " << bytesRead << std::endl;
             if(bytesRead > 0)
             { 
                std::string chunk = std::string(this->buffer,bytesRead);
                // std::cout << "chunk size : " << chunk.size() << std::endl;
                // std::cout << chunk << std::endl;
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
    std::cout << "HIIII\n";
    // std::cout << res << std::endl;
    return res;
}

void response::Send(int sck,request &req)
 {
    // std::cout << "body done ::" << req._res->_isDone << std::endl;
     if(req._res->_isDone == false){
    std::string res = req._res->FinalString(req);
            // std::cout << "res : " << res << std::endl;
            int bytes_sent = send(sck,res.c_str(), res.size(), 0);
            std::cout << "send to >>>>>>>>>> " << sck << std::endl;
            std::cout << "len is : " << bytes_sent << std::endl;
            if (bytes_sent == -1)
                exit(69);
            // while ((size_t)bytes_sent <  res.size())
            // {
            //     std::cout << "send again " << std::endl;
            //     res = res.substr(bytes_sent, 0);
            //     bytes_sent = send(sck,res.c_str(), res.size(), 0);
            // }
           }
           
        //    free(this->buffer);
   
   
}















// void    SendResponse(int sck,request &req)
// {
//     std::string res = req._res->getStatusCode();;
//     res += req._res->getContentType();
//     res += req._res->get_Content_Lenght();
//     res += "\r\n";
//     send(sck,res.c_str(),res.size(),0);
// }
// {
   
   

//     send(sck,res.c_str(),res.size(),0);

// }

//      std::string htmlContent =  "<!DOCTYPE html>\r\n"
//                                 "<html>\r\n"
//                                 "<head>\r\n"
//                                   "<title>Welcome to My Website</title>\r\n"
//                                 "</head>\r\n"
//                                 "<body>\r\n"
//                                   "<h1>Welcome to My Website</h1>\r\n"
//                                   "<p>Thank you for visiting our site!</p>\r\n"
//                                 "</body>\r\n"
//                                 "</html>\r\n";
//  std::string res = "HTTP/1.1 200 OK\r\n"
//                       "Content-Type: text/html\r\n"
//                       "Content-Length: " + std::to_string(htmlContent.size()) + "\r\n\r\n" + htmlContent;
//             //(void)sck;}
//             std::cout << res << std::endl;