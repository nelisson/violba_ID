#ifndef ITEM_H
#define ITEM_H
#include <string>

using namespace std;

class Item {
    private:
        int level_;
        int ID_;
        string name_;
        static int id_;
        int generateID();

    protected:

    public:
        int getLevel();
        int getID();
        string getName();

        Item(int level, string name);
        virtual ~Item();

};

#endif // ITEM_H
