#include <iostream>
#include <math.h>
#include <algorithm>
#include <typeinfo>
#include <vector>

using namespace std;

void Pause()
{
    cout<<"Please press any key to return to previous menu... ";
    cin.get();
    cin.get();

}

void Clear()
{
#if defined _WIN32
    system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
#elif defined (__APPLE__)
    system("clear");
#endif
}

class Monomial
{
    private:
        int deg;
        int coeff;
    public:
        friend class Polynomial;
        friend class ReduciblePolynomial;
        Monomial(int deg=0,int coeff=0)
        {
            this->deg=deg;
            this->coeff=coeff;
        }

        Monomial(const Monomial& mon)
        {
            this->deg=mon.deg;
            this->coeff=mon.coeff;
        }

        ~Monomial()
        {
            this->deg=0;
            this->coeff=0;
        }

        friend bool operator<(Monomial& x,Monomial&y)
        {
            return x.deg>y.deg;
        }

        friend istream& operator>>(istream& in, Monomial& mon)
        {
            cout<<"Read coefficient of current term: ";
            in>>mon.coeff;
            cout<<"Read degree of current term: ";
            in>>mon.deg;

            return in;
        }

        friend ostream& operator<<(ostream& out, Monomial& mon)
        {
            if(mon.coeff == 1 || mon.coeff == -1)
                if(mon.deg == 0)
                    out<<"1";
                else
                    if(mon.deg != 1)
                        out<<"x^"<<mon.deg;
                    else
                        out<<"x";
            else
                if(mon.deg == 0)
                    out<<mon.coeff;
                else
                    if(mon.deg!=1)
                        out<<mon.coeff<<"x^"<<mon.deg;
                    else
                        out<<mon.coeff<<"x";

            return out;
        }
};

class Polynomial
{
    protected:
        int nr_terms;
        Monomial *m;

    public:
        static int s;
        Polynomial(int nr_terms=0) /// initialization constructor
        {
            this->nr_terms=nr_terms;
            m=new Monomial[this->nr_terms];
        }

        Polynomial(const Polynomial& Poly) /// copy constructor
        {
            delete m;
            this->nr_terms=Poly.nr_terms;
            m=new Monomial[Poly.nr_terms];
            for(int i=0;i<Poly.nr_terms;i++)
                m[i]=Poly.m[i];
        }

        Polynomial& operator=(const Polynomial& Poly)
        {
            delete m;
            this->nr_terms=Poly.nr_terms;
            m=new Monomial[Poly.nr_terms];
            for(int i=0;i<Poly.nr_terms;i++)
                m[i]=Poly.m[i];
            return *this;
        }

        ~Polynomial() ///destructor
        {
            delete m;
            nr_terms=0;
        }

        virtual void set_nr_terms(int nr_terms)
        {
            delete[] m;
            this->nr_terms=nr_terms;
            this->m=new Monomial[this->nr_terms];
        }

        virtual double ValueInX(double x)
        {
            double s=0;
            for(int i=0;i<nr_terms;i++)
            {
                s=s+m[i].coeff*pow(x,m[i].deg);
            }
            return s;
        }

        virtual bool irreducibility()
        {
            if(m[0].deg==1)
                return true;
            if(m[0].deg==0)
            {
                if(m[0].coeff!=0)
                    return true;
                else
                    return false;
            }
            if(m[nr_terms-1].deg!=0)
            {
                return false;
            }
            else
                {
                    for(int i=1;i<=m[nr_terms-1].coeff;i++)
                    {

                        if(m[nr_terms-1].coeff%i==0)
                            for(int j=1;j<=m[0].coeff;j++)
                                if(m[0].coeff%j==0)
                                {
                                    if(ValueInX(i/j)==0)
                                    {
                                        return false;
                                    }
                                    if(ValueInX(-i/j)==0)
                                    {
                                        return false;
                                    }
                                }
                    }
                    return true;
                }
        }

