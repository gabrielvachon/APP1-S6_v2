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
#include <thread>

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

void copy_matrix(double *mtx, double *other_mtx, int start, int end);

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
    std::thread thread_x_add(curl_e_x_add, E, curl_E);
    std::thread thread_x_sub(curl_e_x_sub, E, curl_E);
    std::thread thread_y_add(curl_e_y_add, E, curl_E);
    std::thread thread_y_sub(curl_e_y_sub, E, curl_E);
    std::thread thread_z_add(curl_e_z_add, E, curl_E);
    std::thread thread_z_sub(curl_e_z_sub, E, curl_E);
    thread_x_add.join();
    thread_x_sub.join();
    thread_y_add.join();
    thread_y_sub.join();
    thread_z_add.join();
    thread_z_sub.join();
    return curl_E;
}

double *curl_h(double *H)
{
    std::cerr << "CPP : Curl H" << std::endl;
    double *curl_H = new double[SIZE]{};
    std::thread thread_x_add(curl_h_x_add, H, curl_H);
    std::thread thread_x_sub(curl_h_x_sub, H, curl_H);
    std::thread thread_y_add(curl_h_y_add, H, curl_H);
    std::thread thread_y_sub(curl_h_y_sub, H, curl_H);
    std::thread thread_z_add(curl_h_z_add, H, curl_H);
    std::thread thread_z_sub(curl_h_z_sub, H, curl_H);
    thread_x_add.join();
    thread_x_sub.join();
    thread_y_add.join();
    thread_y_sub.join();
    thread_z_add.join();
    thread_z_sub.join();
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
                int o_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_X;
                int l_index = ((i + 0) * 30000) + ((j + 1) * 300) + ((k + 0) * 3) + OFFSET_Z;
                int r_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_Z;
                curl_E[o_index] += E[l_index] - E[r_index];
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
                int o_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_X;
                int l_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 1) * 3) + OFFSET_Y;
                int r_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_Y;
                curl_E[o_index] -= E[l_index] - E[r_index];
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
                int o_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_Y;
                int l_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 1) * 3) + OFFSET_X;
                int r_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_X;
                curl_E[o_index] += E[l_index] - E[r_index];
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
                int o_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_Y;
                int l_index = ((i + 1) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_Z;
                int r_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_Z;
                curl_E[o_index] -= E[l_index] - E[r_index];
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
                int o_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_Z;
                int l_index = ((i + 1) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_Y;
                int r_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_Y;
                curl_E[o_index] += E[l_index] - E[r_index];
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
                int o_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_Z;
                int l_index = ((i + 0) * 30000) + ((j + 1) * 300) + ((k + 0) * 3) + OFFSET_X;
                int r_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_X;
                curl_E[o_index] -= E[l_index] - E[r_index];
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
                int o_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_X;
                int l_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_Z;
                int r_index = ((i + 0) * 30000) + ((j - 1) * 300) + ((k + 0) * 3) + OFFSET_Z;
                curl_H[o_index] += H[l_index] - H[r_index];
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
                int o_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_X;
                int l_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_Y;
                int r_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k - 1) * 3) + OFFSET_Y;
                curl_H[o_index] -= H[l_index] - H[r_index];
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
                int o_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_Y;
                int l_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_X;
                int r_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k - 1) * 3) + OFFSET_X;
                curl_H[o_index] += H[l_index] - H[r_index];
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
                int o_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_Y;
                int l_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_Z;
                int r_index = ((i - 1) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_Z;
                curl_H[o_index] -= H[l_index] - H[r_index];
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
                int o_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_Z;
                int l_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_Y;
                int r_index = ((i - 1) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_Y;
                curl_H[o_index] += H[l_index] - H[r_index];
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
                int o_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_Z;
                int l_index = ((i + 0) * 30000) + ((j + 0) * 300) + ((k + 0) * 3) + OFFSET_X;
                int r_index = ((i + 0) * 30000) + ((j - 1) * 300) + ((k + 0) * 3) + OFFSET_X;
                curl_H[o_index] -= H[l_index] - H[r_index];
            }
        }
    }
}

void copy_matrix(double *mtx, double *mtx_to_copy, int start, int end)
{
    for (int i = start; i < end; i++)
    {
        mtx[i] = mtx_to_copy[i];
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
        double *curl_H = curl_h(mtx);

        std::thread thread_copy_1(copy_matrix, mtx, curl_H, 0, 500000);
        std::thread thread_copy_2(copy_matrix, mtx, curl_H, 500000, 1000000);
        std::thread thread_copy_3(copy_matrix, mtx, curl_H, 1000000, 1500000);
        std::thread thread_copy_4(copy_matrix, mtx, curl_H, 1500000, 2000000);
        std::thread thread_copy_5(copy_matrix, mtx, curl_H, 2000000, 2500000);
        std::thread thread_copy_6(copy_matrix, mtx, curl_H, 2500000, 3000000);

        thread_copy_1.join();
        thread_copy_2.join();
        thread_copy_3.join();
        thread_copy_4.join();
        thread_copy_5.join();
        thread_copy_6.join();

        delete curl_H;

        std::cerr << "CPP: Curl H done."
                  << std::endl;
        ack_signal();

        wait_signal();
        double *curl_E = curl_e(mtx);

        std::thread thread_copy_7(copy_matrix, mtx, curl_E, 0, 500000);
        std::thread thread_copy_8(copy_matrix, mtx, curl_E, 500000, 1000000);
        std::thread thread_copy_9(copy_matrix, mtx, curl_E, 1000000, 1500000);
        std::thread thread_copy_10(copy_matrix, mtx, curl_E, 1500000, 2000000);
        std::thread thread_copy_11(copy_matrix, mtx, curl_E, 2000000, 2500000);
        std::thread thread_copy_12(copy_matrix, mtx, curl_E, 2500000, 3000000);

        thread_copy_7.join();
        thread_copy_8.join();
        thread_copy_9.join();
        thread_copy_10.join();
        thread_copy_11.join();
        thread_copy_12.join();

        delete curl_E;

        std::cerr << "CPP: Curl E done."
                  << std::endl;
        ack_signal();
    }

    munmap(shm_mmap, BUFFER_SIZE);
    return 0;
}