// rats.cpp

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
#include <cassert>

using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;            // max number of rows in the arena
const int MAXCOLS = 20;            // max number of columns in the arena
const int MAXRATS = 100;           // max number of rats allowed

const int NORTH = 0;
const int EAST  = 1;
const int SOUTH = 2;
const int WEST  = 3;
const int NUMDIRS = 4;

const int EMPTY      = 0;
const int HAS_POISON = 1;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
              // type name, since it's mentioned in the Rat declaration.

class Rat
{
  public:
      // Constructor
    Rat(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    void move();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
	int m_hp;
	bool toMove;
};

class Player
{
  public:
      // Constructor
    Player(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    string dropPoisonPellet();
    string move(int dir);
    void   setDead();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};

class Arena
{
  public:
      // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();

      // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     ratCount() const;
    int     getCellStatus(int r, int c) const;
    int     numberOfRatsAt(int r, int c) const;
    void    display(string msg) const;

      // Mutators
    void setCellStatus(int r, int c, int status);
    bool addRat(int r, int c);
    bool addPlayer(int r, int c);
    void moveRats();

  private:
    int     m_grid[MAXROWS][MAXCOLS];
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Rat*    m_rats[MAXRATS];
    int     m_nRats;
    int     m_turns;

      // Helper functions
    void checkPos(int r, int c) const;
    bool isPosInBounds(int r, int c) const;
};

class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nRats);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;

      // Helper functions
    string takePlayerTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int min, int max);
