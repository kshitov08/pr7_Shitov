// pr7_Shitov2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>

using namespace std;
#define DEBUG_MODE

struct Game {
    string title;
    string genre;
    string platform;
    int hours;
    string status;
    int rating;
};

Game LineToRecord(string line)
{
    if (line.empty())
        throw runtime_error("Строка пустая");

    string data[6];
    for (int i = 0; i < 6; i++)
    {
        size_t pos = line.find(";");

        if (pos == string::npos && i == 5)
        {
            data[i] = line;
            break;
        }
        else if (pos == string::npos)
        {
            throw logic_error("Недостаточно данных в строке");
        }

        data[i] = line.substr(0, pos);

        if (data[i].empty())
            throw invalid_argument("Значение в строке пустое");

        line.erase(0, pos + 1);
    }

    Game g;

    g.title = data[0];
    if (g.title.empty())
        throw invalid_argument("Пустое название");

    g.genre = data[1];
    if (g.genre.empty())
        throw invalid_argument("Пустой жанр");

    g.platform = data[2];
    if (g.platform.empty())
        throw invalid_argument("Пустая платформа");

    try
    {
        g.hours = stod(data[3]);
    }
    catch (exception&)
    {
        throw invalid_argument("Не удалось преобразовать часы в число");
    }
    if (g.hours < 0)
        throw out_of_range("Часы не могут быть отрицательными");

    g.status = data[4];
    if (g.status.empty())
        throw invalid_argument("Пустой статус");

    try
    {
        g.rating = stod(data[5]);
    }
    catch (exception&)
    {
        throw invalid_argument("Не удалось преобразовать оценку в число");
    }
    if (g.rating < 0.0 || g.rating > 10.0)
        throw out_of_range("Оценка должна быть в диапазоне 0..10");

    return g;
}

int LoadGames(const string& filename, Game games[], int maxGames)
{
    ifstream file(filename);
    if (!file.is_open())
        throw runtime_error("Не удалось открыть файл: " + filename);

    int count = 0;
    string line;
    while (getline(file, line))
    {
        if (line.empty()) continue;

        Game g = LineToRecord(line);

        if (count >= maxGames)
            throw runtime_error("Слишком много записей");

        games[count++] = g;
    }
    file.close();

#ifdef DEBUG_MODE
    cout << "[DEBUG] Загружено записей: " << count << endl;
#endif

    return count;
}

void SaveGames(const string& filename, Game games[], int count)
{
    ofstream file(filename);
    if (!file.is_open())
        throw runtime_error("Не удалось открыть файл для записи: " + filename);

    for (int i = 0; i < count; i++)
    {
        file << games[i].title << ';'
            << games[i].genre << ';'
            << games[i].platform << ';'
            << games[i].hours << ';'
            << games[i].status << ';'
            << games[i].rating << '\n';
    }
    file.close();

#ifdef DEBUG_MODE
    cout << "[DEBUG] Сохранено записей: " << count << endl;
#endif
}

void PrintGame(const Game& g)
{
    cout << g.title << " | " << g.genre << " | " << g.platform
        << " | " << g.hours << " ч | " << g.status
        << " | оценка " << g.rating << endl;
}

void PrintGames(Game games[], int count)
{
    if (count == 0) { cout << "Ничего не найдено\n"; return; }
    for (int i = 0; i < count; i++)
    {
        cout << i + 1 << ". ";
        PrintGame(games[i]);
    }
#ifdef DEBUG_MODE
    cout << "[DEBUG] Выведено записей: " << count << endl;
#endif

}
string toLower(string s)
{
    for (size_t i = 0; i < s.size(); i++)
        if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] = s[i] + 32;
    return s;
}
//Поиск по части названия
void FindByTitle(Game games[], int count, const string& part)
{
    string p = toLower(part);
    bool found = false;
    for (int i = 0; i < count; i++)
    {
        if (toLower(games[i].title).find(p) != string::npos)
        {
            cout << i + 1 << ". ";
            PrintGame(games[i]);
            found = true;
        }
    }
    if (!found) cout << "Ничего не найдено\n";
}

//Фильтрация по жанру
void FilterByGenre(Game games[], int count, const string& genre)
{
    int count2 = 0;
    bool found = false;
    for (int i = 0; i < count; i++)
        if (games[i].genre == genre)
        {
            cout << i + 1 << ". ";
            PrintGame(games[i]);
            found = true;
            count2++;
        }
    if (!found) cout << "Ничего не найдено\n";
#ifdef DEBUG_MODE
    cout << "[DEBUG] Найдено записей: " << count2 << endl;
#endif
}

