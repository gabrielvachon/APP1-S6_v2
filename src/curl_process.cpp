#include <iostream>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


static const int NUM_DIMENSIONS = 3;
static const int MATRIX_SIZE = 100;
static const int BUFFER_SIZE = MATRIX_SIZE * MATRIX_SIZE * MATRIX_SIZE * NUM_DIMENSIONS *  sizeof(double);

char buffer_[BUFFER_SIZE];

void wait_signal()
{
    std::string msg;
    std::cin >> msg;
    std::cerr << "CPP : Starting processing." << std::endl;
}

void ack_signal()
{
    std::cout << "" << std::endl;
}

void curl_h()
{

}

void curl_e()
{

}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "CPP ERROR : no shared file provided as argv[1]" << std::endl;
        return -1;
    }
    wait_signal();
    
    memset(buffer_, 0, BUFFER_SIZE);
    FILE* shm_f = fopen(argv[1], "w");
    fwrite(buffer_, sizeof(char), BUFFER_SIZE, shm_f);
    fclose(shm_f);

    std::cerr << "CPP: File ready." << std::endl;

    ack_signal();

    int shm_fd = open(argv[1], O_RDWR);
    void* shm_mmap = mmap(NULL, BUFFER_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
    close(shm_fd);

    if (shm_mmap == MAP_FAILED) {
        std::cerr << "CPP ERROR : SHM\n";
        perror(NULL);
        return -1;
    }

    double* mtx = (double*)shm_mmap;

    while (true) {
        wait_signal();
        curl_h();
        std::cerr << "CPP: Curl H done.\n" << std::endl;
        ack_signal();

        wait_signal();
        curl_e();
        std::cerr << "CPP: Curl E done.\n" << std::endl;
        ack_signal();     
    }

    munmap(shm_mmap, BUFFER_SIZE);
    return 0;
}