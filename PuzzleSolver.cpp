#include <iostream>

using namespace std;

string PromptString()
{//prompt user for a valid array
    string userString;  //creates a string to store user input

    cout << "Enter your puzzle below in the following format: 012345678 or 0123456789abcdef\n";

    while (true)
    {//prompt for array until a valid array is given

        cin >> userString;   //prompt user for array

        if (userString.size() != 9 && userString.size() != 16)
        {//check if the puzzle is a valid size, if not, prompt for array again

            cout << "Invalid array size.\n";
            continue;
        }

        break;
    }

    return userString;
}

int ZeroIndex(string state)
{
    for (int i = 0; i < state.size(); ++i)
    {//identify where the 0 is
        if (state[i] == '0')
        {//store the value of the slide index
            return i;
        }
    }
}

void SwapChar(string& state, int first, int second)
{//swap two characters in a string
    char tmp;
    tmp = state[first];
    state[first] = state[second];
    state[second] = tmp;
}

string MoveUp(string state)
{
    int size = state.size();

    bool eightPuzzle = true;

    if (size == 16) //determine which puzzle is being used
        eightPuzzle = false;

    int slideIndex = ZeroIndex(state); //create a variable to id where the 0 in the puzzle is

    if (eightPuzzle)
    {
        if (slideIndex < 3)
            return "";

        SwapChar(state, slideIndex, slideIndex - 3);
    }
    else
    {
        if (slideIndex < 4)
            return "";

        SwapChar(state, slideIndex, slideIndex - 4);
    }

    return state;

}

string MoveDown(string state)
{
    int size = state.size();

    bool eightPuzzle = true;

    if (size == 16) //determine which puzzle is being used
        eightPuzzle = false;

    int slideIndex = ZeroIndex(state); //create a variable to id where the 0 in the puzzle is

    if (eightPuzzle)
    {
        if (slideIndex > 5)
            return "";

        SwapChar(state, slideIndex, slideIndex + 3);
    }
    else
    {
        if (slideIndex > 11)
            return "";

        SwapChar(state, slideIndex, slideIndex + 4);
    }

    return state;
}

string MoveLeft(string state)
{
    int size = state.size();

    bool eightPuzzle = true;

    if (size == 16) //determine which puzzle is being used
        eightPuzzle = false;

    int slideIndex = ZeroIndex(state); //create a variable to id where the 0 in the puzzle is

    if (eightPuzzle)
    {
        if (slideIndex == 0 || slideIndex == 3 || slideIndex == 6)
            return "";

        SwapChar(state, slideIndex, slideIndex - 1);
    }
    else
    {
        if (slideIndex == 0 || slideIndex == 4 || slideIndex == 8 || slideIndex == 12)
            return "";

        SwapChar(state, slideIndex, slideIndex - 1);
    }

    return state;
}

string MoveRight(string state)
{
    int size = state.size();

    bool eightPuzzle = true;

    if (size == 16) //determine which puzzle is being used
        eightPuzzle = false;

    int slideIndex = ZeroIndex(state); //create a variable to id where the 0 in the puzzle is

    if (eightPuzzle)
    {
        if (slideIndex == 2 || slideIndex == 5 || slideIndex == 8)
            return "";

        SwapChar(state, slideIndex, slideIndex + 1);
    }
    else
    {
        if (slideIndex == 3 || slideIndex == 7 || slideIndex == 11 || slideIndex == 15)
            return "";

        SwapChar(state, slideIndex, slideIndex + 1);
    }

    return state;
}


int main()
{
    string initialState,
           goalState,
           path;

    cout << "Initial State: \n";

    initialState = PromptString();

    cout << "Path:\n";

    cin >> path;

    while (!path.empty())
    {
        if (path[0] == 'u')
        {
            initialState = MoveUp(initialState);
        }
        else if (path[0] == 'd')
        {
            initialState = MoveDown(initialState);
        }
        else if (path[0] == 'l')
        {
            initialState = MoveLeft(initialState);
        }
        else if (path[0] == 'r')
        {
            initialState = MoveRight(initialState);
        }

        path.erase(0,1);
    }

    for (int i = 0; i < 9; ++i)
    {
        if (i % 3 == 0)
        {
            cout << "\n";
        }
        cout << initialState[i];
    }
    return 0;

}
