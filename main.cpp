// Simple predator-prey simulation game
//
//Created by: Anastasia Sizensky


#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

const int GRID_SIZE = 20;
const int ANT = 2;
const int DOODLEBUG = 1;
const int INITIALIZED_ANTS = 100;
const int INITIALIZED_DOODLEBUGS = 5;
const int ANT_BREED = 3;
const int DOODLEBUG_BREED = 8;
const int DOODLEBUG_STARVE_TIME = 3;


class Critters;
class Doodlebug;
class Ant;


class Game
{
    friend class Critters;
    friend class Doodlebug;
    friend class Ant;

public:

    Game();
    ~Game();
    Critters* getLocation(int x, int y);
    void setLocation(int x, int y, Critters *critter);
    void showGrid();
    void oneTurn();

private:

    Critters* grid[GRID_SIZE][GRID_SIZE];
    //20 x 20 grid

};


class Critters
{
    friend class Game;

public:

    Critters();
    Critters(Game *game, int x, int y);

    virtual ~Critters();
    virtual void breed() = 0;
    virtual void move() = 0;
    virtual int getType() = 0;
    virtual bool starve() = 0;

protected:
    bool moved;
    int breedCounter;
    int x,y; //positions on the grid
    Game *game;

};


Game::Game()

{
    int i,j;

    for (i=0; i < GRID_SIZE; i++)

    {

        for (j=0; j < GRID_SIZE; j++)

        {

            grid[i][j]= nullptr;

        }

    }

}

Game::~Game()

{
    int i,j;

    for (i=0; i < GRID_SIZE; i++)

    {

        for (j=0; j < GRID_SIZE; j++)

        {

            if (grid[i][j]!= nullptr) delete (grid[i][j]);

        }

    }

}

Critters* Game::getLocation(int x, int y)

{

    if ((x>=0) && (x < GRID_SIZE) && (y >= 0) && (y < GRID_SIZE))

        return grid[x][y];

    return nullptr;

}

void Game::setLocation(int x, int y, Critters *critter)

{

    if ((x>=0) && (x < GRID_SIZE) && (y >= 0) && (y < GRID_SIZE))

    {

        grid[x][y] = critter;

    }

}


void Game::showGrid()

{

    int i,j;

    cout << endl << endl;

    for (j=0; j < GRID_SIZE; j++)

    {

        for (i=0; i < GRID_SIZE; i++)

        {

            if (grid[i][j]== nullptr)

                cout << ".";

            else if (grid[i][j]->getType()==ANT)

                cout << "o";

            else cout << "X";

        }

        cout << endl;

    }

}

void Game::oneTurn()

{

    int i,j;
    for (i=0; i < GRID_SIZE; i++)

        for (j=0; j < GRID_SIZE; j++)

        {

            if (grid[i][j]!= nullptr) grid[i][j]->moved = false;

        }

// move if it's a Doodlebug because DB move before ants

    for (i=0; i < GRID_SIZE; i++)

        for (j=0; j < GRID_SIZE; j++)

        {

            if ((grid[i][j]!= nullptr) && (grid[i][j]->getType()==DOODLEBUG))

            {

                if (!grid[i][j]->moved)

                {

                    grid[i][j]->moved = true; // Mark as moved

                    grid[i][j]->move();

                }

            }

        }

// move ants

    for (i=0; i < GRID_SIZE; i++)

        for (j=0; j < GRID_SIZE; j++)

        {

            if ((grid[i][j]!= nullptr) && (grid[i][j]->getType()==ANT))

            {

                if (!grid[i][j]->moved)

                {

                    grid[i][j]->moved = true;

                    grid[i][j]->move();

                }

            }

        }

    for (i=0; i < GRID_SIZE; i++)

        for (j=0; j < GRID_SIZE; j++)

        {


            if ((grid[i][j]!= nullptr) &&

                (grid[i][j]->getType()==DOODLEBUG))

            {

                if (grid[i][j]->starve())

                {

                    delete (grid[i][j]);

                    grid[i][j] = nullptr;

                }

            }

        }

    for (i=0; i < GRID_SIZE; i++)

        for (j=0; j < GRID_SIZE; j++)

        {

            if ((grid[i][j]!= nullptr) && grid[i][j]->moved)

            {

                grid[i][j]->breed();

            }

        }

}

