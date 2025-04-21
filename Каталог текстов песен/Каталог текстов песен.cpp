#include <iostream>
#include <string.h>
#include <fstream>


//#define DEBUG


using namespace std;

const int MAX_LINES = 100;
const int MAX_LENGTH = 256;

struct Composer
{
    char name[20];
    char surname[20];
};

struct Lyrics
{
    char lines[MAX_LINES][MAX_LENGTH];
    int lineCounter = 0;
};

struct Song
{
    int songID = 0;
    Composer composer;
    char songName[100];
    Lyrics lyrics;
    char performer[100];
    char year[15];
};

struct SongList
{
    Song* songs;
    int listSize = 10;
    int numberOfSongs = 0;
};

// заполнение текста ручнымм вводом
void fillLyrics(Song& song)
{
    cout << "Введите текст песни (когда текст введен - напечатайте \"end\" и нажмите клавишу \"Enter\"): ";
    while (song.lyrics.lineCounter < MAX_LINES)
    {
        cin.getline(song.lyrics.lines[song.lyrics.lineCounter], MAX_LENGTH);
        if (strcmp(song.lyrics.lines[song.lyrics.lineCounter], "end") == 0)
        {
            break;
        }
        song.lyrics.lineCounter++;
    }
}

// заполнение текста из файла
void fillLyricsByFile(Song& song)
{
    cout << "Укажите путь к файлу, содержащему текст песни: ";
    char path[256];
    cin.getline(path, sizeof(path));

    ifstream fin(path);
    if (!fin.is_open())
    {
        cout << "При открытии файла произошла ошибка!" << endl;
        return;
    }
    //создаем экземпляр класса для записи в него содержимого файла
    Song temp;
    int i = 0;
    while (i < MAX_LINES && fin.getline(temp.lyrics.lines[i], MAX_LENGTH))//пока getline вовращает true и не превышен размер массива строк
    {
        i++;
    }
    temp.lyrics.lineCounter = i;
    song.lyrics = temp.lyrics;
    fin.close();
}

//вывод текста песни
void printLyrics(Song& song)
{
    cout << "Текст песни: " << endl;
    for (int i = 0; i < song.lyrics.lineCounter; i++)
    {
        cout << song.lyrics.lines[i] << endl;
    }
}

//функция заполнения информации о песни и текста песни
void fillSongInfo(Song& song)
{
    cout << "Введите название песни: "; cin.getline(song.songName, sizeof(song.songName));
    cout << "Введите исполнителя: "; cin.getline(song.performer, sizeof(song.performer));
    cout << "Введите имя композитора: "; cin.getline(song.composer.name, sizeof(song.composer.name));
    cout << "Введите фамилию композитора: "; cin.getline(song.composer.surname, sizeof(song.composer.surname));
    cout << "Введите год выпуска: "; cin.getline(song.year, sizeof(song.year));
    char choice;
    start:
    cout << "Как вы хотите заполнить текст песни: 1 - загрузить файл с текстом, 2 - ввести текст вручную: ";
    cin >> choice;
    cin.ignore();
    switch (choice)
    {
    case '1':
        fillLyricsByFile(song);
        break;
    case '2':
        fillLyrics(song);
        break;
    default:
        cout << "Попробуйте еще раз." << endl;
        goto start;
    }
}

//вывод песни
void printSong(Song song)
{
    cout << "Исполнитель: " << song.performer << ", Песня: " << song.songName << endl;
    cout << "(Композитор: " << song.composer.name << " " << song.composer.surname << ", " << song.year << ")" << endl << endl;
    printLyrics(song);
}

                                                            //1. добавление песни в список песен
void addSong(SongList& list, Song song)
{
    if (list.listSize == list.numberOfSongs) //проверка и увеличение размера массива
    {
        Song* newList = new Song[list.listSize *= 2];
        for (int i = 0; i < list.numberOfSongs; i++)
        {
            newList[i] = list.songs[i];
        }
        delete[] list.songs;
        list.songs = newList;
    }
    list.songs[list.numberOfSongs++] = song;
}

                                                                //7. печать списка песен
void printSongList(SongList& list)
{
    for (int i = 0; i < list.numberOfSongs;i++)

    {
        printSong(list.songs[i]);
        cout << endl;
    }

}


                                                               //2 Удаление песни из списка
void deleteSong(SongList& list)
{
    if (list.numberOfSongs > 0)
    {
        char songname[50];
        char performer[50];
        cout << "Введите название песни, которую хотите удалить из каталога: ";
        cin >> songname;
        cout << "Введите исполнителя песни, которую хотите удалить из каталога: ";
        cin >> performer;
        bool isFound = false;
        Song* newSongs = new Song[list.listSize - 1]; //создаем новый массив с размером на один меньше, чем был каталог
        int newsize = 0;
        for (int i = 0; i < list.numberOfSongs; i++) // перебор всех песен в каталоге
        {
            if (strcmp(list.songs[i].songName, songname) != 0 && strcmp(list.songs[i].performer, performer) != 0) //если названия  совпали, то
            {
                newSongs[newsize++] = list.songs[i];
            }
            else
            {
                isFound = true;
            }
        }

        if (isFound)
        {
            delete[] list.songs;
            list.songs = newSongs;
            list.numberOfSongs = newsize;
            cout << "Песня успешно удалена! " << endl;

        }
        else
        {
            delete[] newSongs;
            cout << "Совпадения не найдены!" << endl;
        }
    }
    else
    {
        cout << "Каталог пуст!" << endl;
    }

}



                                                                 //3 изменение текста песни
