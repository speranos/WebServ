#include "../response.hpp"

bool canUploadToPath(const char *location)
{
    if (access(location, F_OK) == 0)
    {
        // Check read permissions
        if (access(location, R_OK) == 0)
        {
            // std::cout << "location exists and has read permission." << std::endl;
            // Check write permissions
            if (access(location, W_OK) == 0);
            else{
                std::cerr << "location does not have write permission." << std::endl;
                return false;
            }

            // Check execute permissions
            if (access(location, X_OK) == 0);
            else{
                std::cerr << "location does not have execute permission." << std::endl;
                return false;
            }
        }
        else
            std::cerr << "location exists but does not have read permission." << std::endl;
        return true;
    }
    else
    {
        std::cerr << "location does not exist." << std::endl;
        return false;
    }
}

bool isDirectory(const char *path)
{
    struct stat path_stat;

    if (stat(path, &path_stat) != 0)
    {
        std::cerr << "Error getting path information." << std::endl;
        return false;
    }

    return S_ISDIR(path_stat.st_mode);
}

int hasIndexFiles(const char *dirPath)
{
    DIR *dir = opendir(dirPath);
    if (!dir)
    {
        std::cerr << "Error opening directory." << std::endl;
        closedir(dir);
        return false;
    }
    int hasIndex = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
        hasIndex++;
    closedir(dir);
    return hasIndex;
}

void response::MethodPost(request &req)
{
    if(req.getFlag() == true){
        // cgi
        // std::cout << "dasdas" << std::endl;
        if (req.getLoc().get_cgi() == true)
        {

            std::string a = req._res->cgi_exec(req);
            // std::cout << "cgi results" << a << std::endl;
            req.op = 1;
        }
        else
        {
            req.statuscode = 403;
            req.SetErrorStatusCode(403);
            req.setStatusCodePath(req);
            req.op = 4;
        }
    }
    std::string upload_path = req._loc.get_upload();
    if (canUploadToPath(upload_path.c_str()))
    {
        // std::cout << "**********upload path >> " << upload_path << std::endl;
        if (!system(("mv " + req.getBody() + " " + upload_path).c_str()))
        {
            req.op = 7;
        }
    }
    else
    {
        std::ofstream outFile(upload_path.c_str());
        if (!outFile)
        {
            req.statuscode = 404;
            req.SetErrorStatusCode(404);
            req.setStatusCodePath(req);
            req.op = 4;
        }
        else
        {
            if (isDirectory(upload_path.c_str()))
            {
                // directory
                // if (req._uri.find_last_of("/") + 1){
                //     // req._uri += "/";
                //     req.op = 5;
                // }
                // else
                // {
                    if (!req.getLoc().get_index().empty())
                    {
                        if (req.getLoc().get_cgi() == true)
                        {

                            // std::cout << "cgi results" << std::endl;
                            std::string a = req._res->cgi_exec(req);
                            req.op = 1;
                        }
                        else
                        {
                            req.statuscode = 403;
                            req.SetErrorStatusCode(403);
                            req.setStatusCodePath(req);
                            req.op = 4;
                        }
                    }
                    else
                    {
                        req.statuscode = 403;
                        req.SetErrorStatusCode(403);
                        req.setStatusCodePath(req);
                        req.op = 4;
                    }
                // }
            }
            else
            {
                // file
                // std::cout << "******************************" << std::endl;
                if (req.getLoc().get_cgi() == true)
                {
                    std::string a = req._res->cgi_exec(req);
                    // std::cout << "cgi results" << a << std::endl;
                    req.op = 1;
                }
                else
                {
                    req.statuscode = 403;
                    req.SetErrorStatusCode(403);
                    req.setStatusCodePath(req);
                    req.op = 4;
                }
            }
        }
    }
}