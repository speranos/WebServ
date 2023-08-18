#include "../response.hpp"

bool fileExists(const char* path) {
    struct stat fileInfo;
    return (stat(path, &fileInfo) == 0);
}


bool writePerm(const char* filename)
{
    struct stat fileStat;
    if (stat(filename, &fileStat) != 0)
        return false;
    return (fileStat.st_mode & S_IWUSR) != 0;
}

void response::methodDelete(request& req){
        std::string myLocation = req.getLocPath();
        struct stat fileInfo;
        // Check if it's a directory
        if (stat(myLocation.c_str(), &fileInfo) == 0)
        {
            // if yes
            if (S_ISDIR(fileInfo.st_mode))
            {
                if (myLocation[myLocation.size() - 1] != '/'){
                                req.statuscode = 409;
                            req.SetErrorStatusCode(409);
                            req.setStatusCodePath(req);
                            req.op = 4;
                }
                else{
                                req.statuscode = 403;
                                req.SetErrorStatusCode(403);
                                req.setStatusCodePath(req);
                                req.op = 4;
                }
                    
            }
            // if it's a file
            else
            {
                // Check if the file you want to delete exists be3da
                if (fileExists(myLocation.c_str()))
                {
                    // Check if it has write permission
                    if (writePerm(myLocation.c_str()))
                    {
                        // Delete the file
                        int deleteResult = std::remove(myLocation.c_str());
                        if (deleteResult == 0)
                            // Successfully deleted
                            std::cout << "204 No content" << std::endl;
                        else{
                                        req.statuscode = 500;
                                    req.SetErrorStatusCode(500);
                                    req.setStatusCodePath(req);
                                    req.op = 4;
                        }
                    }
                    else{
                                    req.statuscode = 403;
                            req.SetErrorStatusCode(403);
                            req.setStatusCodePath(req);
                            req.op = 4;
                    }
                }
                else
                {
                        req.statuscode = 404;
                        req.SetErrorStatusCode(404);
                        req.setStatusCodePath(req);
                        req.op = 4;
                }
            }
        }
}
