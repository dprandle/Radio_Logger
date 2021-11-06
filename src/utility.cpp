#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <fstream>
#include <sstream>

#include "logger.h"
#include "utility.h"
#include "timer.h"

namespace util
{
static std::string locked_str;

std::string & to_lower(std::string & s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}

bool save_data_to_file(uint8_t * data, uint32_t size, const char * fname, int mode_flags)
{
    int fd = open(fname, O_RDWR | O_CREAT, mode_flags);
    if (fd != -1)
    {
        ilog("Saving {} bytes to {}", size, fname);
        write(fd, data, size);
        close(fd);
        return true;
    }
    else
    {
        wlog("Unable to open file {} - error: {}", fname, strerror(errno));
        return false;
    }
    return 0;
}

bool read_file_contents_to_string(const std::string & fname, std::string & contents)
{
    std::stringstream buffer;

    std::ifstream output;
    output.open(fname, std::ios::in);
    if (!output.is_open())
        return false;

    output.seekg(0, std::ios::end);
    size_t size = output.tellg();
    contents.resize(size);
    output.seekg(0);
    output.read(&contents[0], size);
    output.close();
    return true;
}

void delay(double ms)
{
    Timer t;
    t.start();
    while ((t.elapsed() * 1000.0) < ms)
        t.update();
}

std::string get_current_date_string()
{
    time_t t = time(nullptr);
    tm * ltm = localtime(&t);
    return std::to_string(1900+ltm->tm_year) + "-" + std::to_string(1 + ltm->tm_mon) + "-" + std::to_string(ltm->tm_mday);
}

std::string get_current_time_string()
{
    time_t t = time(nullptr);
    tm * ltm = localtime(&t);
    return std::to_string(ltm->tm_hour) + ":" + std::to_string(ltm->tm_min) + ":" + std::to_string(ltm->tm_sec);
}

uint16_t files_in_dir(const char * dirname)
{
    uint16_t count = 0;
    DIR * dir = opendir(dirname);
    dirent * ent;
    if (dir)
    {
        while ((ent = readdir(dir)))
        {
            if (ent->d_type == DT_REG)
                ++count;
        }
        closedir(dir);
    }
    else
    {
        wlog("Could not open directory {}",dirname);
    }
    return count;
}

uint16_t filenames_in_dir(const char * dirname, char ** & buffer)
{
    uint16_t file_cnt = files_in_dir(dirname);
    if (file_cnt == 0)
        return file_cnt;
    
    DIR * dir = opendir(dirname);
    dirent * ent;
    buffer = (char**)malloc(file_cnt * sizeof(char*));
    
    uint16_t ind = 0;
    while ((ent = readdir(dir)))
    {    
        if (ent->d_type == DT_REG)
        {
            uint32_t name_len = strlen(ent->d_name);
            buffer[ind] = (char*)malloc(strlen(ent->d_name) + 1);
            strcpy(buffer[ind],ent->d_name);
            ++ind;
        }
    }
    closedir(dir);
    return file_cnt;
}

uint32_t hash_id(const char * str)
{
    uint32_t hash = 5381;
    int32_t c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

} // namespace util