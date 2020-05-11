#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

void Pause() {
    cout << "Please press any key to return to previous menu... ";
    cin.get();
    cin.get();
}
void Clear() {
    #if defined _WIN32
    system("cls");
    #elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
    #elif defined(__APPLE__)
    system("clear");
    #endif
}

class Pharmacy {
    public:
        virtual void read(istream& in) = 0;
        virtual void write(ostream& out) = 0;
        friend istream& operator>> (istream& in , Pharmacy& ph) {
            ph.read(in);
            return in;
        }
        friend ostream& operator<< (ostream& out, Pharmacy& ph) {
            ph.write(out);
            return out;
        }
};

class PhysicalPharmacy: public Pharmacy {
    private:
        string name;
        int nr_employees;
        double profit;
    public:
        PhysicalPharmacy(string name = "", int nr_employees = 0, double profit = 0) {
            this->name = name;
            this->nr_employees = nr_employees;
            this->profit = profit;
        }
        PhysicalPharmacy(PhysicalPharmacy& ph) {
            this->name = ph.name;
            this->nr_employees = ph.nr_employees;
            this->profit = ph.profit;
        }
        PhysicalPharmacy operator=(const PhysicalPharmacy& ph) {
            this->name = ph.name;
            this->nr_employees = ph.nr_employees;
            this->profit = ph.profit;
            return *this;
        }
        ~PhysicalPharmacy() {
            this->name = "";
            this->nr_employees = 0;
            this->profit = 0;
        }
        void read(istream& in ) {
            cout << endl << "Name: ";
            cin >> name;
            cout << "Number of employees: ";
            cin >> nr_employees;
            cout << "Profit: ";
            cin >> profit;
        }
        void write(ostream& out) {
            cout << endl << "Name: " << name << endl;
            cout << "Number of employees: " << nr_employees << endl;
            cout << "Profit: " << profit << endl;
        }
        friend istream& operator>> (istream& in , PhysicalPharmacy& pp) {
            pp.read(in);
            return in;
        }
        friend ostream& operator<< (ostream& out, PhysicalPharmacy& pp) {
            pp.write(out);
            return out;
        }
};

template <class T> class OnlinePharmacy {
    private:
        T total_nr_visitors = 0;
    public:
        OnlinePharmacy() {
            total_nr_visitors = 0;
        }
        OnlinePharmacy(const OnlinePharmacy& op) {
            total_nr_visitors = op.total_nr_visitors;
        }
        T set_total_nr_visitors(T x) {
            total_nr_visitors += x;
        }
        T get_total_nr_visitors() const {
            return total_nr_visitors;
        }
        void write(ostream& out) {
            out << "There are " << total_nr_visitors << " total visitors online." << endl << endl;
        }
        friend ostream& operator<< (ostream& out, OnlinePharmacy& op) {
            op.write(out);
            return out;
        }
};

template <> class OnlinePharmacy<int> {
    private:
        int total_nr_visitors = 0;
    public:
        OnlinePharmacy() {
            total_nr_visitors = 0;
        }
        OnlinePharmacy(const OnlinePharmacy& op) {
            total_nr_visitors = op.total_nr_visitors;
        }
        int set_total_nr_visitors(int x) {
            total_nr_visitors += x;
        }
        int get_total_nr_visitors() const {
            return total_nr_visitors;
        }
        void write(ostream& out) {
            out << "There are " << total_nr_visitors << " total visitors online." << endl << endl;
        }
        friend ostream& operator << (ostream& out, OnlinePharmacy& op) {
            op.write(out);
            return out;
        }
};

template <class T> class PharmacyManagement {
    private:
        OnlinePharmacy <int> x;
        static int index;
        int n, retail;
        T **v;
        vector<tuple<string, int, double> > totally_and_utterly_useless_data_structure_but_I_will_do_it_anyway;
    public:
        PharmacyManagement(int n = 0) {
            this->n = n;
            v = new T*[n];
        }
        PharmacyManagement(PharmacyManagement& pm) {
            delete[] v;
            this->n = pm.n;
            v = pm.v;
        }
        PharmacyManagement(const PharmacyManagement& pm) {
            retail = pm.retail;
            delete v;
            this->n = pm.n;
            v = new Pharmacy*[n];
            for (int i = 0; i < n; i++) v[i] = pm.v[i];
        }
        PharmacyManagement& operator= (const PharmacyManagement& pm) {
            retail = pm.retail;
            delete v;
            this->n = pm.n;
            v = new Pharmacy*[n];
            for (int i = 0; i < n; i++) v[i] = pm.v[i];
            return *this;
        }
        ~PharmacyManagement() {
            delete[] v;
            this->n = 0;
        }
        void add(int i) {
            here2:
            string type; 
            cout << endl << "Introduce the type of pharmacy #" << i + 1 << ": ";cin >> type;
            try {
                if (type.compare("physical") == 0) {
                    v[i] = new PhysicalPharmacy;
                    cin >> *v[i];
                } else {
                    if (type.compare("online") == 0) {
                        v[i] = NULL;
                        string a;
                        int b;
                        double c;
                        cout << endl << "URL: ";
                        cin >> a;
                        cout << "Number of visitors: ";
                        cin >> b;
                        x.set_total_nr_visitors(b);
                        cout << "Discount: ";
                        cin >> c;
                        cout << endl;
                        totally_and_utterly_useless_data_structure_but_I_will_do_it_anyway.push_back(make_tuple(a, b, c));
                    } else throw 1;
                }
            } catch (int i) {
                cout << "Please type either online or physical." << endl;
                goto here2;
            }
        }
        void start() {
            here0: try {
                cout << endl << "Introduce retail's ID: ";
                cin >> retail;
                throw retail;
            }
            catch (int i) {
                if (i <= 0) {
                    cout << endl << "Please introduce a number higher than 0." << endl;
                    goto here0;
                }
            }
        }
        void read(istream& in) {
            start();
            here1: try {
                cout << endl << "Introduce the number of pharmacies: ";
                cin >> n;
                throw n;
            }
            catch (int i) {
                if (i <= 0) {
                    cout << endl << "Please introduce a number higher than 0." << endl;
                    goto here1;
                }
            }
            delete[] v;
            v = new T*[n];
            for (int i = 0; i < n; i++) {
                add(i);
                index++;
            }
        }
        void write(ostream& out) {
            vector<tuple<string, int, double> > ::iterator iter = totally_and_utterly_useless_data_structure_but_I_will_do_it_anyway.begin();
            cout << endl << "Retail ID: " << retail << endl;
            for (int i = 0; i < n; i++) {
                if (dynamic_cast <PhysicalPharmacy*> (v[i]) != NULL) {
                    cout << "Pharmacy #" << i + 1 << endl << "Type: physical" << *v[i] << endl;
                } else {
                    cout << endl << "Pharmacy #" << i + 1 << endl << "Type: online";
                    cout << endl << "URL: " << get <0> (*iter) << endl;
                    cout << "Number of visitors: " << get <1> (*iter) << endl;
                    cout << "Discount: " << get <2> (*iter) << endl << endl;
                    iter++;
                }
            }
            cout<<x;
        }
        friend void operator+= (PharmacyManagement < T >& pm, Pharmacy& p) {
            if (pm.n == 0) pm.start();
            PharmacyManagement <T>*temp = new PharmacyManagement <T> [++pm.n];
            *temp = pm;
            pm = *temp;
            pm.add(pm.n - 1);
            index++;
            cout << endl << "The pharmacy has been added." << endl << endl;
        }
        friend istream& operator>> (istream& in , PharmacyManagement& pm) {
            pm.read(in);
            return in;
        }
        friend ostream& operator<< (ostream& out, PharmacyManagement& pm) {
            pm.write(out);
            return out;
        }
};
template <class T> int PharmacyManagement <T> ::index = 0;

void menu() {
    int exit = 0, action, read = 0, n;
    PharmacyManagement <Pharmacy>*p = new PharmacyManagement <Pharmacy> ;
    while (!exit) {
        cout << "MENU:" << endl << endl;
        cout << "1. Read information about pharmacies." << endl;
        cout << "2. Print all existing data." << endl;
        cout << "3. Add a pharmacy." << endl;
        cout << "0. Exit program." << endl;
        cout << endl << "Select action: "; cin >> action;
        switch (action) {
        case 0:
            cout << endl << "You have successfully exited the program." << endl;
            exit = 1;
            Pause(); Clear(); break;
        case 1:
            if (read == 1) {
                cout << endl << "You can't overwrite pre-existing information. " << endl << endl;
                Pause(); Clear(); break;
            } else {
                cin >>*p;
                read=1;
                Pause(); Clear(); break;
            }
        case 2:
            if (read == 0) {
                cout << "No information available." << endl;
                Pause(); Clear(); break;
            } else {
                cout <<*p;
                Pause(); Clear(); break;
            }
        case 3:
            if (read == 0) cout << endl << "There is no pre-existing information, but I saw it coming. Don't worry, it's going to work." << endl;
            Pharmacy*v;
            *p +=*v;
            read = 1;
            Pause(); Clear(); break;
        default:
            cout << "Error. Please introduce a value between 0 and 3." << endl;
            Pause(); Clear(); break;
        }
    }
}

int main() {
    menu();
}