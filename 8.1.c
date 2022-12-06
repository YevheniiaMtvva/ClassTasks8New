#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

int * arr; //Глобальный массив - условие

void * attr_thread_function(void * thread_arg) { //Выводим постоянно элементы
    int i, n = *(int*) thread_arg;

    while(1)
    {
        sleep(1); //Спим 1 секунду - условие

        for (i = 0; i < n; i++) //Выводим массив
        {
            fprintf(stderr, "%d ", arr[i]);
        }

        fprintf(stderr, "\n");

        if (arr[n - 1]) //Если массив заполнен - выйти из цикла, закончить поток. Условие
        {
            break;
        }
    }
    
    return NULL;
}

void * thread_function(void * arg) //Заполняем массив
{
	int n = *(int*) arg; //Принимаем целочисленное значение из аргумента

	sleep(n); //Спим столько секунд сколько необходимо - условие
    arr[n] = rand() % 10 + 1; //Рандомное число от 1 до 10 - условие

    sleep(arr[n]); //Спим столько секунд, сколько нарандомили - условие

	return NULL;
}

int create_threads(const int n) //Функция создания потоков
{
    int i;
    pthread_t * threads = malloc(sizeof(pthread_t) * n); //Выделяем память потокам
    
    for (i = 0; i < n; i++)
    {
        if(pthread_create(&threads[i], NULL, &thread_function, &i) != 0) //Создаем столько потоков сколько надо
	    {
		    fprintf(stderr, "Error\n");
		    return 1;
	    }
    }

    for (i = 0; i < n; i++)
    {
        if(pthread_join(threads[i], NULL) != 0) //Ждём завершения каждого созданного потока
	    {
		    fprintf(stderr, "Error\n");
		    return 1;
	    }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) //Если аргументов в командной строке мало то ошибка
	{
		fprintf(stderr, "Too few arguments\n");
		return 1;
	}
    int i, n = atoi(argv[1]); //Считываем то что написали в консольке
    arr = malloc(sizeof(int) * n); //Выделяем массиву память

    srand(time(NULL));

    pthread_attr_t attr; //Инициализируем атрибут
    pthread_t attr_thread; //Поток с атрибутом

    pthread_attr_init (&attr); //Инициализирует атрибут
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); //Говорим шо он (атрибут) делает поток отсоединенным

    pthread_create(&attr_thread, &attr, &attr_thread_function, &n); //Создаем отсоединенный поток (не нужно ждать его завершения в мейне)
    create_threads(n); //Создаем потоки через функцию - условие

    pthread_attr_destroy (&attr); //Уничтожаем атрибут
    
    return 0;
}