void editTheLyrics(SongList& list)
{
    char songname[50];
    char newLyrics[5000];
    cout << "Введите название песни, текст которой хотите отредактировать: ";
    cin >> songname;
    bool isFound = false;
    for (int i = 0; i < list.numberOfSongs; i++)
    {
        if (strcmp(list.songs[i].songName, songname) == 0)
        {
            isFound = true;
            cout << "Текст песни до исправления: " << endl;
            printLyrics(list.songs[i]);
            cout << "Введите новый текст: ";
            //cin.ignore();
            fillLyrics(list.songs[i]);
            cout << "Текст песни успешно изменен." << endl;
        }
        else
        {
            cout << "Совпадения не найдены!" << endl;
        }
    }
}


                                                    //4 скачивание текста песни в файл
void downloadSong(SongList list)
{
    char songname[50];
    cout << "Введите название песни, текст которой хотите скачать в виде txt файла: ";
    cin >> songname;
    bool isFound = false;
    int i = 0;


    //поиск совпадений

    for (; i < list.numberOfSongs; i++)
    {
        if (strcmp(list.songs[i].songName, songname) == 0)
        {
            isFound = true;
            break;
        }
    }

    if (isFound)
    {
        cout << "Введите путь для сохранения файла: ";
        char nameToDownload[100] = ""; // Инициализация пустой строки
        cin.ignore(); // Очистка буфера // Игнорируем один символ (обычно это '\n')
        cin.getline(nameToDownload, sizeof(nameToDownload));
        // Добавление ".txt" к имени файла
        strncat_s(nameToDownload, ".txt", sizeof(nameToDownload) - strlen(nameToDownload) - 1);
        cout << nameToDownload << endl;

        ofstream fout(nameToDownload);
        if (!fout.is_open())
        {
            cout << "Не удалось открыть файл";
            return;
        }
        for (int j = 0; j < list.songs[i].lyrics.lineCounter; j++)
        {
            fout << list.songs[i].lyrics.lines[j] << endl;

        }
        fout.close();
    }
    else
    {
        cout << "Совпадения не найдены!" << endl;
    }
}



                                                            //5 Поиск песни по исполнителю
void searchByPerformer(SongList list)

{
    char performer[50];
    cout << "Введите исполнителя для поиска по исполнителю: ";
    cin.getline(performer,50);
    bool isFound = false;
    int i = 0;

    for (; i < list.numberOfSongs; i++)
    {
        if (strstr(list.songs[i].performer, performer) != nullptr)
        {
            printSong(list.songs[i]);
            cout << endl;
            isFound = true;
        }
    }

    if (!isFound)
    {
        cout << "Совпадения не найдены!" << endl;
    }
}


                                                    //6 поиск песни по ключевому слову из текста
void searchByKey(SongList list)
{
    char keyword[50];
    cout << "Введите ключевое слово для поиска по ключевому слову: ";
    cin.getline(keyword,50);
    bool isFound = false;
    int i = 0;
    for (; i < list.numberOfSongs; i++)
    {
        for (int j = 0; j < list.songs[i].lyrics.lineCounter; j++)
        {
            if (strstr(list.songs[i].lyrics.lines[j], keyword) != nullptr)
            {
                printSong(list.songs[i]);
                cout << endl;
                isFound = true;
                break;
            }
        }

    }
    if (!isFound)
    {
        cout << "Совпадения не найдены!" << endl;
    }
}


int main()
{
    system("chcp 1251");
    setlocale(LC_ALL, "ru");

    //common info
    SongList list;
    list.songs = new Song[list.listSize];

    char menu;
    char exit = '1';
    start2:
    start:
    while (exit)
    {
        cout << "Меню:" << endl << "1. Добавить песню" << endl << "2. Удалить песню" << endl << "3. Редактировать текст песни"
            << endl << "4. Выгрузка текста песни в файл" << endl << "5. Поиск песни по исполнителю" << endl << "6. Поиск песни по ключевому слову" << endl
            << "7. Отобразить каталог текстов песен" << endl << "8. Выход из программы" << endl << endl;

        cin >> menu;
        cin.ignore();

        switch (menu)
        {
        case '1': {
            //song info
            int songAdd = 1;
            while (songAdd)
            {
                Song song;
                fillSongInfo(song);
                addSong(list, song);
                cout << endl << "Песня успешно добавлена. Добавить еще? 1 - да, 0 - нет" << endl;;
                cin >> songAdd;
                cout << endl;
            }
            break;
        }
        case '2': {
            deleteSong(list);
            break;
        }
        case '3': {
            editTheLyrics(list);
            break;
        }
        case '4': {
            downloadSong(list);
            break;
        }
        case '5': {
            searchByPerformer(list);
            break;
        }
        case '6': {
            searchByKey(list);
            break;
        }
        case '7': {
            printSongList(list);
            break;
        }
        case '8': {
            exit = 0;
            goto start;
            break;
        }
        default:
            cout << "Попробуйте еще раз" << endl;
            goto start2;
        }

        cout << endl << "1 - вернуться в меню, 0 - выйти из программы" << endl;
        cin >> exit;
    }


    return 0;
}

//Добавление текста песни {может быть введен с клавиатуры или загружен из файла}

//Удаление текста песни

//Изменение текста песни

//Отображение текста песни в файл

//Поиск и отображение всех песен одного автора

//Поиск и отображение Всех песен, содержащих слово, указанное пользователем

