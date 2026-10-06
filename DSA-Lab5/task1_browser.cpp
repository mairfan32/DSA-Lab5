// task 1: browser tab manager 
#include <iostream>
#include <string>
#include <limits>
using namespace std;

struct Tab {
    int id;
    string title, url;
    Tab *next, *prev;
    Tab(int i, const string& t, const string& u) : id(i), title(t), url(u), next(this), prev(this) {}
};

class TabManager {
    Tab* current = nullptr;
public:
    ~TabManager() {
        if (!current) return;
        Tab* t = current->next;
        while (t != current) { Tab* n = t->next; delete t; t = n; }
        delete current;
    }

    void openTab(int id, const string& title, const string& url) {
        if (find(id)) { cout << "A tab with ID " << id << " already exists.\n"; return; }
        Tab* n = new Tab(id, title, url);
        if (!current) { current = n; }
        else {
            n->next = current->next;
            n->prev = current;
            current->next->prev = n;
            current->next = n;
            current = n;                 // new tab becomes the active tab
        }
        cout << "Opened tab " << id << ".\n";
    }

    void closeCurrent() {
        if (!current) { cout << "No tabs open.\n"; return; }
        Tab* d = current;
        if (d->next == d) current = nullptr;       // only tab
        else {
            d->prev->next = d->next;
            d->next->prev = d->prev;
            current = d->next;                     // next tab becomes current
        }
        cout << "Closed tab " << d->id << ".\n";
        delete d;
    }

    void moveNext() { if (!current) cout << "No tabs open.\n"; else current = current->next; }
    void movePrev() { if (!current) cout << "No tabs open.\n"; else current = current->prev; }

    static void show(const Tab* t) {
        cout << "ID: " << t->id << " | Title: " << t->title << " | URL: " << t->url << "\n";
    }

    void displayCurrent() const {
        if (!current) { cout << "No tabs open.\n"; return; }
        show(current);
    }

    void displayForward() const {
        if (!current) { cout << "No tabs open.\n"; return; }
        Tab* t = current;
        do { show(t); t = t->next; } while (t != current);
    }

    void displayBackward() const {
        if (!current) { cout << "No tabs open.\n"; return; }
        Tab* t = current;
        do { show(t); t = t->prev; } while (t != current);
    }

    Tab* find(int id) const {
        if (!current) return nullptr;
        Tab* t = current;
        do { if (t->id == id) return t; t = t->next; } while (t != current);
        return nullptr;
    }

    void search(int id) const {
        Tab* t = find(id);
        if (t) show(t); else cout << "Tab " << id << " not found.\n";
    }
};

int main() {
    TabManager tm;
    int choice;
    do {
        cout << "\nBrowser Tab Manager\n"
                "1. Open New Tab\n2. Close Current Tab\n3. Move Next\n4. Move Previous\n"
                "5. Display Current Tab\n6. Display All Tabs Forward\n7. Display All Tabs Backward\n"
                "8. Search Tab\n0. Exit\nChoice: ";
        if (!(cin >> choice)) break;
        switch (choice) {
            case 1: {
                int id; string title, url;
                cout << "Tab ID: "; cin >> id; cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Title: "; getline(cin, title);
                cout << "URL: "; getline(cin, url);
                tm.openTab(id, title, url); break;
            }
            case 2: tm.closeCurrent(); break;
            case 3: tm.moveNext(); tm.displayCurrent(); break;
            case 4: tm.movePrev(); tm.displayCurrent(); break;
            case 5: tm.displayCurrent(); break;
            case 6: tm.displayForward(); break;
            case 7: tm.displayBackward(); break;
            case 8: { int id; cout << "Tab ID: "; cin >> id; tm.search(id); break; }
            case 0: cout << "End Program.\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    return 0;
}