#include <iostream>
#include <pthread.h>
#include <string>
#include <vector>

typedef struct
{
    std::vector<int> &fst, &scnd;
    int size;
} arguments;

void *sm(std::vector<int> &ms1, std::vector<int> &ms2, int size)
{
    for (int i = 0; i < size; ++i)
    {
        ms1[i] += ms2[i];
        ms2[i] = 0;
    }
}

void thread_work(std::vector<std::vector<int>> &data, int threads_count, int count_vectors, int ln_of_one_vector)
{
    pthread_t threads[threads_count];
    int step = 1, created = 0, i = 0;
    while (count_vectors > step && created < threads_count)
    {
        for (int i = 0; i < count_vectors; i += 2 * step)
        {
            pthread_create(&threads[created], NULL, sm, data[i], data[i + 1], ln_of_one_vector);
        }
        // if (count_vectors % 2 == 0)
        // {
        //     count_vectors /= 2;
        // }
        // else
        // {
        //     count_vectors /= 2;
        //     ++count_vectors;
        // }
        step *= 2;
    }
}

int main(int argc, char *argv[])
{
    int count_vectors, ln_of_one_vector;
    std::cout << "How many arrays would you like to operate with?" << std::endl;
    std::cin >> count_vectors;
    std::cout << "What size of vector would you like?" << std::endl;
    std::cin >> ln_of_one_vector;

    std::vector<std::vector<int>> data(count_vectors, std::vector<int>(ln_of_one_vector));

    for (int i = 0; i < count_vectors; ++i)
    {
        for (int j = 0; j < ln_of_one_vector; ++j)
        {
            data[i][j] = rand() % 100;
        }
    }

    int count_threads = atoi(argv[1]);

    thread_work(data, count_threads, count_vectors, ln_of_one_vector);
}