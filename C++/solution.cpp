/*Необходимо написать программу на С++, которая реализует работу с простой базой данных (сокращённо «БД»).
 Программа будет общаться с пользователем через стандартный ввод и вывод (потоки stdin и stdout).
 Будем хранить в нашей БД пары вида: дата, событие. Определим дату как строку 
 вида Год-Месяц-День, где Год, Месяц и День — целые числа.
 Событие определим как строку из произвольных печатных символов без разделителей внутри (пробелов,
 табуляций и пр.). Событие не может быть пустой строкой. В одну и ту же дату может произойти много
 разных событий, БД должна суметь их все сохранить. Одинаковые события, 
 произошедшие в один и тот же день, сохранять не нужно: достаточно сохранить только одно из них.
Наша БД должна понимать определённые команды, чтобы с ней можно было взаимодействовать*/


#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

void InvalidFormat(const string &dats)
{
    stringstream ss;
    ss << "Wrong date format: " << dats;
    throw runtime_error(ss.str());
}

void CorrectInput(stringstream &stream, const string &dats)
{
    if (stream.peek() != '-')
    {
        InvalidFormat(dats);
    }
    stream.ignore(1);
}
class Data
{
public:
    Data(const int &new_year, const int &new_month, const int &new_day)
    {
        year = new_year;
        month = new_month;
        day = new_day;
    }
    void DataParse(const string &dats)
    {
        int y, d, m;
        char s;
        stringstream stream(dats);
        stream >> y;
        CorrectInput(stream, dats);
        stream >> m;
        CorrectInput(stream, dats);
        stream >> d;
        if (!stream.eof())
        {
            InvalidFormat(dats);
        }
        if (y > 9999)
        {
            InvalidFormat(dats);
        }
        else
        {
            year = y;
        }
        if (m > 99)
        {
            InvalidFormat(dats);
        }
        if (d > 99)
        {
            InvalidFormat(dats);
        }
        if (m > 12 || m <= 0)
        {
            stringstream ss;
            ss << "Month value is invalid: " << to_string(m);
            throw runtime_error(ss.str());
        }
        else
        {
            month = m;
        }
        if (d <= 0 || d > 31)
        {
            stringstream ss;
            ss << "Day value is invalid: " << to_string(d);
            throw runtime_error(ss.str());
        }
        else
        {
            day = d;
        }
    }
    Data(const string &date)
    {
        DataParse(date);
    }
    int GetY() const
    {
        return year;
    }
    int GetM() const
    {
        return month;
    }
    int GetD() const
    {
        return day;
    }

private:
    int day;
    int month;
    int year;
};
class DataBase
{
public:
    void AddEv(const Data &data, const string &event)
    {
        database[data].insert(event);
    }
    bool DelEv(const Data &data, const string &event)
    {
        if (database.count(data))
        {
            if (database[data].count(event))
            {
                database[data].erase(event);
                return true;
            }
        }
        return false;
    }
    int DelD(const Data &data)
    {
        int number = 0;
        if (database.count(data))
        {
            number = database[data].size();
        }
        database.erase(data);
        return number;
    }
    set<string> FindEv(const Data &date)
    {
        set<string> even;
        if (database.count(date))
        {
            for (const auto& gip : database.at(date))
            {
                even.insert(gip);
            }
        }
        return even;
    }
    void PrintDB() const
    {
        for (const auto &i : database)
        {
            for (const auto &j : i.second)
            {
                if (i.first.GetY() != -1)
                {
                    cout << setfill('0');
                    cout << setw(4) << i.first.GetY() << "-" << setw(2) << i.first.GetM() << "-" << setw(2) << i.first.GetD();
                cout << " " << j << endl;
                }
            }
        }
    }

private:
    map<Data, set<string>> database;
};

bool operator<(const Data &lhs, const Data &rhs)
{
    if (lhs.GetY() == rhs.GetY())
    {
        if (lhs.GetM() == rhs.GetM())
        {
            return lhs.GetD() < rhs.GetD();
        }
        return lhs.GetM() < rhs.GetM();
    }
    return lhs.GetY() < rhs.GetY();
}

int main()
{
    DataBase celebration;
    string order;
    while (getline(cin, order))
    {
        stringstream ss(order);
        string temp;
        
        try
        {
            ss >> temp;
            if (temp == "Add")
            {
                string date, events = "";
                ss >> date >> events;
                try
                {
                    celebration.AddEv({date}, events);
                }
                catch (exception &e)
                {
                    cout << e.what() << endl;
                    return 0;
                }
            }
            else if (temp == "Del")
            {
                string date, events;
                ss >> date >> events;
                if (events != "")
                {
                    if (celebration.DelEv({date}, events))
                    {
                        cout << "Deleted successfully" << endl;
                    }
                    else
                        cout << "Event not found" << endl;
                }
                else
                {
                    int numbers = celebration.DelD({date});
                    cout << "Deleted " << numbers << " events" << endl;
                }
            }
            else if (temp == "Find")
            {
                string date;
                ss >> date;
                set<string> events = celebration.FindEv({date});
                for (const auto &git : events)
                {
                    cout << git << endl;
                }
            }
            else if (temp == "Print")
            {
                celebration.PrintDB();
            }
            else
            {
                if (temp != "")
                {
                    stringstream ss;
                    ss << "Unknown command: " << temp;
                    throw runtime_error(ss.str());
                }
            }
        }
        catch (exception &e)
        {
            cout << e.what() << endl;
            return 0;
        }
    }
    return 0;
}