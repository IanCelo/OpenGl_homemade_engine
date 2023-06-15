#pragma once


//  Singleton class
template<typename T> class Singleton
{
private:
    static T* singleton;

protected:
    Singleton() {};
    ~Singleton() {};

public:

    //  Static function used to get the instance of a singleton if it exist or create it if not
    static T* instance()
    {
        if (singleton == nullptr)
        {
            singleton = new T;
        }

        //  Return the instanciated singleton
        return (static_cast<T*>(singleton));
    }


    //  Static function used to kill an existing instance of singleton
    static void kill()
    {
        if(nullptr != singleton)
        {
            delete singleton;
            singleton = nullptr;
        }
    }
};

template<typename T>
T* Singleton<T>::singleton = nullptr;