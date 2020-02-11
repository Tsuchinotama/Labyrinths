#include <iostream>
#include <array>
#include <vector>
#include <list>
#include <utility>

using namespace std;

typedef array<array<bool, 8>, 8> Board;

typedef pair<int, int> Position;

typedef array<array<Position, 8>, 8> Board_pos;

struct Maze
{
   Board Squares;
   Board Exit;
   Board_pos Predecessor;
   Board Way;
};

constexpr Position False_pos({-1, -1});

typedef vector<Position> Voisins;

Voisins Voisins_pos(Maze maz, Position pos);

bool ExitSearch(Maze& maz, Position start_pos, Position& exit_pos);

Board Empty_board();

Board_pos Empty_pred();

void Show_maze(Maze maz, Position start_pos);

int main()
{
    Board Exit;
    Exit=Empty_board();
    Board Squares=
    {{
        {true, true, true, true, true, true, true, true},
        {true, false, false, false, true, false, false, true},
        {true, true, true, false, true, false, false, true},
        {false, false, true, false, true, true, false, true},
        {true, false, false, false, false, false, true, true},
        {true, false, true, true, true, false, false, true},
        {true, false, true, false, false, false, false, true},
        {true, true, true, true, true, true, true, true}
    }};
    Exit[3][0]=true;
    Position start_pos({5,1});
    Board_pos pred=Empty_pred();
    pred[4][1]=start_pos;
    Board Way(Empty_board());
    Maze maz;
    maz.Squares=Squares;
    maz.Exit=Exit;
    maz.Predecessor=pred;
    maz.Way=Way;
    Position exit_pos;
    Show_maze(maz, start_pos);
    Maze copy_maz(maz);
    if (ExitSearch(maz, start_pos, exit_pos))
    {
        copy_maz.Predecessor=maz.Predecessor;
        for (int i=0; i<8; i++)
        {
            for (int j=0; j<8; j++)
            {
                cout << " (" << maz.Predecessor[i][j].first << "," << maz.Predecessor[i][j].second << ") ";
            }
            cout << endl;
        }
        cout << endl;
        cout << "Exiting from the maze starting from (" << start_pos.first << "," << start_pos.second << ") is possible." << endl << endl;
        while (exit_pos!=start_pos)
        {
            cout << "(" << exit_pos.first << "," << exit_pos.second << ")" << endl;
            copy_maz.Way[exit_pos.first][exit_pos.second]=true;
            exit_pos=maz.Predecessor[exit_pos.first][exit_pos.second];
        }
        cout << "(" << start_pos.first << "," << start_pos.second << ")" << endl << endl;
        Show_maze(copy_maz, start_pos);
    }
    else
    {
        cout << "Exiting from the maze starting from (" << start_pos.first << "," << start_pos.second << ") is impossible" << endl;
    }

    return 0;
}

Board Empty_board()
{
    Board tab;
    for (int i=0; i<=7; i++)
    {
        for (int j=0; j<=7; j++)
        {
            tab[i][j]=false;
        }
    }
    return tab;
}

Board_pos Empty_pred()
{
    Board_pos pred;
    for (int i=0; i<=7; i++)
    {
        for (int j=0; j<=7; j++)
        {
            pred[i][j]=False_pos;
        }
    }
    return pred;
}

Voisins Voisins_pos(Position pos)
{
    Voisins voisins;
    voisins.clear();
    if (pos.first==0 && pos.second==0)
    {
        voisins.push_back({pos.first+1, pos.second});
        voisins.push_back({pos.first, pos.second+1});
    }
    else if (pos.first==0 && pos.second==7)
    {
        voisins.push_back({pos.first+1, pos.second});
        voisins.push_back({pos.first, pos.second-1});
    }
    else if (pos.first==7 && pos.second==0)
    {
        voisins.push_back({pos.first-1, pos.second});
        voisins.push_back({pos.first, pos.second+1});
    }
    else if (pos.first==7 && pos.second==7)
    {
        voisins.push_back({pos.first-1, pos.second});
        voisins.push_back({pos.first, pos.second-1});
    }
    else if (pos.first==0)
    {
        voisins.push_back({pos.first+1, pos.second});
        voisins.push_back({pos.first, pos.second-1});
        voisins.push_back({pos.first, pos.second+1});
    }
    else if (pos.first==7)
    {
        voisins.push_back({pos.first-1, pos.second});
        voisins.push_back({pos.first, pos.second-1});
        voisins.push_back({pos.first, pos.second+1});
    }
    else if (pos.second==0)
    {
        voisins.push_back({pos.first-1, pos.second});
        voisins.push_back({pos.first+1, pos.second});
        voisins.push_back({pos.first, pos.second+1});
    }
    else if (pos.second==7)
    {
        voisins.push_back({pos.first-1, pos.second});
        voisins.push_back({pos.first+1, pos.second});
        voisins.push_back({pos.first, pos.second-1});
    }
    else
    {
        voisins.push_back({pos.first-1, pos.second});
        voisins.push_back({pos.first+1, pos.second});
        voisins.push_back({pos.first, pos.second-1});
        voisins.push_back({pos.first, pos.second+1});
    }
    return voisins;
}

bool ExitSearch(Maze& maz, Position pos, Position& exit_pos)
{
    if (maz.Squares[pos.first][pos.second] || pos.first<0 || pos.first>7 || pos.second<0 || pos.second>7)
    {
        return false;
    }
    else
    {
        maz.Squares[pos.first][pos.second]=true;
        if (maz.Exit[pos.first][pos.second])
        {
            exit_pos=pos;
            return true;
        }
        else
        {
            Voisins voisins=Voisins_pos(pos);
            bool coince;
            coince=maz.Squares[voisins[0].first][voisins[0].second];
            if (!coince && maz.Predecessor[voisins[0].first][voisins[0].second]==False_pos)
                {
                    maz.Predecessor[voisins[0].first][voisins[0].second]=pos;
                }
            for (int i=1; i<voisins.size(); i++)
            {
                coince=coince && maz.Squares[voisins[i].first][voisins[i].second];
                if (!maz.Squares[voisins[i].first][voisins[i].second] && maz.Predecessor[voisins[i].first][voisins[i].second]==False_pos)
                {
                    maz.Predecessor[voisins[i].first][voisins[i].second]=pos;
                }
            }
            if (coince)
            {
                return false;
            }
            else
            {
                Position Up({pos.first-1, pos.second});
                Position Down({pos.first+1, pos.second});
                Position Left({pos.first, pos.second-1});
                Position Right({pos.first, pos.second+1});
                return (ExitSearch(maz, Up, exit_pos) || ExitSearch(maz, Down, exit_pos) || ExitSearch(maz, Left, exit_pos) || ExitSearch(maz, Right, exit_pos));
            }
        }
    }
}

void Show_maze(Maze maz, Position start_pos)
{
    for (int i=0; i<8; i++)
        {
            for (int j=0; j<8; j++)
            {
                Position pos({i, j});
                if (maz.Squares[i][j])
                {
                    cout << " # ";
                }
                else if (maz.Exit[i][j])
                {
                    cout << " S ";
                }
                else if (start_pos==pos)
                {
                    cout << " * ";
                }
                else if (maz.Way[i][j])
                {
                    cout << " x ";
                }
                else
                {
                    cout << "   ";
                }
            }
            cout << endl;
        }
        cout << endl;
}

