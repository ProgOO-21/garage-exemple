#include <iostream>

using namespace std;

class MyException : public exception
{
  public:
    MyException(string const& phrase = "") throw() : phrase(phrase) {}

    virtual const char* what() const throw()
    {
        return phrase.c_str();
    }

    // méthode virtuelle donc destructeur virtuel
    virtual ~MyException() throw() {}

  private:
    string phrase;
};

enum Role
{
    no,
    user,
    admin
};

struct User
{
    virtual ~User() {}

    Role getRole()
    {
        return role;
    }

  protected:
    Role role = Role::no;
};

// interface
struct IPrintable
{
    virtual void print() = 0;
};

// un interface peu hériter d'un autre interface
struct HasColor : public IPrintable
{
    virtual int getColor() = 0;
};

struct Vehicle
{
    virtual ~Vehicle() {}

    virtual string getInfo()
    {
        return "  - id : " + std::to_string(id) +
               "\n"
               "  - price : " +
               std::to_string(price) + "\n";
    }

    int id;
    int price;
    string brand;
};

struct Camion : public Vehicle, public HasColor
{
    int essieu;

    virtual string getInfo()
    {
        return Vehicle::getInfo() + "  - essieu : " + std::to_string(essieu) + "\n";
    }

    int getColor() override
    {
        // pas besoin d'implémenter une classe couleur, faire simple.
        // Idem pour niveau ou suspension
        return 2; 
    }

    void print(){
        cout << "Implementation de l'interface print dans camion" << endl;
    }
};

// possibilité par héritage, est-un ...
struct OptionA
{
    void checkOptionA(Vehicle* v)
    {
        cout << "Test option A " << v->brand << endl;
    }

    // utilisation d'un interface
    void checkOptionAInt(IPrintable* p){
        p->print();
    }
};

// possibilité par composition, contient un ...
struct OptionB
{
    void checkOption(Vehicle* v)
    {
        cout << "Test option B " << v->brand << endl;
    }

    void checkOptionBInt(IPrintable* b){
        b->print();
    }
};

struct Test : public OptionA
{
    // 2 possibilités d'implémentation, passe le véhicule ou l'interface en paramètre
    void test(HasColor* obj)
    {
        cout << "Nous allons tester la couleur " << endl;
        cout << "Le résultat est " << obj->getColor() << endl; // suffisant pour un test
        checkOptionAInt(obj); // autorisé car hasColor est un printable
    }

    void test1(Vehicle* v)
    {
        if (auto res = dynamic_cast<HasColor*>(v))
        {
            cout << "Un objet de type HasColor est testé" << endl;
            checkOptionA(v);
            checkOptionAInt(res); // autorisé car hasColor est un printable
            ob.checkOption(v);
            ob.checkOptionBInt(res); // autorisé car hasColor est un printable
            cout << "Nous allons tester la couleur " << endl;
            cout << "Le résultat est " << res->getColor() << endl;
        }
    }

    OptionB ob; // ajout d'une option par composition
};

struct SuperUser : public User
{
    SuperUser()
    {
        role = Role::user;
    }

    void buy(Vehicle* v)
    {
        this->v = v;
        cout << "Mon compte c'est vidé de " << v->price << " !!!!" << endl;
    }
    Vehicle* v;
};

class Garage
{
  public:
    Garage()
    {
        // ce code est ici pour la démo
        Vehicle* c = new Camion();
        c->id = 5;
        c->brand = "Ford";
        c->price = 123456;
        v[0] = c;
    }

    Vehicle* getVehicle(int id)
    {
        int i = 0;
        while (v[i] != nullptr && i < 4)
        {
            if (v[i]->id == id)
            {
                return v[i];
            }
            i++;
        }
        throw MyException("Vehicle not found");
    }

    Vehicle* remove(int id)
    {
        Vehicle* res = getVehicle(id);
        // retirer du stock ( attention si pointeur est au millieu du tableau )
        return res;
    }

    void printList(User& u)
    {
        if (SuperUser* su = dynamic_cast<SuperUser*>(&u))
        {
            cout << "C'est un super user" << endl;
            cout << getVehicle(5)->getInfo() << endl;
        }
        else
        {
            cout << "C'est un user" << endl;
        }
    }

    Vehicle** getList(User& u)
    {
        if (u.getRole() < Role::user)
        {
            throw MyException("Ask permission!!!");
        }
        return this->v;
    }

  private:
    Vehicle* v[4] = {nullptr};
};

struct Employer : public User
{
    Employer()
    {
        role = Role::admin;
    }

    void sell(int id, SuperUser& u)
    {
        try
        {
            // nous allons le voir au cours, le type auto est un type automatique en fonction
            // de l'affectation
            auto v = g->getVehicle(id);
            cout << "La voiture est : " << endl;
            u.buy(v);
        }
        catch (...)
        {
            cout << "Houston..." << endl;
        }
    }

    void tester()
    {
        Vehicle** list = g->getList(*this);
        int i = 0;
        // les boucles ne sont pas implémentées correctement
        while (list[i] != nullptr && i < 4)
        {
            // possibilité 1
            if (auto res = dynamic_cast<HasColor*>(list[i]))
            {
                cout << "Un objet de type HasColor est testé" << endl;
                t.test(res);
            }

            // possibilité 2
            t.test1(list[i]);

            i++;
        }
    }

    Garage* g;
    Test t;
};

// il serait aussi possible de mettre l'action acheter dans une fonction
// plutôt que dans la méthode sell
void sell(int id, SuperUser& su, Employer& em)
{
    // auto v = em.sell(id);
    // su.buy(v);
}

int main()
{
    Garage g;
    User u;
    SuperUser su;
    Employer em;

    g.printList(u);
    em.tester();

    // choix d'un article après avoir vu la liste
    g.printList(su);
    int idChoose = 5;
    em.sell(idChoose, su);
    // il serait possible de croiser l'acheteur et le vendeur en modifiant les méthodes
    // su.buy(idChoose, em)

    try
    {
        g.getList(u);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}