#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>

using namespace std;

//track for recursive DFS and iterative A*
bool printStart = false,
     printStop = false,
     idPrintStart = false,
     idPrintStop = false;

//track number of nodes expanded by depth first 
//and iterative deepening depth first
int dNumNodes = 0,
    idNumNodes = 0,
    asNumNodes = 0;

//heuristic threashold
int threshold = 9;

//track path searched by depth first and iterative
//deepening depth first
string dPath = "",
       idPath = "",
       asPath = "";

struct queue 
{//Construct Queue ADT

    vector<string> states;  //stores states in the form of a string

    vector<string> paths;   //stores the paths used to get to each state respectively

    void enqueue(string state, string path)
    {//add a state and path into the state queue and path queue
        states.push_back(state);
        paths.push_back(path);
    }

    string dequeue()
    {//pop item from queue and return it

        string temp;    //will store state at front of queue

        temp = states[0];   //save the state in the front of the queue into tmp


        for (int i = 0; i < states.size() - 1; ++i)
        {//shift every state one index forward in the queue
            states[i] = states[i + 1];
        }
            
        states.resize(states.size() - 1);   //lower the size of the queue


        return temp;    //return the state that used to be at the front
    }

    string getPath() 
    {//dequeue function for paths

        string path;    //will store path at the front of the queue

        path = paths[0];    //save the path at the front of the queue into path


        for (int i = 0; i < paths.size() - 1; ++i)
        {//shift every path one index forward in the queue
            paths[i] = paths[i + 1];
        }

        paths.resize(paths.size() - 1); //decrement the queue size

        return path;
    }

    bool isEmpty()
    {//return true if the queue is empty
        if (states.size() == 0)
        {
            return true;
        }
        return false;
    }
};

struct list
{//construct a list to store states and associated costs
    vector<string> states;  //vector to store states
    vector<int> costs;  //voector to store associated cost of each state
    void Push(string state, int cost)
    {//push a state and a cost onto the list
        states.push_back(state);
        costs.push_back(cost);
    }
    bool isEmpty()
    {//if the states list is empty, return true
        if (states.empty())
        {
            return true;
        }
        return false;
    }
    int findMin()
    {//return the index of the state with the least cost

        int min = 0;    //store the index of the minimun value
   
        for (int i = 0; i < costs.size(); ++i)
        {//checl against every element in the list
            if (costs[i] < costs[min])
            {//if the current value is less than the minimum value, set it as minimum
                min = i;
            }
        }

        return min; //return the minimum index
    }
    string popState(int n)
    {//pop the value from index n and return it

        string tmp = states[n]; //store the state at index n and store it in tmp

        for (int i = n; i < states.size() - 1; ++i)
        {//shift all states after n one index forward
            states[i] = states[i + 1];
        }

        states.resize(states.size() - 1);   //decrement the state list size

        return tmp; //return the value poped
    }
    int popCost(int n)
    {//pop the cost at index n from stack

        int tmp = costs[n]; //store the cost at index n in tmp

        for (int i = n; i < costs.size() - 1; ++i)
        {//shift all costs after n one index forward
            costs[i] = costs[i + 1];
        }

        costs.resize(costs.size() - 1); //decrement the cost list size

        return tmp; //return the cost poped
    }
};

bool checkLists(list open, list closed, string state, int cost)
{//checks if the given state is on the open or closed list

    int size = open.states.size();  //stores the size of the open list


    for (int i = 0; i < size; ++i)
    {//check every state in both lists
        if (!open.isEmpty())
        {//check if the list is open
            if (state == open.states[i])
            {//check is the state is the same as the state in the current position
                if (open.costs[i] > cost)
                {//check if the cost passed through is lower than the current cost
                    open.costs[i] = cost;   //update with lower cost
                }
                return true;    //the state already exists in the list
            }
        }


        if (!closed.isEmpty())
        {//check if the closed list is empty
            if (state == closed.states[i])
            {//check if the current state is the same as the state passed
                if (closed.costs[i] > cost)
                {//check if the passed cost is lower than the current cost
                    closed.costs[i] = cost; //update with lower cost
                }
                return true;    //the state exists in the list
            }
        }

    }
    return false;   //state does not exist in either lists
}

int OutOfPlace(string initialState, string goalState)
{//counts the number of values in the initial state that are not where they are in the goal state

    int numOut = 0; //stores the number of out of place values

    for (int i = 0; i < initialState.size(); ++i)
    {//check value in the state
        if (initialState[i] != goalState[i])
            ++numOut;   //if the value in the initial index does not match the goal, increment number of out of place
    }
    return numOut;  //return the number of out of place
}

