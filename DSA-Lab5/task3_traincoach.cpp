// task 3: train coach navigation system 
#include <iostream>
#include <string>
#include <limits>
using namespace std;

struct Coach {
    int number;
    string type;
    int capacity, passengers;
    Coach *next, *prev;
    Coach(int n, const string& t, int cap, int pass)
        : number(n), type(t), capacity(cap), passengers(pass), next(this), prev(this) {}
};

class Train {
    Coach* head = nullptr;      // first coach
    Coach* current = nullptr;   // selected coach
public:
    ~Train() {
        if (!head) return;
        Coach* c = head->next;
        while (c != head) { Coach* n = c->next; delete c; c = n; }
        delete head;
    }

    static void show(const Coach* c) {
        cout << "Coach #" << c->number << " | Type: " << c->type
             << " | Capacity: " << c->capacity << " | Passengers: " << c->passengers
             << " | Empty seats: " << c->capacity - c->passengers << "\n";
    }

    Coach* find(int num) const {
        if (!head) return nullptr;
        Coach* c = head;
        do { if (c->number == num) return c; c = c->next; } while (c != head);
        return nullptr;
    }

    bool valid(int num, int cap, int pass) const {
        if (find(num)) { cout << "Coach number " << num << " already exists.\n"; return false; }
        if (cap < 0 || pass < 0 || pass > cap) {
            cout << "Invalid capacity/passengers (need 0 <= passengers <= capacity).\n"; return false;
        }
        return true;
    }

    void addCoach(int num, const string& type, int cap, int pass) {
        if (!valid(num, cap, pass)) return;
        Coach* x = new Coach(num, type, cap, pass);
        if (!head) { head = current = x; }
        else {
            Coach* tail = head->prev;
            x->prev = tail; x->next = head;
            tail->next = x; head->prev = x;
        }
        cout << "Coach " << num << " added.\n";
    }

    void insertAfter(int after, int num, const string& type, int cap, int pass) {
        Coach* p = find(after);
        if (!p) { cout << "Coach " << after << " not found.\n"; return; }
        if (!valid(num, cap, pass)) return;
        Coach* x = new Coach(num, type, cap, pass);
        x->prev = p; x->next = p->next;
        p->next->prev = x;
        p->next = x;
        cout << "Coach " << num << " inserted after coach " << after << ".\n";
    }

    void removeCoach(int num) {
        Coach* d = find(num);
        if (!d) { cout << "Coach " << num << " not found.\n"; return; }
        if (d->next == d) { head = current = nullptr; }
        else {
            d->prev->next = d->next;
            d->next->prev = d->prev;
            if (d == head) head = d->next;
            if (d == current) current = d->next;   // next valid coach becomes current
        }
        delete d;
        cout << "Coach " << num << " removed.\n";
    }

    void moveForward()  { if (!head) cout << "Train is empty.\n"; else { current = current->next; show(current); } }
    void moveBackward() { if (!head) cout << "Train is empty.\n"; else { current = current->prev; show(current); } }

    void displayClockwise() const {
        if (!head) { cout << "Train is empty.\n"; return; }
        Coach* c = head;
        do { show(c); c = c->next; } while (c != head);
    }

    void displayAnticlockwise() const {
        if (!head) { cout << "Train is empty.\n"; return; }
        Coach* c = head->prev;          // start from the last coach
        do { show(c); c = c->prev; } while (c != head->prev);
    }

    void search(int num) const {
        Coach* c = find(num);
        if (c) show(c); else cout << "Coach " << num << " not found.\n";
    }

    void maxAvailable() const {
        if (!head) { cout << "Train is empty.\n"; return; }
        Coach* best = head;
        Coach* c = head->next;
        while (c != head) {
            if (c->capacity - c->passengers > best->capacity - best->passengers) best = c;
            c = c->next;
        }
        cout << "Coach with the most empty seats:\n";
        show(best);
    }

    void displayCurrent() const {
        if (!head) cout << "Train is empty.\n"; else show(current);
    }

    // Swap next/prev in every node; the old last coach becomes the new first.
    void reverseTrain() {
        if (!head) { cout << "Train is empty.\n"; return; }
        Coach* oldTail = head->prev;
        Coach* c = head;
        do {
            Coach* n = c->next;
            c->next = c->prev;
            c->prev = n;
            c = n;                      // n is the old next, so we keep walking forward
        } while (c != head);
        head = oldTail;
        cout << "Train direction reversed.\n";
    }
};

static void readCoach(int& num, string& type, int& cap, int& pass) {
    cout << "Coach Number: "; cin >> num;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Coach Type: "; getline(cin, type);
    cout << "Passenger Capacity: "; cin >> cap;
    cout << "Current Passengers: "; cin >> pass;
}

int main() {
    Train t;
    int n;
    cout << "Number of coaches: "; cin >> n;
    for (int i = 0; i < n; i++) {
        int num, cap, pass; string type;
        cout << "Coach " << i + 1 << ":\n";
        readCoach(num, type, cap, pass);
        t.addCoach(num, type, cap, pass);
    }

    int c;
    do {
        cout << "\nTrain Coach Navigation System\n"
                "1. Add Coach\n2. Insert Coach After\n3. Remove Coach\n4. Move Forward\n"
                "5. Move Backward\n6. Display Train Clockwise\n7. Display Train Anti-clockwise\n"
                "8. Search Coach\n9. Find Maximum Available Capacity\n10. Display Current Coach\n"
                "11. Reverse Train Direction\n0. Exit\nChoice: ";
        if (!(cin >> c)) break;
        int num, cap, pass, after; string type;
        switch (c) {
            case 1: readCoach(num, type, cap, pass); t.addCoach(num, type, cap, pass); break;
            case 2:
                cout << "Insert after coach number: "; cin >> after;
                readCoach(num, type, cap, pass); t.insertAfter(after, num, type, cap, pass); break;
            case 3: cout << "Coach number to remove: "; cin >> num; t.removeCoach(num); break;
            case 4: t.moveForward(); break;
            case 5: t.moveBackward(); break;
            case 6: t.displayClockwise(); break;
            case 7: t.displayAnticlockwise(); break;
            case 8: cout << "Coach number: "; cin >> num; t.search(num); break;
            case 9: t.maxAvailable(); break;
            case 10: t.displayCurrent(); break;
            case 11: t.reverseTrain(); break;
            case 0: cout << "Goodbye.\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (c != 0);
    return 0;
}