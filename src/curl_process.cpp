#include <iostream>
#include <fstream>
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

static const int BUFFER_SIZE = MATRIX_SIZE * MATRIX_SIZE * MATRIX_SIZE * NUM_DIMENSIONS * sizeof(double);

static const int SIZE = int(100 * 100 * 100 * 3);

static const int OFFSET_X = 0;
static const int OFFSET_Y = 1;
static const int OFFSET_Z = 2;

void curl_e_x_add(double *E, double *curl_E);
void curl_e_x_sub(double *E, double *curl_E);

void curl_e_y_add(double *E, double *curl_E);
void curl_e_y_sub(double *E, double *curl_E);

void curl_e_z_add(double *E, double *curl_E);
void curl_e_z_sub(double *E, double *curl_E);

void curl_h_x_add(double *H, double *curl_H);
void curl_h_x_sub(double *H, double *curl_H);

void curl_h_y_add(double *H, double *curl_H);
void curl_h_y_sub(double *H, double *curl_H);

void curl_h_z_add(double *H, double *curl_H);
void curl_h_z_sub(double *H, double *curl_H);

char buffer_[BUFFER_SIZE];

void wait_signal()
{
    std::string msg;
    std::cin >> msg;
    std::cerr << "CPP : Wait Signal End." << std::endl;
}

void ack_signal()
{
    std::cout << "" << std::endl;
}

double *curl_e(double *E)
{
    std::cerr << "CPP : Curl E" << std::endl;
    double *curl_E = new double[SIZE]{};
    curl_e_x_add(E, curl_E);
    std::cerr << "CPP : Curl E X Add" << std::endl;
    curl_e_x_sub(E, curl_E);
    std::cerr << "CPP : Curl E X Sub" << std::endl;
    curl_e_y_add(E, curl_E);
    std::cerr << "CPP : Curl E Y Add" << std::endl;
    curl_e_y_sub(E, curl_E);
    std::cerr << "CPP : Curl E Y Sub" << std::endl;
    curl_e_z_add(E, curl_E);
    std::cerr << "CPP : Curl E Z Add" << std::endl;
    curl_e_z_sub(E, curl_E);
    std::cerr << "CPP : Curl E Z Sub" << std::endl;

    return curl_E;
}

double *curl_h(double *H)
{
    std::cerr << "CPP : Curl H" << std::endl;
    double *curl_H = new double[SIZE]{};
    curl_h_x_add(H, curl_H);
    std::cerr << "CPP : Curl H X Add" << std::endl;
    curl_h_x_sub(H, curl_H);
    std::cerr << "CPP : Curl H X Sub" << std::endl;
    curl_h_y_add(H, curl_H);
    std::cerr << "CPP : Curl H Y Add" << std::endl;
    curl_h_y_sub(H, curl_H);
    std::cerr << "CPP : Curl H Y Sub" << std::endl;
    curl_h_z_add(H, curl_H);
    std::cerr << "CPP : Curl H Z Add" << std::endl;
    curl_h_z_sub(H, curl_H);
    std::cerr << "CPP : Curl H Z Sub" << std::endl;
    return curl_H;
}

void curl_e_x_add(double *E, double *curl_E)
{
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE - 1; j++)
        {
            for (int k = 0; k < MATRIX_SIZE; k++)
            {
                curl_E[int(i * j * k + OFFSET_X)] += E[int(i * (j + 1) * k + OFFSET_Z)] - E[int(i * j * k + OFFSET_Z)];
            }
        }
    }
}
void curl_e_x_sub(double *E, double *curl_E)
{
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            for (int k = 0; k < MATRIX_SIZE - 1; k++)
            {
                curl_E[int(i * j * k + OFFSET_X)] -= E[int(i * j * (k + 1) + OFFSET_Y)] - E[int(i * j * k + OFFSET_Y)];
            }
        }
    }
}

void curl_e_y_add(double *E, double *curl_E)
{
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            for (int k = 0; k < MATRIX_SIZE - 1; k++)
            {
                curl_E[int(i * j * k + OFFSET_Y)] += E[int(i * j * (k + 1) + OFFSET_X)] - E[int(i * j * k + OFFSET_X)];
            }
        }
    }
}

void curl_e_y_sub(double *E, double *curl_E)
{
    for (int i = 0; i < MATRIX_SIZE - 1; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            for (int k = 0; k < MATRIX_SIZE; k++)
            {
                curl_E[int(i * j * k + OFFSET_Y)] -= E[int((i + 1) * j * k + OFFSET_Z)] - E[int(i * j * k + OFFSET_Z)];
            }
        }
    }
}

