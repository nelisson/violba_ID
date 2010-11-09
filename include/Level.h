#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "Object.h"


using namespace std;

class Level : public Node {
    private:
        vector<Object*> objects_;

    protected:

    public:


        Level();
        virtual ~Level();
};

#endif // LEVEL_H