int Manhattan(string initialState, string goalState)
{//calculate the sum of manhattan distances in the initial state

    int currx,  //store the x location of the initial state
        curry,  //store the y location of the initial state
        goalx,  //store the x location of the goal state
        goaly,  //store the y location of the goal state
        size = initialState.size(), //store the size of the puzzle
        j,  //store the index of the goal state
        sum = 0;    //store the sum of manhattan distances
    
    for (int i = 0; i < size; ++i)
    {//iterate through each value in the state
        if (size == 9)
        {//if the puzzle is the 8 puzzle

            currx = i / 3;  //calculate and store the x value of the current index
            curry = i % 3;  //calculate and store the y value of the current index

            for (j = 0; j < size; ++j)
            {//iterate through each value in the goal state
                if (goalState[j] == initialState[i])    //locate the index of the current value in the goal state
                    break;
            }

            goalx = j / 3;  //calculate and store the x value of the goal index
            goaly = j % 3;  //calculate and store the y value of the goal index

            int denom = abs(goaly - curry); //store the difference in y values

            if ( denom == 0)    //if the denominator is 0, set denominator to 1
                denom = 1;                

            sum += abs(goalx - currx) / denom;  //calculate and update the sum of manhattan distances
        }
        if (size == 16)
        {//if the puzzle is the 16 puzzle

            currx = i / 4;  //calculate and store the x value of the current index
            curry = i % 4;  //calculate and store the y value of the current index

            for (j = 0; j < size; ++j)
            {//iterate through each value of the goal state
                if (goalState[j] == initialState[i])    //locate the index of the current value in the goal state
                    break;
            }

            goalx = j / 4;  //calculate and store the x value of the goal index
            goaly = j % 4;  //calculate and store the y value of the goal index

            int denom = abs(goaly - curry); //store the difference in y values

            if (denom == 0)    //if the denominator is 0, set denominator to 1
                denom = 1;

            sum += abs(goalx - currx) / denom;  //calculate and update the sum of manhattan distances
        }
    }
    return sum; //return the sum of manhattan distances
}

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
{//given a state, return a state with the 0 slider moved up
    int size = state.size();    //stores the size of the puzzle

    bool eightPuzzle = true;  //stores the type of puzzle being used

    if (size == 16) //determine which puzzle is being used
        eightPuzzle = false;

    int slideIndex = ZeroIndex(state); //create a variable to id where the 0 in the puzzle is

    if (eightPuzzle)
    {//if the puzzle is the 8 puzzle
        if (slideIndex < 3) //if the 0 is at the top of the puzzle, the move is invalid
            return "";  //returns null state
        
        SwapChar(state, slideIndex, slideIndex - 3);    //swap the 0 with the value above it
    }
    else
    {//the puzzle is the 15 puzzle
        if (slideIndex < 4) //if th e0 is at the top of the puzzle, the move is invalid
            return "";  //return the null state

        SwapChar(state, slideIndex, slideIndex - 4);    //sawp the 0 with the value above it
    }

    return state;   //return the trasnformed state

}

string MoveDown(string state)
{//given a state, return a state with the slider moved down
    int size = state.size();    //store the size of the puzzle

    bool eightPuzzle = true;    //store the type of puzzle

    if (size == 16) //determine which puzzle is being used
        eightPuzzle = false;

    int slideIndex = ZeroIndex(state); //create a variable to id where the 0 in the puzzle is

    if (eightPuzzle)
    {//the puzzle is the eight puzzle
        if (slideIndex > 5) //if the 0 is at the botto of the puzzle, the move is invalid
            return "";  //return the null state

        SwapChar(state, slideIndex, slideIndex + 3);    //swap the 0 with the value below it
    }
    else
    {//the puzzle is the 15 puzzle
        if (slideIndex > 11)    //if the 0 is at the bottom of the puzzle, the move is invalid
            return "";  //return the null state

        SwapChar(state, slideIndex, slideIndex + 4);    //swap the 0 with the value below it
    }

    return state;   //return the transformed state
}

string MoveLeft(string state)
{//given a state, return a state with the slider moved left
    int size = state.size();    //store the size of the puzzle

    bool eightPuzzle = true;    //store the type of puzzle

    if (size == 16) //determine which puzzle is being used
        eightPuzzle = false;

    int slideIndex = ZeroIndex(state); //create a variable to id where the 0 in the puzzle is

    if (eightPuzzle)
    {//the puzzle is the 8 puzzle
        if (slideIndex == 0 || slideIndex == 3 || slideIndex == 6) //if the 0 is on the left, the move is invalid
            return "";  //return the null state

        SwapChar(state, slideIndex, slideIndex - 1);    //swap the 0 and the value to the left
    }
    else
    {//the puzzle is the 15 puzzle
        if (slideIndex == 0 || slideIndex == 4 || slideIndex == 8 || slideIndex == 12)  //if the 0 is on the left, the move is invalid
            return "";  //return the null state

        SwapChar(state, slideIndex, slideIndex - 1);    //swap the 0 and the value to the left
    }

    return state;   //return the transformed state
}

