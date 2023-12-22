#include <iostream>
#include <Core/win/Window.h>

int main()
{
    Hydro::win::Window window;
    
    std::optional<int> quiting = {};
    while( !quiting )
    {
        quiting = window.ProcessMessages();

        //Do some cool game stuff here
    }

    std::cout << *quiting << std::endl;
}