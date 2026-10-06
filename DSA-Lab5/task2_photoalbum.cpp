// task 2: circular photo album 
#include <iostream>
#include <string>
#include <limits>
using namespace std;

struct Photo {
    int id;
    string name, date, location;
    Photo *next, *prev;
    Photo(int i, const string& n, const string& d, const string& l)
        : id(i), name(n), date(d), location(l), next(this), prev(this) {}
};

class Album {
    Photo* head = nullptr;      // first photo
    Photo* current = nullptr;   // selected photo
public:
    ~Album() {
        if (!head) return;
        Photo* p = head->next;
        while (p != head) { Photo* n = p->next; delete p; p = n; }
        delete head;
    }

    static void show(const Photo* p) {
        cout << "ID: " << p->id << " | Name: " << p->name
             << " | Date: " << p->date << " | Location: " << p->location << "\n";
    }

    Photo* find(int id) const {
        if (!head) return nullptr;
        Photo* p = head;
        do { if (p->id == id) return p; p = p->next; } while (p != head);
        return nullptr;
    }

    void addEnd(int id, const string& n, const string& d, const string& l) {
        if (find(id)) { cout << "Photo ID " << id << " already exists.\n"; return; }
        Photo* x = new Photo(id, n, d, l);
        if (!head) { head = current = x; }
        else {
            Photo* tail = head->prev;
            x->prev = tail; x->next = head;
            tail->next = x; head->prev = x;
        }
    }

    void insertAfterCurrent(int id, const string& n, const string& d, const string& l) {
        if (!head) { addEnd(id, n, d, l); return; }
        if (find(id)) { cout << "Photo ID " << id << " already exists.\n"; return; }
        Photo* x = new Photo(id, n, d, l);
        x->next = current->next; x->prev = current;
        current->next->prev = x;
        current->next = x;
    }

    // unlinks and frees p; returns the node that followed it (or nullptr if list empty)
    Photo* unlink(Photo* p) {
        Photo* nxt = nullptr;
        if (p->next == p) { head = nullptr; }
        else {
            p->prev->next = p->next;
            p->next->prev = p->prev;
            nxt = p->next;
            if (p == head) head = nxt;
        }
        delete p;
        return nxt;
    }

    void removeById(int id) {
        Photo* p = find(id);
        if (!p) { cout << "Photo " << id << " not found.\n"; return; }
        bool wasCurrent = (p == current);
        Photo* nxt = unlink(p);
        if (wasCurrent || !head) current = nxt;
        cout << "Photo " << id << " removed.\n";
    }

    void removeCurrent() {
        if (!head) { cout << "Album is empty.\n"; return; }
        int id = current->id;
        current = unlink(current);   // next photo becomes current
        cout << "Photo " << id << " removed.\n";
    }

    void moveNext() { if (!head) cout << "Album is empty.\n"; else { current = current->next; show(current); } }
    void movePrev() { if (!head) cout << "Album is empty.\n"; else { current = current->prev; show(current); } }

    void displayForward() const {
        if (!head) { cout << "Album is empty.\n"; return; }
        Photo* p = current;
        do { show(p); p = p->next; } while (p != current);
    }

    void displayBackward() const {
        if (!head) { cout << "Album is empty.\n"; return; }
        Photo* p = current;
        do { show(p); p = p->prev; } while (p != current);
    }

    void search(int id) const {
        Photo* p = find(id);
        if (p) show(p); else cout << "Photo " << id << " not found.\n";
    }

    int count() const {
        if (!head) return 0;
        int c = 0; Photo* p = head;
        do { c++; p = p->next; } while (p != head);
        return c;
    }
};

static void readPhoto(int& id, string& n, string& d, string& l) {
    cout << "Photo ID: "; cin >> id; cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Name: "; getline(cin, n);
    cout << "Date Taken: "; getline(cin, d);
    cout << "Location: "; getline(cin, l);
}

int main() {
    Album a;
    int n;
    cout << "Number of photos: "; cin >> n;
    for (int i = 0; i < n; i++) {
        int id; string nm, d, l;
        cout << "Photo " << i + 1 << ":\n";
        readPhoto(id, nm, d, l);
        a.addEnd(id, nm, d, l);
    }

    int c;
    do {
        cout << "\n Circular Photo Album \n"
                "1. Add Photo (end)\n2. Insert Photo After Current\n3. Remove Photo by ID\n"
                "4. Remove Current Photo\n5. Move Next\n6. Move Previous\n"
                "7. Display Forward\n8. Display Backward\n9. Search Photo\n10. Count Photos\n0. Exit\nChoice: ";
        if (!(cin >> c)) break;
        int id; string nm, d, l;
        switch (c) {
            case 1: readPhoto(id, nm, d, l); a.addEnd(id, nm, d, l); break;
            case 2: readPhoto(id, nm, d, l); a.insertAfterCurrent(id, nm, d, l); break;
            case 3: cout << "Photo ID: "; cin >> id; a.removeById(id); break;
            case 4: a.removeCurrent(); break;
            case 5: a.moveNext(); break;
            case 6: a.movePrev(); break;
            case 7: a.displayForward(); break;
            case 8: a.displayBackward(); break;
            case 9: cout << "Photo ID: "; cin >> id; a.search(id); break;
            case 10: cout << "Total photos: " << a.count() << "\n"; break;
            case 0: cout << "End program.\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (c != 0);
    return 0;
}