        virtual bool eisenstein()
        {

            if(m[nr_terms-1].deg!=0)
            {
                for(int i=2;i<m[nr_terms-1].coeff;i++)
                {
                    int d=0;
                    int ok=1;
                    if(m[nr_terms-1].coeff%i==0)
                    {
                        for(int j=2;j*j<=i;j++)
                        {
                            if(i%j==0)
                                d++;
                        }
                        if(d==0)
                        {
                            for(int k=1;k<nr_terms-2;k++)
                                if(m[k].coeff%i!=0)
                                    ok=0;
                            if(m[0].coeff%i==0)
                                ok=0;
                            if((m[nr_terms-1].coeff)%(i*i)==0)
                                ok=0;
                            if(ok==1)
                                return true;
                        }
                    }
                }
            }

            return false;
        }

        virtual void read(istream& in)
        {
            int x;
            Monomial mon;
            int static s;

            if(nr_terms==0)
            {
                cout<<"Introduce the degree of polynomial #"<<s+1<<"(+1):";
                cin>>x;
                while(x<0)
                {
                    cout<<"Degree cannot be negative";
                    cin>>x;
                }
                if(x==0)
                {
                    set_nr_terms(1);
                    cout<<"Read coefficient of current term: ";
                    in>>m[0].coeff;
                    return;
                }
                else
                    set_nr_terms(x);
            }
            else
            {
                cout<<"This polynomial has been already initialized and its degree is "<<nr_terms<<".";
            }

            int actual_nr_of_terms=nr_terms;
            Monomial *p=new Monomial[nr_terms];

            for(int i=0;i<nr_terms;i++)
            {
                int not_here=1;
                in>>mon;
                if(mon.coeff!=0)
                {
                    for(int j=0;j<i;j++)
                        if(p[j].deg==mon.deg)
                            {
                                actual_nr_of_terms--;
                                not_here=0;
                                if(p[j].coeff+mon.coeff!=0)
                                    p[j].coeff+=mon.coeff;
                                else
                                {
                                    actual_nr_of_terms--;
                                    p[j].coeff+=mon.coeff;
                                }
                            }
                    if(not_here==1)
                        p[i]=mon;
                }
                else
                    actual_nr_of_terms--;
            }

            if(nr_terms!=actual_nr_of_terms)
            {
                delete[] m;
                m=new Monomial[actual_nr_of_terms];
            }
            int k=0;
            for(int i=0;i<nr_terms;i++)
            {
                if(p[i].coeff!=0)
                    m[k++]=p[i];
            }
            nr_terms=actual_nr_of_terms;
            sort(m,m+actual_nr_of_terms);
            s++;
        }

        friend istream& operator>>(istream& in, Polynomial& Poly)
        {
            Poly.read(in);
            return in;
        }

        virtual void write(ostream& out)
        {
            if(nr_terms==0)
            {
                out<<"Empty polynomial "<<endl;
            }
            else
            {
                out<<"Polynomial: ";
                for(int i=0;i<nr_terms-1;i++)
                {
                    out<<m[i]<<"+";

                }
                out<<m[nr_terms-1];
                out<<"\n";
            }
        }

        friend ostream& operator<<(ostream& out, Polynomial& Poly)
        {
            Poly.write(out);
            return out;
        }
};

class IrreduciblePolynomial:public Polynomial
{
    private:
        string PolyType;
    public:
        IrreduciblePolynomial(int nr_terms=0):Polynomial(nr_terms)
        {
            this->PolyType="Irreducible";
        }

        IrreduciblePolynomial& operator=(const IrreduciblePolynomial& Poly)
        {
            delete m;
            this->nr_terms=Poly.nr_terms;
            m=new Monomial[Poly.nr_terms];
            for(int i=0;i<Poly.nr_terms;i++)
                m[i]=Poly.m[i];
            return *this;
        }

        IrreduciblePolynomial(const IrreduciblePolynomial& p):Polynomial(p)
        {
            this->PolyType="Irreducible";
        }

