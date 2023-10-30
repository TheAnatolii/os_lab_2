#include <iostream>
#include <pthread.h>
#include <string>
#include <chrono>

void printmass(int *ms, int size, int ln_of_one_vector)
{
    for (int i = 0; i < size; ++i)
    {
        if (i % ln_of_one_vector == 0)
        {
            std::cout << std::endl;
        }
        std::cout << ms[i] << " ";
    }
    std::cout << std::endl;
}

typedef struct
{
    int *arr;
    int start, step;
    int size;
} arguments;

void *sm(void *input)
{
    int *arr = ((arguments *)input)->arr;
    int start = ((arguments *)input)->start;
    int step = ((arguments *)input)->step;
    int size = ((arguments *)input)->size;

    for (int i = start; i < start + size; ++i)
    {
        arr[i] += arr[i + step];
        arr[i + step] = 0;
    }
    pthread_exit(0);
}

void thread_work(int *data, int threads_count, int count_vectors, int ln_of_one_vector)
{
    pthread_t threads[threads_count];
    int step = ln_of_one_vector, created = 0, mass = count_vectors;

    while (mass > 1)
    {

        int num_mass = mass / 2;
        int pos = 0, i = 0;

        for (int i = 0; i < num_mass; i++)
        {

            arguments *arg = (arguments *)malloc(sizeof(arguments));
            arg->arr = data;
            arg->start = pos;
            arg->step = step;
            arg->size = ln_of_one_vector;

            pthread_create(&threads[created], NULL, sm, arg);
            ++created;

            pos += 2 * step;
            if (created == threads_count)
            {
                for (int q = 0; q < created; ++q)
                {
                    pthread_join(threads[q], NULL);
                }
                created = 0;
            }
        }

        for (int q = 0; q < created; ++q)
        {
            pthread_join(threads[q], NULL);
        }

        if (mass % 2 == 0)
        {
            mass /= 2;
        }
        else
        {
            mass /= 2;
            ++mass;
        }
        step *= 2;
    }
}

int main(int argc, char *argv[])
{
    int count_vectors, ln_of_one_vector, count_threads;
    std::cout << "How many arrays would you like to operate with?" << std::endl;
    std::cin >> count_vectors;
    std::cout << "What size of vector would you like?" << std::endl;
    std::cin >> ln_of_one_vector;
    std::cout << "How many threads would you like?" << std::endl;
    std::cin >> count_threads;

    int data[count_vectors * ln_of_one_vector];

    for (int i = 0; i < count_vectors * ln_of_one_vector; ++i)
    {
        data[i] = rand() % 10;
    }

    printmass(data, count_vectors * ln_of_one_vector, ln_of_one_vector);
    auto start = std::chrono::high_resolution_clock::now();
    thread_work(data, count_threads, count_vectors, ln_of_one_vector);
    auto end = std::chrono::high_resolution_clock::now();
    printmass(data, ln_of_one_vector, ln_of_one_vector);
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Программа сработала за " << duration.count() << " microseconds" << std::endl;
}