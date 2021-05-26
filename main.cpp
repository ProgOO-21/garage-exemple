#include <iostream>

struct HasColor
{
    int getColor() = 0;
};

struct Vehicle
{
    int id;
};

struct Camion : public Vehicle, public HasColor
{
    int essieu;

    int getColor() override
    {
        return 2;
    }
};

struct Test
{
    void test(HasColor c)
    {
        cout << "Nous allons tester la couleur " << endl;
        cout << "Le résultat est " << obj.getColor() << endl;
    }
}

class Garage
{
    bool exist(int id)
    {
        return v != nullptr && v->id == id;
    }

    Vehicle &remove(int id)
    {
        if (!exist(id))
        {
            return nullptr;
        }
        // ne pas oublier de retirer du stock
        return v;
    }

    void printList(User &u)
    {
        SuperUser su * = dynamic_cast<SuperUser>(u);
        if (su != nullptr)
        {
            cout << "C'est un super user" << endl;
        }
    }

    Test &getTest()
    {
        return t;
    }

  private:
    Vehicle *v;
    Test t;
};

enum Role
{
    no,
    admin,
    super,
    empl
};

struct User
{
    User(Role role) : role(role) {}
    Role role;
};

struct SuperUser : public User
{
    SuperUser() : User(Role::super) {}
    Vehicle *v;

    void buy(int id) {}
};

struct Employer : public User
{
    Vehicle &sell(int id)
    {
        if (g->exist(id))
        {
            cout << "La voiture est : " << endl;
        }
    }

    void tester()
    {
        Test &t = g->getTest();
    }

    Garage *g;
};

int main() {}