Critters::Critters()

{

    game = nullptr;

    moved = false;

    breedCounter = 0;

    x=0;

    y=0;

}

Critters::Critters(Game *game, int x, int y)

{

    this->game = game;

    moved = false;

    breedCounter = 0;

    this->x=x;

    this->y=y;

    game->setLocation(x, y, this);

}



Critters::~Critters()

= default;


class Ant : public Critters

{

    friend class Game;

public:

    Ant();

    Ant(Game *game, int x, int y);

    void breed() override;

    void move() override;

    int getType() override;

    bool starve(){ return false; }

};


Ant::Ant() : Critters()

{
    //intentionally left empty
}

Ant::Ant(Game *game, int x, int y) : Critters(game, x, y)

{

}



void Ant::move()

{

    int cell = rand() % 4;
    // 4 because there are only 4 possible directions

    if (cell == 0)

    {

        if ((y>0) && (game->getLocation(x, y - 1) == nullptr))

        {

            game->setLocation(x, y - 1, game->getLocation(x, y));  // Move to new spot

            game->setLocation(x, y, nullptr);

            y--;

        }

    }

    else if (cell == 1)

    {

        if ((y < GRID_SIZE - 1) && (game->getLocation(x, y + 1) == nullptr))

        {

            game->setLocation(x, y + 1, game->getLocation(x, y));  // Move to new spot

            game->setLocation(x, y, nullptr);  // Set current spot to empty

            y++;

        }

    }

    else if (cell == 2)

    {

        if ((x>0) && (game->getLocation(x - 1, y) == nullptr))

        {

            game->setLocation(x - 1, y, game->getLocation(x, y));

            game->setLocation(x, y, nullptr);

            x--;

        }

    }

// Try to move right

    else

    {

        if ((x < GRID_SIZE - 1) && (game->getLocation(x + 1, y) == nullptr))

        {

            game->setLocation(x + 1, y, game->getLocation(x, y));

            game->setLocation(x, y, nullptr);

            x++;

        }

    }

}


int Ant::getType()

{

    return ANT;

}



void Ant::breed()

{

    breedCounter++;

    if (breedCounter == ANT_BREED)

    {

        breedCounter = 0;

        if ((y>0) && (game->getLocation(x, y - 1) == nullptr))

        {

            Ant *newAnt = new Ant(game, x, y - 1);

        }

        else if ((y < GRID_SIZE - 1) && (game->getLocation(x, y + 1) == nullptr))

        {

            Ant *newAnt = new Ant(game, x, y + 1);

        }

        else if ((x>0) && (game->getLocation(x - 1, y) == nullptr))

        {

            Ant *newAnt = new Ant(game, x - 1, y);

        }

        else if ((x < GRID_SIZE - 1) && (game->getLocation(x + 1, y) == nullptr))

        {

            Ant *newAnt = new Ant(game, x + 1, y);

        }

    }

}


class Doodlebug : public Critters

{
    friend class Game;

public:

    Doodlebug();

    Doodlebug(Game *game, int x, int y);

    void breed();

    void move();

    int getType();

    bool starve();

private:

    int starveCounter;

};


Doodlebug::Doodlebug() : Critters()

{

    starveCounter = 0;

}

Doodlebug::Doodlebug(Game *game, int x, int y) : Critters(game, x, y)

{

    starveCounter = 0;

}



void Doodlebug::move()