void curl_e_z_add(double *E, double *curl_E)
{
    for (int i = 0; i < MATRIX_SIZE - 1; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            for (int k = 0; k < MATRIX_SIZE; k++)
            {
                curl_E[int(i * j * k + OFFSET_Z)] += E[int((i + 1) * j * k + OFFSET_Y)] - E[int(i * j * k + OFFSET_Y)];
            }
        }
    }
}

void curl_e_z_sub(double *E, double *curl_E)
{
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE - 1; j++)
        {
            for (int k = 0; k < MATRIX_SIZE; k++)
            {
                curl_E[int(i * j * k + OFFSET_Z)] -= E[int(i * (j + 1) * k + OFFSET_X)] - E[int(i * j * k + OFFSET_X)];
            }
        }
    }
}
void curl_h_x_add(double *H, double *curl_H)
{
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 1; j < MATRIX_SIZE; j++)
        {
            for (int k = 0; k < MATRIX_SIZE; k++)
            {
                curl_H[int(i * j * k + OFFSET_X)] += H[int(i * j * k + OFFSET_Z)] - H[int(i * (j - 1) * k + OFFSET_Z)];
            }
        }
    }
}
void curl_h_x_sub(double *H, double *curl_H)
{
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            for (int k = 1; k < MATRIX_SIZE; k++)
            {
                curl_H[int(i * j * k + OFFSET_X)] -= H[int(i * j * k + OFFSET_Y)] - H[int(i * j * (k - 1) + OFFSET_Y)];
            }
        }
    }
}

void curl_h_y_add(double *H, double *curl_H)
{
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            for (int k = 1; k < MATRIX_SIZE; k++)
            {
                curl_H[int(i * j * k + OFFSET_Y)] += H[int(i * j * k + OFFSET_X)] - H[int(i * j * (k - 1) + OFFSET_X)];
            }
        }
    }
}

void curl_h_y_sub(double *H, double *curl_H)
{
    for (int i = 1; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            for (int k = 0; k < MATRIX_SIZE; k++)
            {
                curl_H[int(i * j * k + OFFSET_Y)] -= H[int(i * j * k + OFFSET_Z)] - H[int((i - 1) * j * k + OFFSET_Z)];
            }
        }
    }
}

void curl_h_z_add(double *H, double *curl_H)
{
    for (int i = 1; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            for (int k = 0; k < MATRIX_SIZE; k++)
            {
                curl_H[int(i * j * k + OFFSET_Z)] += H[int(i * j * k + OFFSET_Y)] - H[int((i - 1) * j * k + OFFSET_Y)];
            }
        }
    }
}

void curl_h_z_sub(double *H, double *curl_H)
{
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 1; j < MATRIX_SIZE; j++)
        {
            for (int k = 0; k < MATRIX_SIZE; k++)
            {
                curl_H[int(i * j * k + OFFSET_Z)] -= H[int(i * j * k + OFFSET_X)] - H[int(i * (j - 1) * k + OFFSET_X)];
            }
        }
    }
}
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "CPP ERROR : no shared file provided as argv[1]" << std::endl;
        return -1;
    }
    wait_signal();

    memset(buffer_, 0, BUFFER_SIZE);
    FILE *shm_f = fopen(argv[1], "w");
    fwrite(buffer_, sizeof(char), BUFFER_SIZE, shm_f);
    fclose(shm_f);

    std::cerr << "CPP:  File ready." << std::endl;
    ack_signal();

    int shm_fd = open(argv[1], O_RDWR);
    void *shm_mmap = mmap(NULL, BUFFER_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
    close(shm_fd);

    if (shm_mmap == MAP_FAILED)
    {
        std::cerr << "ERROR SHM\n";
        perror(NULL);
        return -1;
    }

    double *mtx = (double *)shm_mmap;

    while (true)
    {
        wait_signal();
        mtx = curl_h(mtx);
        std::cerr << "CPP: Curl H done.\n"
                  << std::endl;
        ack_signal();

        wait_signal();
        mtx = curl_e(mtx);
        std::cerr << "CPP: Curl E done.\n"
                  << std::endl;
        ack_signal();
    }

    munmap(shm_mmap, BUFFER_SIZE);
    return 0;
}