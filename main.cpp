#include <iostream>
#include <pthread.h>
#include <string>

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
    int *fst, *scnd;
    int size;
} arguments;

void *sm(void *input)
{
    int *ms1 = ((arguments *)input)->fst;
    int *ms2 = ((arguments *)input)->scnd;
    int size = ((arguments *)input)->size;

    for (int i = 0; i < size; ++i)
    {
        ms1[i] += ms2[i];
        ms2[i] = 0;
    }
    pthread_exit(0);
}

void thread_work(int *data, int threads_count, int count_vectors, int ln_of_one_vector)
{
    pthread_t threads[threads_count];
    int step = ln_of_one_vector, created = 0, mass = count_vectors;

    while (mass > 1)
    {
        int ms1[ln_of_one_vector], ms2[ln_of_one_vector];

        int num_mass = mass / 2;

        for (int i = 0; i < num_mass; i += 2 * step)
        {

            for (int j = i; j < i + ln_of_one_vector; ++j)
            {
                ms1[j - i] = data[j];
            }
            for (int k = i + step; k < i + step + ln_of_one_vector; ++k)
            {
                ms2[k - i - step] = data[k];
            }

            printmass(ms1, ln_of_one_vector, ln_of_one_vector);
            printmass(ms2, ln_of_one_vector, ln_of_one_vector);

            arguments *arg = (arguments *)malloc(sizeof(arguments));
            arg->fst = ms1;
            arg->scnd = ms2;
            arg->size = ln_of_one_vector;

            pthread_create(&threads[created], NULL, sm, arg);
            ++created;
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

    for (int i = 0; i < threads_count; ++i)
    {
        pthread_join(threads[i], NULL);
    }
}

int main(int argc, char *argv[])
{
    int count_vectors, ln_of_one_vector, count_threads;
    // std::cout << "How many arrays would you like to operate with?" << std::endl;
    // std::cin >> count_vectors;
    // std::cout << "What size of vector would you like?" << std::endl;
    // std::cin >> ln_of_one_vector;
    // std::cout << "How many threads would you like?" << std::endl;
    // std::cin >> count_threads;

    count_vectors = 3;
    ln_of_one_vector = 3;
    count_threads = 3;

    int data[count_vectors * ln_of_one_vector];

    for (int i = 0; i < count_vectors * ln_of_one_vector; ++i)
    {
        data[i] = rand() % 10;
    }

    printmass(data, count_vectors * ln_of_one_vector, ln_of_one_vector);

    thread_work(data, count_threads, count_vectors, ln_of_one_vector);

    printmass(data, count_vectors * ln_of_one_vector, ln_of_one_vector);
}