bool decodeDirection(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Rat implementation
///////////////////////////////////////////////////////////////////////////

Rat::Rat(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** A rat must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Rat created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
	m_hp = 2;
	toMove = false;
}

int Rat::row() const
{
    return m_row;
}

int Rat::col() const
{
    return m_col;
}

bool Rat::isDead() const
{
    return m_hp<1;
}

void Rat::move()
{
	if(m_hp==1)
	{
	  if(toMove)//alternates so rat only moves every two turns
		  toMove=false;
	  else
	  {
		  toMove=true;
		  return;
	  }
	}
	if(attemptMove(*m_arena,randInt(0,3),m_row,m_col))
	{
		if(m_arena->getCellStatus(m_row,m_col)==HAS_POISON)
		{
			m_hp--;
			m_arena->setCellStatus(m_row,m_col,EMPTY);
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

string Player::dropPoisonPellet()
{
    if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
        return "There's already a poison pellet at this spot.";
    m_arena->setCellStatus(m_row, m_col, HAS_POISON);
    return "A poison pellet has been dropped.";
}

string Player::move(int dir)
{
	string moved = "";
	if(attemptMove(*m_arena,dir,m_row,m_col))
	{
		switch(dir)
		{
			case NORTH:
				moved="north";
				break;
			case EAST:
				moved="east";
				break;
			case SOUTH:
				moved="south";
				break;
			case WEST:
				moved="west";
				break;
		}
		if(m_arena->numberOfRatsAt(m_row,m_col)>0)
		{
			setDead();
			return "Player walked into a rat and died.";
		}
	}
	else
		return "Player couldn't move; player stands.";
	return "Player moved "+moved+".";
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
             << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nRats = 0;
    m_turns = 0;
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            setCellStatus(r, c, EMPTY);
	for (int i=0; i<MAXRATS; i++)
		m_rats[i]=nullptr;
}

Arena::~Arena()
{
    delete m_player;
	for(int i=0;i<MAXRATS;i++)
		delete m_rats[i];
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::ratCount() const
{
    return m_nRats;
}

int Arena::getCellStatus(int r, int c) const
{
    checkPos(r, c);
    return m_grid[r-1][c-1];
}

int Arena::numberOfRatsAt(int r, int c) const
{
    int rats = 0;
	for(int i=0;i<MAXRATS;i++)
		if(m_rats[i]!=nullptr && !m_rats[i]->isDead() && m_rats[i]->row()==r && m_rats[i]->col()==c)
				rats++;
    return rats;
}

void Arena::display(string msg) const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    
      // Fill displayGrid with dots (empty) and stars (poison pellets)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r-1][c-1] = (getCellStatus(r,c) == EMPTY ? '.' : '*');
	
	for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
			switch(numberOfRatsAt(r,c))
			{
				case 0:
				break;
				case 1:
					displayGrid[r-1][c-1] = 'R';
				break;
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
					displayGrid[r-1][c-1] = '0'+numberOfRatsAt(r,c);
				break;
				default:
					displayGrid[r-1][c-1]='9';
			}

      // Indicate player's position
    if (m_player != nullptr)
        displayGrid[m_player->row()-1][m_player->col()-1] = (m_player->isDead() ? 'X' : '@');

      // Draw the grid
    clearScreen();
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
            cout << displayGrid[r-1][c-1];
        cout << endl;
    }
    cout << endl;

      // Write message, rat, and player info
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << ratCount() << " rats remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player!" << endl;
    else if (m_player->isDead())
        cout << "The player is dead." << endl;
    cout << m_turns << " turns have been taken." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
    checkPos(r, c);
    m_grid[r-1][c-1] = status;
}

bool Arena::addRat(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;

      // Don't add a rat on a spot with a poison pellet
    if (getCellStatus(r, c) != EMPTY)
        return false;

      // Don't add a rat on a spot with a player
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
        return false;
	
	Rat* ratToAdd = new Rat(this, r, c);
	
	for(int i = 0; i<MAXRATS;i++)
		if(m_rats[i]==nullptr)
		{
			m_rats[i]=ratToAdd;
			m_nRats++;
			return true;
		}
    return false;//returns false if there are MAXRATS because every pointer will not be empty
}

bool Arena::addPlayer(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;

      // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

      // Don't add a player on a spot with a rat
    if (numberOfRatsAt(r, c) > 0)
        return false;

    m_player = new Player(this, r, c);
    return true;
}

void Arena::moveRats()
{
	for(int i=0;i<MAXRATS;i++)
		if(m_rats[i]!=nullptr)
		{
			m_rats[i]->move();//move every rat
			if(m_rats[i]->isDead())//if the rat is dead, delete it
			{
				delete m_rats[i];
				m_rats[i]=nullptr;
				m_nRats--;
			}
		}
	if(numberOfRatsAt(m_player->row(),m_player->col())>0)
		m_player->setDead();
      // Another turn has been taken
    m_turns++;
}

bool Arena::isPosInBounds(int r, int c) const
{
    return (r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols);
}

void Arena::checkPos(int r, int c) const
{
    if (r < 1  ||  r > m_rows  ||  c < 1  ||  c > m_cols)
    {
        cout << "***** " << "Invalid arena position (" << r << ","
             << c << ")" << endl;
		cout<<m_rows<<" "<<m_cols;
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nRats)
{
    if (nRats < 0)
    {
        cout << "***** Cannot create Game with negative number of rats!" << endl;
        exit(1);
    }
    if (nRats > MAXRATS)
    {
        cout << "***** Trying to create Game with " << nRats
             << " rats; only " << MAXRATS << " are allowed!" << endl;
        exit(1);
    }
    if (rows == 1  &&  cols == 1  &&  nRats > 0)
    {
        cout << "***** Cannot create Game with nowhere to place the rats!" << endl;
        exit(1);
    }

      // Create arena
    m_arena = new Arena(rows, cols);
      // Add player
    int rPlayer;
    int cPlayer;
    do
    {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
    m_arena->addPlayer(rPlayer, cPlayer);

      // Populate with rats
    while (nRats > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (r == rPlayer && c == cPlayer)
            continue;
        m_arena->addRat(r, c);
        nRats--;
    }
}

Game::~Game()
{
    delete m_arena;
}

string Game::takePlayerTurn()
{
    for (;;)
    {
        cout << "Your move (n/e/s/w/x or nothing): ";
        string playerMove;
        getline(cin, playerMove);

        Player* player = m_arena->player();
        int dir;

        if (playerMove.size() == 0)
        {
            if (recommendMove(*m_arena, player->row(), player->col(), dir))
                return player->move(dir);
            else
                return player->dropPoisonPellet();
        }
        else if (playerMove.size() == 1)
        {
            if (tolower(playerMove[0]) == 'x')
                return player->dropPoisonPellet();
            else if (decodeDirection(playerMove[0], dir))
                return player->move(dir);
        }
        cout << "Player move must be nothing, or 1 character n/e/s/w/x." << endl;
    }
}

void Game::play()
{
    m_arena->display("");
    while ( ! m_arena->player()->isDead()  &&  m_arena->ratCount() > 0)
    {
        string msg = takePlayerTurn();
        Player* player = m_arena->player();
        if (player->isDead())
        {
            cout << msg << endl;
            break;
        }
        m_arena->moveRats();
        m_arena->display(msg);
    }
    if (m_arena->player()->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

  // Return a uniformly distributed random int from min to max, inclusive
int randInt(int min, int max)
{
    if (max < min)
        swap(max, min);
    static random_device rd;
    static mt19937 generator(rd());
    uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

bool decodeDirection(char ch, int& dir)
{
    switch (tolower(ch))
    {
      default:  return false;
      case 'n': dir = NORTH; break;
      case 'e': dir = EAST;  break;
      case 's': dir = SOUTH; break;
      case 'w': dir = WEST;  break;
    }
    return true;
}

  // Return false without changing anything if moving one step from (r,c)
  // in the indicated direction would run off the edge of the arena.
  // Otherwise, update r and c to the position resulting from the move and
  // return true.
bool attemptMove(const Arena& a, int dir, int& r, int& c)
{	
	int dx = 0;
	int dy = 0;
	switch(dir)
	{
		case NORTH:
			dy--;
			break;
		case EAST:
			dx++;
			break;
		case SOUTH:
			dy++;
			break;
		case WEST:
			dx--;
			break;
	}
	if(r+dy >= 1  &&  r+dy <= a.rows()  &&  c+dx >= 1  &&  c+dx <= a.cols())
	{
		c+=dx;
		r+=dy;
		return true;
	}
    return false;
}

  // Recommend a move for a player at (r,c):  A false return means the
  // recommendation is that the player should drop a poison pellet and not
  // move; otherwise, this function sets bestDir to the recommended
  // direction to move and returns true.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
	struct MoveStats//struct to store information at a certain location
	{
		MoveStats(const Arena& arena, int row, int col, int d=-1)
		{
			dir=d;
			int dx = 0;
			int dy = 0;
			switch(d)
			{
				case NORTH:
					dy--;
					break;
				case EAST:
					dx++;
					break;
				case SOUTH:
					dy++;
					break;
				case WEST:
					dx--;
					break;
				default:
					dir=-1;
			}
			ratsAround = arena.numberOfRatsAt(row+dy-1,col+dx)+arena.numberOfRatsAt(row+dy+1,col+dx)
				+arena.numberOfRatsAt(row+dy,col+dx-1)+arena.numberOfRatsAt(row+dy,col+dx+1);
			ratsAt = arena.numberOfRatsAt(row+dy,col+dx);
			valid = attemptMove(arena,d,row,col);
		}
		int ratsAround;
		int ratsAt;
		bool valid;
		int dir;
	};
	
	MoveStats moveArray[5]={//move array stores the number of rats at the square to move to, number of rats around the square to move to, and the validity of moving in a given direction
		MoveStats(a,r,c,NORTH),
		MoveStats(a,r,c,EAST),
		MoveStats(a,r,c,SOUTH),
		MoveStats(a,r,c,WEST),
		MoveStats(a,r,c)};
    if(moveArray[4].ratsAround>0)//if the current location has no rats around it, stay put
	{
		bool directionSet=false;
		for(int i=0;i<4;i++)//does not check the current location
		{
			bool leastDanger=true;
			for(int j=0;j<5;j++)//checks if current location has less danger, then no move will happen
				if(moveArray[j].valid && moveArray[j].ratsAt==0 && moveArray[j].ratsAround<moveArray[i].ratsAround)
					leastDanger=false;
			if(moveArray[i].valid && moveArray[i].ratsAt==0 && leastDanger)//picks a valid, empty direction with lowest rats around the end location
			{
				bestDir=moveArray[i].dir;
				directionSet=true;
			}
		}
		return directionSet;//if no valid empty directions 
	}		
    return false;
}

///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

int main()
{
      // Create a game
      // Use this instead to create a mini-game: 
	  //Game g(3, 5, 2);
    Game g(10, 12, 40);

      // Play the game
    g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++ UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _MSC_VER  //  Microsoft Visual C++

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                                                        &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif