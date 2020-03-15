#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

// O sa incerc sa dau cateva explicatii, nu stiu cat de clar e codul

void Pause() // nu exista functie ca sa inghet consola care sa mearga pe toate OS-urile, asa ca am improvizat prostia asta
{
    cin.get();
    cin.get();
}

void Clear() // din nou... nu e nimic universal, asa ca am facut asta
{
#if defined _WIN32
    system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
#elif defined (__APPLE__)
    system("clear");
#endif
}

class Monomial // clasa pentru nod
{               // n-am considerat necesar sa supraincarc operatorul +, pentru ca nu toate monoamele se pot aduna intre ele
                // sau daca se aduna, din 2 monoame cu exponenti diferiti iese un polinom, si nu are rost sa ma complic
                // cu franturi de liste, sa le leg intre ele

    private:
        int coeff;
        int exp;
        Monomial* next;

    public:

        Monomial(int coeff=0, int exp=0, Monomial* next=NULL) // constructor
        {
            this->coeff = coeff;
            this->exp = exp;
            this->next = next;
        }

        friend istream& operator>>(istream& in, Monomial& temp);
        friend ostream& operator<<(ostream& out, Monomial& temp);

        friend class RarePolynomial;
        friend istream& operator>>(istream& in, class RarePolynomial& X);
        friend ostream& operator<<(ostream& out, class RarePolynomial& X);
        friend RarePolynomial& operator+(RarePolynomial& X, RarePolynomial& Y);
        friend RarePolynomial& operator-(RarePolynomial& X, RarePolynomial& Y);
        friend RarePolynomial& operator*(RarePolynomial& X, RarePolynomial& Y);
};

istream& operator>>(istream& in, Monomial& temp)
{
    cout<<"Type coefficient: ";
    in>>temp.coeff;
    cout<<"Type exponent: ";
    in>>temp.exp;
    cout<<"\n";

    return in;
}

ostream& operator<<(ostream& out, Monomial& Monomial)
{
    if(Monomial.coeff == 1 || Monomial.coeff == -1)
        if(Monomial.exp == 0)
            out<<"1";
        else
            if(Monomial.exp != 1)
                out<<"x^"<<Monomial.exp;
            else
                out<<"x";
    else
        if(Monomial.exp == 0)
            out<<Monomial.coeff;
        else
            if(Monomial.exp!=1)
                out<<Monomial.coeff<<"x^"<<Monomial.exp;
            else
                out<<Monomial.coeff<<"x";

    return out;
}

class RarePolynomial // clasa pentru lista
{
    private:
        Monomial* front;
        Monomial* rear;

    public:
        RarePolynomial() // constructor
        {
            this->front = NULL;
            this->rear = NULL;
        }

        RarePolynomial(const RarePolynomial& X) // copy constructor
        {
            if(X.front == NULL)
                front = NULL;
            else
            {
                front=new Monomial(X.front->coeff, X.front->exp);
                Monomial *current_term = front;
                Monomial *temp_front = X.front;
                Monomial *temp = temp_front;
                while (temp->next != NULL)
                {
                    current_term->next = new Monomial(temp->next->coeff, temp->next->exp);
                    temp = temp->next;
                    current_term = current_term->next;
                }
            }
        }

        ~RarePolynomial() // destructor
        {
            Monomial* temp;

            while (front != NULL)
            {
                temp = front->next;
                delete front;
                front = temp;
            }
        }

        int Value(int x) // valoarea polinomului
        {
            int sum=0;
            Monomial* current_term = front;

            while(current_term != NULL)
            {
                sum += current_term->coeff * pow(x, current_term->exp);
                current_term = current_term->next;
            }

            return sum;
        }

        void AddTerm(Monomial* temp) // adaugare termeni in lista
        {
            if(front == NULL)
            {
                front = temp;
                rear = temp;
            }
            else
            {
                Monomial* current_term = front;
                Monomial* prev;

                while(temp->exp < current_term->exp && current_term!=rear)
                {
                    prev=current_term;
                    current_term=current_term->next;
                }

                if(current_term == front && temp->exp > current_term->exp)
                {
                    temp->next = front;
                    front=temp;
                }
                else
                    if(current_term == rear && temp->exp < current_term->exp)
                    {
                        rear->next = temp;
                        rear = temp;
                    }
                    else
                        if(temp->exp == current_term->exp)
                            if(current_term->coeff + temp->coeff == 0)
                                current_term = current_term->next;
                            else
                                current_term->coeff = current_term->coeff + temp->coeff;
                        else
                        {
                            temp->next = prev->next;
                            prev->next = temp;
                        }
            }
        }

        bool IsEmpty() // verificare daca lista e goala
        {
            if(front == NULL)
                return true;
            else
                return false;
        }

        friend ostream& operator<<(ostream& out, RarePolynomial& list);
        friend istream& operator>>(istream& in, RarePolynomial& X);
        friend RarePolynomial& operator+(RarePolynomial& X, RarePolynomial& Y);
        friend RarePolynomial& operator-(RarePolynomial& X, RarePolynomial& Y);
        friend RarePolynomial& operator*(RarePolynomial& X, RarePolynomial& Y);
};

ostream& operator<<(ostream& out, RarePolynomial& X)
{
    Monomial *temp = X.front;
    while(temp)
    {
        out<<*temp;
        if(temp->next)
            if(temp->coeff>0)
                out<<"+";
        temp = temp->next;
    }
    return out;
}

istream& operator>>(istream& in, RarePolynomial& X)
{

    while("")
    {
        Monomial* temp = new Monomial;
        cin>>*temp;
        if(temp->coeff == 0)
            break;
        X.AddTerm(temp);
    }
    return in;
}

RarePolynomial& operator+(RarePolynomial& X, RarePolynomial& Y)
{
    RarePolynomial *sum = new RarePolynomial;
    Monomial *current_x = X.front;
    Monomial *current_y = Y.front;

    while(current_x != NULL && current_y != NULL)
    {
        Monomial *temp = new Monomial;

        if(current_x->exp == current_y->exp)
        {
            temp->coeff = current_x->coeff + current_y->coeff;
            temp->exp = current_x->exp;
            sum->AddTerm(temp);
            current_x = current_x->next;
            current_y = current_y->next;
        }
        else
            if(current_x->exp < current_y->exp)
            {
                temp->coeff = current_y->coeff;
                temp->exp = current_y->exp;
                sum->AddTerm(temp);
                current_y = current_y->next;
            }
            else
                {
                    temp->coeff = current_x->coeff;
                    temp->exp = current_x->exp;
                    sum->AddTerm(temp);
                    current_x = current_x->next;
                }
    }

    while(current_x != NULL)
    {
        Monomial *temp = new Monomial;
        temp->coeff = current_x->coeff;
        temp->exp = current_x->exp;
        sum->AddTerm(temp);
        current_x = current_x->next;
    }

    while(current_y != NULL)
    {
        Monomial *temp = new Monomial;
        temp->coeff = current_y->coeff;
        temp->exp = current_y->exp;
        sum->AddTerm(temp);
        current_y = current_y->next;
    }

    return *sum;
}

RarePolynomial& operator-(RarePolynomial& X, RarePolynomial& Y)
{
    RarePolynomial *diff = new RarePolynomial;
    Monomial *current_x = X.front;
    Monomial *current_y = Y.front;

    while(current_x != NULL && current_y != NULL)
    {
        Monomial *temp = new Monomial;

        if(current_x->exp == current_y->exp)
        {
            if(current_x->coeff != current_y->coeff)
            {
                temp->coeff = current_x->coeff - current_y->coeff;
                temp->exp = current_x->exp;
                diff->AddTerm(temp);
                current_x = current_x->next;
                current_y = current_y->next;
            }
            else
                {
                    current_x = current_x->next;
                    current_y = current_y->next;
                }

        }
        else
            if(current_x->exp < current_y->exp)
            {
                temp->coeff =- current_y->coeff;
                temp->exp = current_y->exp;
                diff->AddTerm(temp);
                current_y = current_y->next;
            }
            else
                {
                    temp->coeff = current_x->coeff;
                    temp->exp = current_x->exp;
                    diff->AddTerm(temp);
                    current_x = current_x->next;
                }
    }

    while(current_x != NULL)
    {
        Monomial *temp = new Monomial;

        temp->coeff = current_x->coeff;
        temp->exp = current_x->exp;
        diff->AddTerm(temp);
        current_x = current_x->next;
    }

    while(current_y != NULL)
    {
        Monomial *temp = new Monomial;

        temp->coeff =- current_y->coeff;
        temp->exp = current_y->exp;
        diff->AddTerm(temp);
        current_y = current_y->next;
    }

    return *diff;
}

RarePolynomial& operator*(RarePolynomial& X, RarePolynomial& Y)
{
    RarePolynomial *prod = new RarePolynomial;
    Monomial *current_x = X.front;

    while(current_x != NULL)
    {
        Monomial *current_y=  Y.front;

        while(current_y != NULL)
        {
            Monomial *temp = new Monomial;
            temp->coeff = current_x->coeff * current_y->coeff;
            temp->exp = current_x->exp + current_y->exp;
            prod->AddTerm(temp);
            current_y = current_y->next;
        }
        current_x = current_x->next;
    }

    return *prod;
}

int Menu(RarePolynomial& X, RarePolynomial& Y) /// meniu
{
    int action;
    Back:

    cout<<"Polynomials current introduced: \n";
    cout<<"A: "<<X<<"\n"<<"B: "<<Y<<"\n\n";
    cout<<"Please choose from the following actions: \n\n";
    cout<<"1. Introduce the first polynomial: \n";
    cout<<"2. Introduce the second polynomial: \n";
    cout<<"3. Calculate the value of polynomials in a given point. \n";
    cout<<"4. Calculate the sum of introduced polynomials. \n";
    cout<<"5. Calculate the difference of introduced polynomials. \n";
    cout<<"6. Calculate product of introduced polynomials. \n";
    cout<<"7. Exit program.\n\n";
    cout<<"Select action... ";cin>>action;

    if((X.IsEmpty() || Y.IsEmpty()) && action>3)
    {
        Clear();
        cout<<"You cannot perform action "<<action<<" without having introduced two polynomials.";

        if(X.IsEmpty())
            if(Y.IsEmpty())
                cout<<"Please introduce both polynomials.";
            else
                cout<<"Please introduce the first polynomial.";
        else
            cout<<"Please introduce the second polynomial.";

        cout<<"\nPress any key to return to previous menu...";

        Pause();
        Clear();
        goto Back;
    }
    else
        if(X.IsEmpty() && Y.IsEmpty() && action==3)
        {
            Clear();
            cout<<"You cannot perform action 3 without any polynomial introduced. Please introduce at least one to proceed.";
            cout<<"\nPress any key to return to previous menu...";

            Pause();
            Clear();
            goto Back;
        }

    return action;
}

void ReadPoly(RarePolynomial& X) /// citire polinom
{
    int action;

    do
    {
        Clear();
        X.~RarePolynomial();
        cout<<"(Please type a pairs containing (coefficient,exponent); you can end input by typing a null coefficient)\n\n";
        cin>>X;

        Clear();
        cout<<"You introduced: "<<X<<"\n\nDo you want to modify? \n1.Yes\n2.No\n";
        cout<<"\nSelect action... ";
        cin>>action;

        while(action<1 && action>2)
        {
            cout<<"Incorrect input. Please type 1 or 2. \n";
            cout<<"\nSelect action... ";
            cin>>action;
        }
    }while(action==1);
    Clear();
}

void ValueInX(RarePolynomial& X, RarePolynomial& Y) /// calcularea valorii unui polinom intr-un punct
{
    int action, value;
    Clear();
    cout<<"A: "<<X<<"\n"<<"B: "<<Y<<"\n\n";
    cout<<"You have selected action 3: Calculate the value of polynomials in a given point. Do you wish to continue?\n1.Yes\n2.No\n";
    cout<<"\nSelect action... ";
    cin>>action;

    while(action<1 || action>2)
    {
        cout<<"Incorrect input. Please type 1 or 2. \n";
        cout<<"\nSelect action... ";
        cin>>action;
    }

    if(action == 1)
    {
        Back:
        Clear();
        cout<<"A: "<<X<<"\n"<<"B: "<<Y<<"\n\n";
        cout<<"Which polynomial do you wish to use for this action? \n\n";
        cout<<"Type 1 for the first polynomial, 2 for the second. \n";
        cout<<"\nSelect action... ";
        cin>>action;

        while(action<1 || action>2)
        {
            cout<<"Incorrect input. Please introduce a value between 1 and 2. \n";
            cout<<"Select action... \n";
            cin>>action;
        }

        Clear();

        if(action == 1)
        {
            if(X.IsEmpty())
            {
                cout<<"You haven't introduced any values for this polynomial. Try with the second polynomial.";
                cout<<"\nPress any key to return to previous menu...";
                Pause();
                goto Back;
            }
            else
            {
                cout<<"What value do you want to calculate the polynomial "<<X<<" in? \n";
                cout<<"Awaiting value... ";
                cin>>value;
                Clear();

                cout<<"The value of the polynomial "<<X<<" in "<<value<<" is "<<X.Value(value)<<".\n";
                cout<<"\nPress any key to return to previous menu...";
                Pause();
                Clear();
            }
        }
        else
        {
            if(Y.IsEmpty())
            {
                cout<<"You haven't introduced any values for this polynomial. Try with the first polynomial.";
                cout<<"\nPress any key to return to previous menu...";
                Pause();
                goto Back;
            }
            else
            {
                cout<<"What value do you want to calculate the polynomial "<<Y<<" in? \n";
                cout<<"Awaiting value... ";
                cin>>value;
                Clear();
                cout<<"The value of the polynomial "<<Y<<" in "<<value<<" is "<<Y.Value(value)<<".\n";
                cout<<"\nPress any key to return to previous menu...";
                Pause();
                Clear();
            }
        }
    }
    else
    {
        cout<<"\nPress any key to return to previous menu...";
        Pause();
        Clear();
    }
}

void SumPoly(RarePolynomial& X, RarePolynomial&Y)
{
    int action;
    Clear();
    cout<<"Polynomials current introduced: \n";
    cout<<"A: "<<X<<"\n"<<"B: "<<Y<<"\n\n";
    cout<<"You have selected action 4: Calculate the sum of introduced polynomials. Do you wish to continue?\n1.Yes\n2.No\n";
    cout<<"\nSelect action... ";
    cin>>action;
    cout<<"\n";

    while(action<1 || action>2)
    {
        cout<<"Incorrect input. Please type 1 or 2. \n";
        cout<<"\nSelect action... ";
        cin>>action;
        cout<<"\n";
    }

    Clear();
    if(action==1)
    {
        RarePolynomial Z=X+Y;
        cout<<"The sum of "<<X<<" and "<<Y<<" is "<<Z;
        cout<<"\n\nPress any key to return to previous menu...";
        Pause();
        Clear();
    }
    else
    {
        cout<<"\nPress any key to return to previous menu...";
        Pause();
        Clear();
    }
}

void DiffPoly(RarePolynomial& X, RarePolynomial& Y)
{
    int action;
    Clear();
    cout<<"Polynomials current introduced: \n";
    cout<<"A: "<<X<<"\n"<<"B: "<<Y<<"\n\n";
    cout<<"You have selected action 5: Calculate the difference of introduced polynomials. Do you wish to continue?\n1.Yes\n2.No\n";
    cout<<"\nSelect action... ";
    cin>>action;

    while(action<1 || action>2)
    {
        cout<<"Incorrect input. Please type 1 or 2. \n";
        cout<<"\nSelect action... ";
        cin>>action;
    }

    Clear();
    if(action==1)
    {
        RarePolynomial Z=X-Y;
        cout<<"The difference of "<<X<<" and "<<Y<<" is "<<Z;
        cout<<"\n\nPress any key to return to previous menu...";
        Pause();
        Clear();
    }
    else
    {
        cout<<"\nPress any key to return to previous menu...";
        Pause();
        Clear();
    }
}

void MultPoly(RarePolynomial& X, RarePolynomial& Y)
{
    int action;
    Clear();
    cout<<"Polynomials current introduced: \n";
    cout<<"A: "<<X<<"\n"<<"B: "<<Y<<"\n\n";
    cout<<"You have selected action 6: Calculate the product of introduced polynomials. Do you wish to continue?\n1.Yes\n2.No\n";
    cout<<"\nSelect action... ";
    cin>>action;
    cout<<"\n";

    while(action<1 || action>2)
    {
        cout<<"Incorrect input. Please type 1 or 2. \n";
        cout<<"\nSelect action... ";
        cin>>action;
        cout<<"\n";
    }

    Clear();
    if(action==1)
    {
        RarePolynomial Z=X*Y;
        cout<<"The product of "<<X<<" and "<<Y<<" is "<<Z;
        cout<<"\n\nPress any key to return to previous menu...";
        Pause();
        Clear();
    }
    else
    {
        cout<<"\nPress any key to return to previous menu...";
        Pause();
        Clear();
    }
}

int main()
{
    int action;
    RarePolynomial A,B,C;

    while("")
    {
        action=Menu(A,B);

        switch(action)
        {
            case 1:
                ReadPoly(A);
                break;

            case 2:
                ReadPoly(B);
                break;

            case 3:
                ValueInX(A,B);
                break;

            case 4:
                SumPoly(A,B);
                break;

            case 5:
                DiffPoly(A,B);
                break;

            case 6:
                MultPoly(A,B);
                break;

            case 7:
                Clear();
                cout<<"Are you sure you want to exit?\n1.Yes\n2.No\n\nSelect action... ";cin>>action;
                if(action==1)
                    goto end;
                else
                {
                    Clear();
                    cout<<"Press any key to return to previous menu... ";
                    Pause();
                }
                break;

            default:
                cout<<"Incorrect input. Please introduce a value between 1 and 7. \n";
                cout<<"Press any key to return to previous menu... ";

                Pause();
                Clear();
        }
    }

    end:
    cout<<"Program exited succesfully.";

    return 0;
}