{


    if ((y>0) && (game->getLocation(x, y - 1) != nullptr) &&

        (game->getLocation(x, y - 1)->getType() == ANT))

    {

        delete (game->grid[x][y - 1]);  // DB eats the ant

        game->grid[x][y - 1] = this;

        game->setLocation(x, y, nullptr);

        starveCounter =0 ;

        y--;

        return;

    }

    else if ((y < GRID_SIZE - 1) && (game->getLocation(x, y + 1) != nullptr) &&

             (game->getLocation(x, y + 1)->getType() == ANT))

    {

        delete (game->grid[x][y + 1]);  // DB eats ant

        game->grid[x][y + 1] = this;

        game->setLocation(x, y, nullptr);

        starveCounter =0 ;

        y++;

        return;

    }

    else if ((x>0) && (game->getLocation(x - 1, y) != nullptr) &&

             (game->getLocation(x - 1, y)->getType() == ANT))

    {

        delete (game->grid[x - 1][y]);  // DB eats ant

        game->grid[x - 1][y] = this;

        game->setLocation(x, y, nullptr);

        starveCounter =0 ;

        x--;

        return;

    }

    else if ((x < GRID_SIZE - 1) && (game->getLocation(x + 1, y) != nullptr) &&

             (game->getLocation(x + 1, y)->getType() == ANT))

    {

        delete (game->grid[x + 1][y]);

        game->grid[x + 1][y] = this;

        game->setLocation(x, y, nullptr);

        starveCounter =0 ;

        x++;

        return;

    }

    int cell = rand() % 4;

// pick a random direction (out of 4) to locate food/ants

    if (cell == 0)

    {

        if ((y>0) && (game->getLocation(x, y - 1) == nullptr))

        {

            game->setLocation(x, y - 1, game->getLocation(x, y));

            game->setLocation(x, y, nullptr);

            y--;

        }

    }
//down
    else if (cell == 1)

    {

        if ((y < GRID_SIZE - 1) && (game->getLocation(x, y + 1) == nullptr))

        {

            game->setLocation(x, y + 1, game->getLocation(x, y));

            game->setLocation(x, y, nullptr);

            y++;

        }

    }

// left

    else if (cell == 2)

    {

        if ((x>0) && (game->getLocation(x - 1, y) == nullptr))

        {

            game->setLocation(x - 1, y, game->getLocation(x, y));  // Move to new spot

            game->setLocation(x, y, nullptr);  // Set current spot to empty

            x--;

        }

    }

// right

    else

    {

        if ((x < GRID_SIZE - 1) && (game->getLocation(x + 1, y) == nullptr))

        {

            game->setLocation(x + 1, y, game->getLocation(x, y));  // Move to new spot

            game->setLocation(x, y, nullptr);  // Set current spot to empty

            x++;

        }

    }

    starveCounter++;


}


int Doodlebug::getType()

{

    return DOODLEBUG;

}


void Doodlebug::breed()

{

    breedCounter++;

    if (breedCounter == DOODLEBUG_BREED)

    {

        breedCounter = 0;

// Try to make a new ant either above, left, right, or down

        if ((y>0) && (game->getLocation(x, y - 1) == nullptr))

        {

            auto *newDoodle = new Doodlebug(game, x, y - 1);

        }

        else if ((y < GRID_SIZE - 1) && (game->getLocation(x, y + 1) == nullptr))

        {

            auto *newDoodle = new Doodlebug(game, x, y + 1);

        }

        else if ((x>0) && (game->getLocation(x - 1, y) == nullptr))

        {

            auto *newDoodle = new Doodlebug(game, x - 1, y);

        }

        else if ((x < GRID_SIZE - 1) && (game->getLocation(x + 1, y) == nullptr))

        {

            auto *newDoodle = new Doodlebug(game, x + 1, y);

        }

    }

}


bool Doodlebug::starve()

{

    if (starveCounter > DOODLEBUG_STARVE_TIME)

    {

        return true;

    }

    else

    {

        return false;

    }

}

int main()

{

    string userInput;

    srand(time(nullptr));

    Game g;

    // Randomly create 100 ants

    int antCounter = 0;

    while (antCounter < INITIALIZED_ANTS)

    {

        int x;
        x = rand() % GRID_SIZE;

        int y;
        y = rand() % GRID_SIZE;

        if (g.getLocation(x, y) == nullptr)

        {

            antCounter++;

            auto *ant1 = new Ant(&g, x, y);

        }

    }

    int doodlebugCounter = 0;

   while (doodlebugCounter < INITIALIZED_DOODLEBUGS)

    {

        int x;
        x = rand() % GRID_SIZE;

        int y;
        y = rand() % GRID_SIZE;

       if (g.getLocation(x, y) == nullptr){

            doodlebugCounter++;

        }

    }
    cout<<endl<<"********  Welcome to Doodlebug VS Ant  ********"<<endl;

    while (true)

    {

        g.showGrid();

        g.oneTurn();

        cout << endl << "Press enter for next step" << endl;

        getline(cin, userInput);

    }

    return 0;

}
