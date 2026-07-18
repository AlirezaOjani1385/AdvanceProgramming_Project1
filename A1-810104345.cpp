#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

const int HALL1_SEATS = 10; 
const int HALL2_SEATS = 20; 
const int HALL3_SEATS = 30; 

typedef struct movie {
    int hall_number;
    int time;
    int free_seats;
    string name;
} movie;

typedef struct customer {
    int seat_number;
    string name;
    movie movie_customer;
} customer;

void sort_customers(vector<customer>& v);
void sort_movies(vector<movie>& v) ;
void add_show(vector<movie>& movies, map<int, int>& seats, string name, int time, int hall_number);
void reserve(vector<customer>& customers, vector<movie>& movies, string name, int seat_number, int hall_number, int time);
void list_movies(vector<movie>& movies, int hall_number);
void report_seats(vector<customer>& customers, vector<movie>& movies, int hall_number, int time);

int main() {
    vector<movie> movies;
    vector<customer> customers;
    map<int, int> seats = {{1, HALL1_SEATS}, {2, HALL2_SEATS}, {3, HALL3_SEATS}};
    string action;
    while (cin >> action) {
        if (action == "add_show") {
            string name;
            int time;
            int hall_number;
            cin >> name >> time >> hall_number;
            add_show(movies, seats, name, time, hall_number);
        }
        else if (action == "reserve") {
            int hall_number;
            int time;
            int seat_number;
            string name;
            cin >> hall_number >> time >> seat_number >> name;
            reserve(customers, movies, name, seat_number, hall_number, time);
        }
        else if(action == "list_movies") {
            int hall_number;
            cin >> hall_number;
            list_movies(movies, hall_number);
        }
        else if(action == "report_seats") {
            int hall_number;
            int time;
            cin >> hall_number >> time;
            report_seats(customers, movies, hall_number, time);
        }
    }
}

void sort_customers(vector<customer>& v) {
    sort(v.begin(), v.end(), [](customer a, customer b){ return a.seat_number < b.seat_number; });
}

void sort_movies(vector<movie>& v) {
    sort(v.begin(), v.end(), [](movie a, movie b){ return a.time < b.time; });
}

void add_show(vector<movie>& movies, map<int, int>& seats, string name, int time, int hall_number) {
    if (find_if(movies.begin(), movies.end(), [hall_number, time](movie mv)
    { return ((mv.hall_number == hall_number) && (mv.time == time)); }) != movies.end()) {
        cout << "A movie is already scheduled in this hall at this time" << endl;
        return;
    }
    movie add_movie{hall_number, time, seats[hall_number], name};
    movies.push_back(add_movie);
    cout << "OK" << endl;
}

void reserve(vector<customer>& customers, vector<movie>& movies, string name, int seat_number, int hall_number, int time) {
    if (find_if(customers.begin(), customers.end(), [seat_number, hall_number, time](customer cus)
    { return ((cus.seat_number == seat_number) && (cus.movie_customer.hall_number == hall_number) && (cus.movie_customer.time == time)); }) != customers.end()) {
        cout << "This seat is already reserved" << endl;
        return;
    }
    auto it = find_if(movies.begin(), movies.end(), [hall_number, time](movie mv){ return ((mv.hall_number == hall_number) && (mv.time == time)); });
    if (it == movies.end()) {
        cout << "No show is scheduled in this hall at the specified time" << endl;
        return;
    }
    else {
        (*it).free_seats -= 1;
    }
    customer add_customer{seat_number, name, {hall_number, time}};
    customers.push_back(add_customer);
    cout << "OK" << endl;
}

void list_movies(vector<movie>& movies, int hall_number) {
    sort_movies(movies);
    if (find_if(movies.begin(), movies.end(), [hall_number](movie mv){ return (mv.hall_number == hall_number); }) == movies.end()) {
        cout << "No movie found" << endl;
        return;
    }
    for (movie mv : movies) {
        if (mv.hall_number == hall_number)
            cout << mv.name << " at " << mv.time << ":00: " << mv.free_seats << " seats available" << endl;
    }
}

void report_seats(vector<customer>& customers, vector<movie>& movies, int hall_number, int time) {
    sort_customers(customers);
    if (find_if(customers.begin(), customers.end(), [hall_number, time](customer cus)
    { return ((cus.movie_customer.hall_number == hall_number) && (cus.movie_customer.time == time)); }) == customers.end()) {
        cout << "All seats are available" << endl;
        return;
    }
    for (customer cus : customers) {
        if ((cus.movie_customer.hall_number == hall_number) && (cus.movie_customer.time == time)) {
            cout << "Seat " << cus.seat_number << " is reserved by " << cus.name << endl;
        }
    }
}