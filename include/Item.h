#ifndef ITEM_H
#define ITEM_H
#include <string>
#include "AnimatedNode.h"
#include <irrlicht/irrlicht.h>
#include <iostream>

#define DEFAULT_ITEM_LEVEL 1
#define DROP_ANIMATION 0,39

using namespace std;
using namespace irr;
using namespace irr::scene;
using namespace irr::video;


class Item : public AnimatedNode, public ISceneNode {
    private:
        int level_;
        int ID_;
        std::string name_;
        std::string modelPath_;
        std::string imagePath_;
        static int id_;
        

        int generateID() { return id_++; }

    protected:
        ITexture* image_;
    public:

        std::string getModelPath(){return modelPath_;}
        std::string getImagePath(){return imagePath_;}
        int getLevel() const { return level_; }
        int getID() const { return ID_; }
        const std::string getNamex() const {  return name_; }
        ITexture* getImage() {return image_; }
        void setImage(ITexture* image) { image_ = image; }

        virtual void render() {}
        virtual const core::aabbox3d<f32>& getBoundingBox() const { }

        Item(ISceneNode * parent,
             ISceneManager * manager,
             const std::string name,
             const char * modelPath,
             const char* imagePath,
             int level = DEFAULT_ITEM_LEVEL
             );

        virtual Item* copy(ISceneNode * parent = NULL,
                   ISceneManager * manager = NULL);       
        
        virtual ~Item();

};

#endif // ITEM_H
