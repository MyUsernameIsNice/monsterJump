#ifndef PLATFORM_H
#define PLATFORM_H

class Platform
{
    public:
        Platform(float posX, float posY, float posZ){}
        float getPosX() { return posX; }
        float getPosY() { return posY; }
        float getPosZ() { return posZ; }
        virtual ~Platform();
    private:
        float posX;
        float posY;
        float posZ;
};

#endif // PLATFORM_H