        ~IrreduciblePolynomial()
        {
            this->PolyType="\0";
        }

        void read(istream& in)
        {
            Polynomial::read(in);
        }

        void write(ostream& out)
        {
            out<<PolyType<<" ";
            Polynomial::write(out);
        }

        friend istream& operator>>(istream& in, IrreduciblePolynomial& Poly)
        {
            Poly.read(in);
            return in;
        }

        friend ostream& operator<<(ostream& out, IrreduciblePolynomial& Poly)
        {
            Poly.write(out);
            return out;
        }

};

class ReduciblePolynomial:public Polynomial
{
    private:
        string PolyType;
    public:

        ReduciblePolynomial(int nr_terms=0):Polynomial(nr_terms)
        {
            this->PolyType="Reducible";
        }

        ReduciblePolynomial& operator=(const ReduciblePolynomial& Poly)
        {
            delete m;
            this->nr_terms=Poly.nr_terms;
            m=new Monomial[Poly.nr_terms];
            for(int i=0;i<Poly.nr_terms;i++)
                m[i]=Poly.m[i];
            return *this;
        }

        ReduciblePolynomial(const ReduciblePolynomial& p):Polynomial(p)
        {
            this->PolyType="Reducible";
        }

        ~ReduciblePolynomial()
        {
            this->PolyType="\0";
        }

        void read(istream& in)
        {
            Polynomial::read(in);
        }

        void factorisation()
        {
            int a=0,b=0,c=0;

            for(int i=0;i<nr_terms;i++)
            {
                if(m[i].deg==2)
                    a=m[i].coeff;
                if(m[i].deg==1)
                    b=m[i].coeff;
                if(m[i].deg==0)
                    c=m[i].coeff;
            }
            double d=b*b-4*a*c;
            double x1,x2;
            if(d>0)
            {
                x1=-b-sqrt(d);
                x1=x1/(2*a);
                x2=-b+sqrt(d);
                x2=x2/(2*a);
                if(x1>0)
                {
                    if(x2>0)
                        cout<<"(x-"<<x1<<")(x-"<<x2<<")"<<endl;
                    else
                        if(x2<0)
                            cout<<"(x-"<<x1<<")(x+"<<abs(x2)<<")"<<endl;
                        else
                            cout<<"(x-"<<x1<<")x"<<endl;
                }
                else
                {
                    if(x1<0)
                    {
                        if(x2>0)
                            cout<<"(x+"<<abs(x1)<<")(x-"<<x2<<")"<<endl;
                        else
                            if(x2<0)
                                cout<<"(x+"<<abs(x1)<<")(x+"<<abs(x2)<<")"<<endl;
                            else
                                cout<<"(x+"<<abs(x1)<<")x"<<endl;
                    }

                    else
                        {
                            if(x2>0)
                                cout<<"x(x-"<<x2<<")"<<endl;
                            else
                                if(x2<0)
                                    cout<<"x(x+"<<abs(x2)<<")"<<endl;
                                else
                                    cout<<m[0].coeff<<"*x*x"<<endl;
                        }
                }

            }
            else
                if(d==0)
                {
                    if(nr_terms!=1)
                    {
                        x1=-b;
                        x1=x1/(2*a);
                        if(x1>0)
                            cout<<"(x-"<<x1<<")^2"<<endl;
                        else
                            if(x1<0)
                                cout<<"(x+"<<abs(x1)<<")^2"<<endl;
                            else
                                cout<<m[0].coeff<<"*x*x"<<endl;
                    }
                    else
                        cout<<m[0].coeff<<"*x*x"<<endl;
                }

        }

        friend istream& operator>>(istream& in, ReduciblePolynomial& Poly)
        {
            Poly.read(in);
            return in;
        }

        void write(ostream& out)
        {
            out<<PolyType<<" ";
            Polynomial::write(out);
            if(m[0].deg==2)
            {
                out<<"Factorisation: ";
                factorisation();
            }
        }

