#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

int counter = 0; //Счётчик

void cancel_function(void * buffer) //Функция которая вызывается при отмене или окончании потока
{
    fprintf(stderr, "Array was canceled on: %d iteration.\n", *(int*) buffer);
}

void * thread_function() { //Бесконечный поток
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); //Делаем поток асинхронным

    void * temp_buffer = &counter;
    pthread_cleanup_push(cancel_function, temp_buffer); //Регистрируем в обработчик функцию, закидывая в неё буффер, который ссылается на адресс счётчика.

    while(1)
    {
        fprintf(stderr, "\e[1;1H\e[2J"); //Чистим консольку для удобства
        fprintf(stderr, "%d\n", counter); //Выводим счётчик

        counter++;
        sleep(1);
    }

    pthread_cleanup_pop(0); //Удаляем раннее зарегестрированную функцию

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

    pthread_create(&thread, NULL, &thread_function, NULL); //Создаем поток

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