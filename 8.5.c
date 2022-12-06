#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

static pthread_key_t thread_key; //Ключ потоковых данных

void write_to_thread(const int number, const int counter)
{
    char * str = (char*) pthread_getspecific(thread_key);
    fprintf(stderr, "%d -- %s: %d\n", counter, str, number);
}

void * thread_function(void * arg) //Заполняем массив
{
	int n = *(int*) arg; //Принимаем целочисленное значение из аргумента

    char * str = "This is static part of the message. Random number is";
    pthread_setspecific(thread_key, str); //Кидаем в потоковые данные строку которую инициализировали выше

    int rand_number = rand() % (n + 6); //Генерируем случайное число из диапазона от 0 до номера этого потока + 5

    write_to_thread(rand_number, n + 1);

    sleep(1); //Спим столько секунд сколько необходимо - условие

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
    int n = atoi(argv[1]); //Считываем то что написали в консольке

    srand(time(NULL));

    pthread_key_create(&thread_key, NULL); //Создаем ключ для потоков
    create_threads(n); //Создаем потоки через функцию - условие
    
    return 0;
}