        friend ostream& operator<<(ostream& out, ReduciblePolynomial& Poly)
        {
            Poly.write(out);
            return out;
        }

};

void type(Polynomial *&p)
{
    p=new Polynomial;
    Polynomial *temp=new Polynomial;
    cin>>*temp;
    if(temp->irreducibility())
    {
        p=new IrreduciblePolynomial;
        p=new IrreduciblePolynomial(*(IrreduciblePolynomial*)temp);
    }
    else
    {
        p=new ReduciblePolynomial;
        p=new ReduciblePolynomial(*(ReduciblePolynomial*)temp);
    }
}

int main()
{
    int action=-1, n, x;
    string y;

    cout<<"Please introduce the number of polynomials you wish to read: ";
    cin>>n;

    Polynomial **Poly=new Polynomial*[n]();

    for(int i=0;i<n;i++)
    {
        Polynomial *temp=new Polynomial;
        cin>>*temp;
        if(temp->irreducibility())
        {
            Poly[i]=new IrreduciblePolynomial;
            *Poly[i]=*(IrreduciblePolynomial*)temp;
        }
        else
        {
            Poly[i]=new ReduciblePolynomial;
            *Poly[i]=*(ReduciblePolynomial*)temp;
        }
    }
    Pause();
    Clear();

    while("")
    {

        cout<<"1. Print all polynomials to console/terminal. \n";
        cout<<"2. Apply Eisenstein's criterion to a polynomial. \n";
        cout<<"3. Downcast \n";
        cout<<"0. Exit. \n";
        cout<<"\nSelect option: ";
        cin>>action;


        while(action<0 || action>4)
        {
            cout<<endl;
            cout<<"Please introduce a number between 0 and 2:";
            cin>>action;
        }


        switch(action)
        {
            case 0:
                goto end1;

            case 1:

                for(int i=0;i<n;i++)
                {
                    cout<<*Poly[i];
                }

                Pause();
                Clear();
                break;

            case 2:

                cout<<"Please select which polynomial you wish to test: ";
                cin>>x;
                while(x<0 || x>n)
                {
                    cout<<"Please select a number from 1 to "<<n<<":";
                    cin>>x;
                }
                if(Poly[x-1]->eisenstein())
                {
                    cout<<*Poly[x-1];
                    cout<<endl<<"According to the Eisenstein criterion, selected polynomial is irreducible over Q"<<endl;
                }
                else
                {
                    cout<<*Poly[x-1];
                    cout<<endl<<"Irreducibility cannot be decided using the Eisenstein criterion"<<endl;
                }
                Pause();
                Clear();
                break;

            case 3:


                cout<<"Read a reducible/irreducible polynomial: i/r "<<endl;
                cin>>y;
                while(y!="i" && y!="r")
                {
                    cout<<"Please type i or r: ";
                    cin>>y;
                }
                if(y=="i")
                {
                    Polynomial *q=new Polynomial;
                    IrreduciblePolynomial *p=new IrreduciblePolynomial;
                    cin>>*q;
                    p=(IrreduciblePolynomial*)q;
                    cout<<*p;
                }
                else
                {
                    Polynomial *q=new Polynomial;
                    ReduciblePolynomial *p=new ReduciblePolynomial;
                    cin>>*q;
                    p=(ReduciblePolynomial*)q;
                    cout<<*p;
                }

                Pause();
                Clear();
                break;

            default:
                cout<<"Incorrect input. Please introduce a value between 0 and 2. \n";
                Pause();
                Clear();
        }
    }

    end1:
    cout<<"You have successfully exited the program.";

    return 0;
}

/* exemple

            intai se introduce numarul de termeni al polinomului (sau gradul+1, si termenii care nu exista se dau cu 0,0)
            nu conteaza ordinea, se sorteaza

            x^2+x

            x^2+x+1

            x^2+2x+1

            x^3+1

            */