string MoveRight(string state)
{//given a state, return a state with the slider moved right
    int size = state.size();    //store the puzzle size

    bool eightPuzzle = true;    //store the puzzle type

    if (size == 16) //determine which puzzle is being used
        eightPuzzle = false;

    int slideIndex = ZeroIndex(state); //create a variable to id where the 0 in the puzzle is

    if (eightPuzzle)
    {//the puzzle is the 8 puzzle
        if (slideIndex == 2 || slideIndex == 5 || slideIndex == 8)  //if the slider is on the right, the move is invalid
            return "";  //return the null state

        SwapChar(state, slideIndex, slideIndex + 1);    //swap the slider with the value on the right
    }
    else
    {//the puzzle is the 15 puzzle
        if (slideIndex == 3 || slideIndex == 7 || slideIndex == 11 || slideIndex == 15) //if the slider is on the right, the move is invalid
            return "";  //return the null state

        SwapChar(state, slideIndex, slideIndex + 1);    //swap the slider with the value on the right
    }

    return state;   //return the transformed state
}

void BFS(string initialState, string goalState)
{//implement Breadth-First Search to solve the puzzle, given the initial state and the goal state

    int numNodes = 0;   //stores the number of nodes expanded during the search
    queue q;    //stores the list of states being explored
    string state,   //stores the current state
           path = "";   //stores the path searched to find the goal state

    cout << "Breadth First Search Starting...\n";

    q.enqueue(initialState, path);  //push the initial state on to the queue

    while (!q.isEmpty())
    {//while the queue is not empty, explore the states in the queue

        string currPath;    //stores the state poped off the queue

        state = q.dequeue();    //pop the state off the front of the queue and store it in state
        path = q.getPath();     //pop the path off the front of the queue and store it in path
        currPath = path;    //initialize the currPath to path

        ++numNodes; //increment the number of nodes expanded

        if (state == goalState) //if the state poped off the stack is the goal state, end the search
            break;

        string tmp = MoveUp(state); //transform the current state and save it in tmp

        if (tmp != "")
        {//the state is not null

            currPath += "u";    //update the path to reflect the new transformation
            q.enqueue(tmp, currPath);   //push the state and path onto the queue
            currPath = path;    //reset the currPath
        }       

        tmp = MoveDown(state);  //transform the current state and save it in tmp

        if (tmp != "")
        {//the state is not null

            currPath += "d";    //update the path to reflect the new transformation
            q.enqueue(tmp, currPath);   //push the state and path onto the queue
            currPath = path;    //reset the currPath
        }          

        tmp = MoveLeft(state);  //transform the current state and save it in tmp

        if (tmp != "")
        {//the state is not null

            currPath += "l";    //update the path to reflect the new transformation
            q.enqueue(tmp, currPath);   //push the state and path onto the queue
            currPath = path;    //reset the currPath
        }
 
        tmp = MoveRight(state); //transform the current state and save it in tmp

        if (tmp != "")
        {//the state is not null

            currPath += "r";    //update the path to reflect the new transformation
            q.enqueue(tmp, currPath);   //push the state and path onto the queue
            currPath = path;    //reset the currPath
        }
    }

    cout << "Breadth First Search Finished...Nodes Expanded: " << numNodes << ", Path Found: " << path << "\n";
    
}