//Фильтрация по платформе
void FilterByPlatform(Game games[], int count, const string& platform)
{
    int count2 = 0;
    bool found = false;
    for (int i = 0; i < count; i++)
        if (games[i].platform == platform)
        {
            cout << i + 1 << ". ";
            PrintGame(games[i]);
            found = true;
            count2++;
        }
    if (!found) cout << "Ничего не найдено\n";
#ifdef DEBUG_MODE
    cout << "[DEBUG] Найдено записей: " << count2 << endl;
#endif
}

//Вывод пройденных игр
void ShowCompleted(Game games[], int count)
{
    int count2 = 0;
    bool found = false;
    for (int i = 0; i < count; i++)
        if (games[i].status == "Complited")
        {
            cout << i + 1 << ". ";
            PrintGame(games[i]);
            found = true;
            count2++;
        }
    if (!found) cout << "Ничего не найдено\n";
#ifdef DEBUG_MODE
    cout << "[DEBUG] Найдено записей: " << count2 << endl;
#endif
}

//Изменение количества сыгранных часов и статуса
void UpdateProgress(Game games[], int count, const string& title,
    double addHours, const string& newStatus)
{
    if (addHours < 0)
        throw invalid_argument("Добавляемые часы должны быть больше нуля");

    for (int i = 0; i < count; i++)
    {
        if (games[i].title == title)
        {
            games[i].hours += addHours;
            if (!newStatus.empty()) games[i].status = newStatus;
            cout << "Обновлено.\n";
            return;
        }
    }
    throw runtime_error("Игра не найдена: " + title);
}

//Вывод игр с оценкой выше заданной
void ShowRatingAbove(Game games[], int count, double minRating)
{
    int count2 = 0;
    bool found = false;
    for (int i = 0; i < count; i++)
        if (games[i].rating > minRating)
        {
            cout << i + 1 << ". ";
            PrintGame(games[i]);
            found = true;
            count2++;
        }
    if (!found) cout << "Ничего не найдено\n";
#ifdef DEBUG_MODE
    cout << "[DEBUG] Выведено записей: " << count2 << endl;
#endif
}
int main()
{
    setlocale(LC_ALL, "Russian");

    const string filename = "games.txt";
    const int maximum = 20;
    Game games[maximum];
    int count = 0;

    try { count = LoadGames(filename, games, maximum); }
    catch (exception& e)
    {
        cout << "Ошибка загрузки: " << e.what() << endl;
        return 1;
    }
    PrintGames(games, count);
    int choice = -1;
    while (choice != 0)
    {
        cout << "\n===ИГРОВАЯ БИБЛИОТЕКА===\n"
            << "1. Поиск по части названия\n"
            << "2. Фильтр по жанру\n"
            << "3. Фильтр по платформе\n"
            << "4. Пройденные игры\n"
            << "5. Изменить часы и статус\n"
            << "6. Игры с оценкой выше заданной\n"
            << "7. Сохранить\n"
             << "0. Выход\n";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cout << "Некорректный ввод\n";
            continue;
        }
        cin.ignore(10000, '\n');
        try
        {
            if (choice == 1)
            {
                string s;
                cout << "Часть названия: ";
                getline(cin, s);
                FindByTitle(games, count, s);
            }
            else if (choice == 2)
            {
                string s;
                cout << "Жанр: ";
                getline(cin, s);
                FilterByGenre(games, count, s);
            }
            else if (choice == 3)
            {
                string s;
                cout << "Платформа: ";
                getline(cin, s);
                FilterByPlatform(games, count, s);
            }
            else if (choice == 4)
            {
                ShowCompleted(games, count);
            }
            else if (choice == 5)
            {
                string t, st;
                double h;
                cout << "Название: ";
                //cin.ignore(10000, '\n');
                getline(cin, t);
                cout << "Добавить часов: ";
                cin >> h;
                cin.ignore(10000, '\n');
                cout << "Новый статус (Enter — не менять): ";
                getline(cin, st);
                UpdateProgress(games, count, t, h, st);
            }
            else if (choice == 6)
            {
                double r;
                cout << "Минимальная оценка: ";
                cin >> r;
                ShowRatingAbove(games, count, r);
            }
            else if (choice == 7 || choice == 0)
            {
                SaveGames(filename, games, count);
            }
        }
        catch (const exception& e)
        {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }

    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
