#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

struct Indice
{
    int x, y, z;
};

#include <algorithm>
#include <iostream>
#include <cstring>

// for mmap:
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

const char* map_file(const char* fname, size_t& length);

int main()
{
    size_t length;
    auto f = map_file("resources/MagicRoom.obj", length);
    auto l = f + length;

    std::vector<std::string> lines;
    std::string line;
    while (f && f != l)
    {
        const char* next = static_cast<const char*>(memchr(f, '\n', l - f));
        if (!next)
            next = l;  // Handle case where last line may not end with newline
        line = std::string(f, next);
        f = next + 1;
    }

    std::cout << "m_numLines = " << line << "\n";
}

void handle_error(const char* msg) {
    perror(msg); 
    exit(255);
}

const char* map_file(const char* fname, size_t& length)
{
    int fd = open(fname, O_RDONLY);
    if (fd == -1)
        handle_error("open");

    // obtain file size
    struct stat sb;
    if (fstat(fd, &sb) == -1)
        handle_error("fstat");

    length = sb.st_size;

    const char* addr = static_cast<const char*>(mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0u));
    if (addr == MAP_FAILED)
        handle_error("mmap");

    // TODO close fd at some point in time, call munmap(...)
    return addr;
}

// int main(int argc, char const *argv[])
// {
//     // std::string line = "f 8/1/1 2/1/1 3/2/1 4/2/1";
//     std::string line = "f 3001 3004 3022 123";
//     std::stringstream stream(line);
//     int i,j,k,l;
//     std::vector<Indice> indices_buffer;

//     std::string prefix;
//     char slash;
//     stream >> prefix;
//     // stream >> i >> slash >> j >> slash >> k;
//     // std::cout << "i: " << i << "j: " << j << "k: " << k << std::endl;
//     if (prefix == "f") 
//     {
//         char slash; // To discard the slashes
//         if (stream >> i && stream.ignore(10, ' ') &&
//             stream >> j && stream.ignore(10, ' ') &&
//             stream >> k) 
//         {

//             indices_buffer.push_back({i - 1, j - 1, k - 1});
//             std::cout << "Indice: " << i << ", " << j << ", " << k << std::endl;

//             if (stream.ignore(10, ' ') && stream >> l)
//             {
//                 indices_buffer.push_back({i - 1, k - 1, l - 1});
//                 std::cout << "Indice: " << i << ", " << k << ", " << l << std::endl;
//             }
//         }
//     }
//     return 0;
// }
