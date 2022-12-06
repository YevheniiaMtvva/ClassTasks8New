#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

void * thread_function(void * thread_arg) { //Неотменяемый поток
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL); //Делаем поток неотменяемым
    
    int i, n = *(int*) thread_arg;

    for (i = 0; i < 2 * n; i++)
    {
        fprintf(stderr, "\e[1;1H\e[2J"); //Чистим консольку для удобства
        fprintf(stderr, "%d\n", i); //Выводим счётчик

        sleep(1);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 2) //Если аргументов в командной строке мало то ошибка
	{
		fprintf(stderr, "Too few arguments\n");
		return 1;
	}
    int n = atoi(argv[1]); //Считываем то что написали в консольке

    void * result;
    pthread_t thread;

    pthread_create(&thread, NULL, &thread_function, &n); //Создаем поток

    sleep(n); //Спим столько секунд сколько написали
    pthread_cancel(thread); //Затем отменяем поток

    if (!pthread_equal(pthread_self(), thread)) //Так надо потому что было в примере (если несколько потоков сделали проверка на именно тот который нужен нам)
    {
        pthread_join(thread, &result) ;
    }

    if (result == PTHREAD_CANCELED) //Если поток был отменен то написать шо отменен, иначе написать шо дефолтно закончилось
    {
        fprintf(stderr, "Canceled\n");
    }
    else
    {
        fprintf(stderr, "Default\n");
    }
    
    return 0;
}