string DFS(string initialState, string goalState, string path = "")
{//implement the depth first search

    dPath = path;   //save the path being searched into the current path

    ++dNumNodes;    //increment the number of nodes being expanded

    string state,   //store the current state
           currPath = path; //store the current path

    if (!printStart)
    {
        cout << "Depth First Search Starting...\n";
        printStart = true;
    }

    if (initialState != goalState)
    {//if the state being searched is not the goal state, explore the edges

        string tmp = MoveUp(initialState);  //transform the current state and save it in tmp

        if (tmp != "")
        {//the state is not null

            currPath += "u";  //update the path to reflect the transformation

            if (tmp == goalState)
            {//if the transformed state is the goal state, return the state
                dPath += 'u';   //update the path
                return tmp;     //return the state
            }

            tmp = DFS(tmp, goalState, currPath);    //search the new state from the transformation


            if (tmp == goalState)
            {  //is the new state is the goal state
                dPath += 'u';   //update the path
                return tmp; //return the state
            }

            currPath = path;    //reinit the path

        }

        if (tmp == goalState)   //is the goal state was found, skip the rest of the edges
            return tmp;

        tmp = MoveDown(initialState);
        if (tmp != "")
        {
            currPath += "d";
            if (tmp == goalState)
            {
                dPath += 'd';
                return tmp;
            }

            tmp = DFS(tmp, goalState, currPath);
            if (tmp == goalState)
            {
                dPath += 'd';
                return tmp;
            }

            currPath = path;

        }

        if (tmp == goalState)
            return tmp;

        tmp = MoveLeft(initialState);
        if (tmp != "")
        {
            currPath += "l";
            if (tmp == goalState)
            {
                   dPath += 'l';
                   return tmp;
            }

            tmp = DFS(tmp, goalState, currPath);
            if (tmp == goalState)
            {
                dPath += 'l';
                return tmp;
            }

            currPath = path;

        }

        if (tmp == goalState)
            return tmp;

        tmp = MoveRight(initialState);
        if (tmp != "")
        {
            currPath += "r";
            if (tmp == goalState)
            {
                dPath += 'r';
                return tmp;
            }

            tmp = DFS(tmp, goalState, currPath);
            if (tmp == goalState)
            {
                dPath += 'r';
                return tmp;
            }

            currPath = path;

        }
    }
    return "";  //no possible moves can be made
}

bool DLS(string initialState, string goalState, int limit)
{//implement depth limited search
    ++idNumNodes;   //increment the number of nodes expanded

    if (initialState == goalState)  //if the goal state was passed, return true
        return true;

    if (limit <= 0) //if the search limit has been met, end search
        return false;

    if (DLS(goalState, MoveUp(initialState), limit - 1))    //search the Up edge
    {//if the goal is found in the Up branch
        idPath += 'u';  // update the path to reflect the transformation
        return true;    //return the path was found
    }

    if (DLS(goalState, MoveDown(initialState), limit - 1))
    {
        idPath += 'd';
        return true;
    }

    if (DLS(goalState, MoveLeft(initialState), limit - 1))
    {
        idPath += 'l';
        return true;
    }

    if (DLS(goalState, MoveRight(initialState), limit - 1))
    {
        idPath += 'r';
        return true;
    }

    return false;   //path was not found
}

void idDFS(string initialState, string goalState)
{//implement iterative deepening depth first search

    cout << "Iterative Depth First Search Starting...\n";

    int maxDepth = 5;   //set the max search depth

    for (int i = 0; i < maxDepth; i++)
    {//iterate until max depth is met
        if (DLS(initialState, goalState, i))
        {//if the goal state was found, end search
            cout << "Iterative Depth First Search Finished...Nodes Expanded: " << idNumNodes << ", Path Found: " << idPath << "\n";
            return; //end search
        }
    }
    
    //goal was not found in the depth
    cout << "Iterative Depth First Searched Failed...Nodes Expanded: " << idNumNodes << ", Path Found: " << idPath << "\n";
    return; //end search
}

void AStar(string initialState, string goalState)
{//implement A* search

    cout << "A* Search Starting...\n";

    if (initialState == goalState)  //if the goal state was passed through 
        return;

    list open, closed;  //create a list to store the states being explored

    int minIndex,   //store the index of the minimum cost
        currCost,   //store the current states cost
        tmpCost,    //store the transformed cost
        numNodes = 0;   //store the number of nodes expanded

    string currState,   //store the current state
           tmp;     //store the transformed state

    open.Push(initialState, 0); //push the initial node onto the list

    while (!open.isEmpty())
    {//while the open list isnt empty

        ++numNodes; //increment the number of nodes explored

        minIndex = open.findMin();  //save the index of the minimum cost
        currState = open.popState(minIndex);    //pop the minimum cost state out of the list and store it in currState
        currCost = open.popCost(minIndex);  //pop the minimum cost out of the list and store it in currCost

        tmp = MoveUp(currState);    //transform the current state

        if (tmp == goalState)
        {//if the current state is the goal state, end search
            break;
        }
        else
        {

            tmpCost = OutOfPlace(currState, goalState) + Manhattan(currState, currState);//calc cost

            if (!checkLists(open, closed, tmp, tmpCost))
            {//if the current node is not in either list

                open.states.push_back(tmp); //push the transformed state onto the open list
                open.costs.push_back(tmpCost);  //push the transformed state onto the closed list
            }

        }

        tmp = MoveDown(currState);

        if (tmp == goalState)
        {
            break;
        }
        else
        {
            tmpCost = OutOfPlace(currState, goalState) + Manhattan(currState, currState);//calc cost
            if (!checkLists(open, closed, tmp, tmpCost))
            {
                open.states.push_back(tmp);
                open.costs.push_back(tmpCost);
            }
        }

        tmp = MoveLeft(currState);
        if (tmp == goalState)
        {
            break;
        }
        else
        {
            tmpCost = OutOfPlace(currState, goalState) + Manhattan(currState, currState);//calc cost
            if (!checkLists(open, closed, tmp, tmpCost))
            {
                open.states.push_back(tmp);
                open.costs.push_back(tmpCost);
            }
        }

        tmp = MoveRight(currState);
        if (tmp == goalState)
        {
            break;
        }
        else
        {
            tmpCost = OutOfPlace(currState, goalState) + Manhattan(currState, currState);//calc cost
            if (!checkLists(open, closed, tmp, tmpCost))
            {
                open.states.push_back(tmp);
                open.costs.push_back(tmpCost);
            }
        }

        closed.Push(currState, currCost);   //push the current state onto the closed list

    }

    cout << "A* Search Finished...Nodes Expanded: " << numNodes;    
}

int LFS(string initialState, string goalState, int limit)
{//Limited A* search

    ++idNumNodes;   //increment the number of nodes expanded

    if (initialState == goalState)  //if the goal state was passed, return no cost
        return 0;

    if (limit <= 0) //if the limit has beem reached, return large heuristic
        return 100;

    string tmpState = MoveUp(initialState); //transform the current state and save it in tmpState

    int tmp = LFS(goalState, tmpState, limit - 1);  //search the transformed state and return the cost

    if (tmp < threshold)
    {//if the cost is too high, dont search

        asPath += 'u';  //update path 
        return tmp + OutOfPlace(tmpState, goalState) + Manhattan(tmpState, goalState);  //calculate and return heuristic
    }

    tmpState = MoveDown(initialState);

    tmp = LFS(goalState, tmpState, limit - 1);

    if (tmp < threshold)
    {
        asPath += 'd';
        return tmp + OutOfPlace(tmpState, goalState) + Manhattan(tmpState, goalState);
    }

    tmpState = MoveLeft(initialState);

    tmp = LFS(goalState, tmpState, limit - 1);

    if (tmp < threshold)
    {
        asPath += 'l';
        return tmp + OutOfPlace(tmpState, goalState) + Manhattan(tmpState, goalState);
    }

    tmpState = MoveRight(initialState);

    tmp = LFS(goalState, tmpState, limit - 1);

    if (tmp < threshold)
    {
        asPath += 'r';
        return tmp + OutOfPlace(tmpState, goalState) + Manhattan(tmpState, goalState);
    }

    return 100; //return large heuristic
}

void idAStar(string initialState, string goalState, int iter = 0)
{//implement iterative deepening A*

    int maxDepth = 5;   //set the maximum iterations

    if (idPrintStart) 
    {//track is the statement has already been printed
        idPrintStart = true;
        cout << "Iterative Deepening A* Search Starting...\n";
    }
    
    for (int i = 0; i < maxDepth; i++)
    {//iterate through until max depth is met
        if (LFS(initialState, goalState, i))    //begin limited search
        {//if the state has been reached, stop the search
            cout << "Iterative Deepening A* Search Finished...Nodes Expanded: " << asNumNodes << ", Path Found: " << asPath << "\n";
            return;
        }
    }

    cout << "Iterative Deepening A* Searched Failed...Nodes Expanded: " << asNumNodes << ", Path Found: " << asPath << "\n";
}

int main()
{

    //input array

    string initialState,
           goalState;   //Creates a variable to store initial and goal state of the puzzle 

    cout << "Initial State: \n";

    initialState = PromptString();  //prompt user for initial state

    cout << "Goal State: \n";

    goalState = PromptString(); //prompt user for goal state

    //breadth first search
    thread tBFS(BFS, initialState, goalState);
    
    //depth first
    //thread tDFS(DFS, initialState, goalState, "");
    
    //iterative deepening depth first
    thread tidDFS(idDFS, initialState, goalState);
    
    //A* with out of place and manhattan distance hueristics
    thread tAStar(AStar, initialState, goalState);

    //iterative deepening A* w/ out of place and manhattan distance
    thread tidAStar(idAStar, initialState, goalState, 0);
    
    //join all threads
    tBFS.join();
    //tDFS.join();
    //cout << "Depth First Search Finished...Nodes Expanded: " << dNumNodes << ", Path Found: " << dPath << "\n";
    tidDFS.join();
    tAStar.join();
    tidAStar.join();

    return 0;
}

