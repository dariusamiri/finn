#include <iostream>
using namespace std;

#define num 10000
struct Player {
    string name;
    double price;
    double speed;
    double finishing;
    double defence;
    int team_blonging = -1;
}player[num];

struct Team {
    string name;
    double money;
    int wins = 0;
    int looses = 0;
    int draws = 0;
    int num_player = 0;
    int players[1000];
}team[num];

void remove_and_shift_array(int players[], int i, int size_array)
{
    for (int j = i; j < size_array-1; j++) {
        players[j] = players[j+1];
    }
    players[size_array-1] = NULL;
}
void swap(int array[], int i, int j)
{
    int temp = *(array+i);
    *(array+i) = *(array+j);
    *(array+j) = temp;
}
void swap_str(string array[], int i, int j)
{
    string temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}
void bubblesort(int wins[], int size_team, int looses[], string name[] )
{
    for (int i = 0; i < size_team; i++) {
        for (int j = i+1; j < size_team; j++) {
            if (*(wins+i) < *(wins+j)) {
                swap(wins, i, j);
                swap(looses, i, j);
                swap_str(name, i, j);
            }
            else if (*(wins+i) == *(wins+j)) {
                if (*(looses+i) > *(looses+j)){
                swap(wins, i, j);
                swap(looses, i, j);
                swap_str(name, i, j);
                }
            }
        }
    }
}
int main() {
    int playerID = 0;
    int teamID = 0;
    string key;
    while (1) {
        cin >> key;
        if (key.compare("new") == 0) {
            string kind;
            cin >> kind;
            if (kind.compare("player") == 0) {
                cin >> player[playerID].name >> player[playerID].price >> player[playerID].speed >> player[playerID].finishing >> player[playerID].defence;
                    playerID++;
            }
            if (kind.compare("team") == 0) {
                cin >> team[teamID].name >> team[teamID].money;
                teamID++;
            }
        }
        
    if (key.compare("buy") == 0) {
        int player_ID, team_ID;
        cin >> player_ID >> team_ID;
        if (player_ID > playerID){
            cout << "player with the id " << player_ID << " doesnt exist" << endl;
        }
        else if (team_ID > (teamID)){
            cout << "team with the id " << team_ID << " doesnt exist" << endl;
        }
        else if (team[team_ID-1].money < player[player_ID-1].price) {
                   cout << "the team cant afford to buy this player" << endl;
               }
        else if ( player[player_ID-1].team_blonging != -1 ){
            cout << "player already has a team" << endl;
        }
        else{
            team[team_ID-1].money -= player[player_ID-1].price;
            player[player_ID-1].team_blonging++;
            team[team_ID-1].players[team[team_ID-1].num_player] = player_ID;
            team[team_ID-1].num_player++;
            cout << "player added to the team succesfully"<<endl;
        }
        
    }
        
    if (key.compare("sell") == 0) {
        int player_ID, team_ID;
        cin >> player_ID >> team_ID;
        if (team_ID > teamID) {
            cout << "team doesnt exist" << endl;
        }
        else {
            int count = 0;
           for (int i = 0; i < teamID; i++) {
                      if( team[team_ID-1].players[i] == player_ID){
                      cout << "player sold succesfully" << endl;
                          remove_and_shift_array(team[team_ID-1].players, i, team[team_ID-1].num_player);
                          player[player_ID-1].team_blonging = -1;
                          team[team_ID-1].money += player[player_ID-1].price;
                          team[team_ID-1].num_player--;
                          count++;
                          break;
                      }
            }
            if (count == 0) {
                cout << "team doesnt have this player" << endl;
            }
        }
    }
      
    if (key.compare("match") == 0) {
        int team_ID1, team_ID2;
        cin >> team_ID1 >> team_ID2;
        if ((team_ID1 > teamID ) || (team_ID2 > teamID ) ) {
            cout << "team doesnt exist" << endl;
        }
        else if( (team[team_ID1-1].num_player < 11) || (team[team_ID2-1].num_player < 11) ){
            cout << "the game can not be held due to loss of the players" << endl;
        }
        else{
        double power_team_ID1 = 0;
        double power_team_ID2 = 0;
        for (int i = 0; i < 11; i++) {
            power_team_ID1 += player[team[team_ID1-1].players[i]].finishing + player[team[team_ID1-1].players[i]].speed;
            power_team_ID2 += player[team[team_ID2-1].players[i]].defence + player[team[team_ID2-1].players[i]].speed;
        }
            if (power_team_ID1 > power_team_ID2) {
                team[team_ID1-1].wins++;
                team[team_ID2-1].looses++;
                team[team_ID1-1].money += 1000;
            }
            else if (power_team_ID2 > power_team_ID1) {
                team[team_ID2-1].wins++;
                team[team_ID1-1].looses++;
                team[team_ID2-1].money += 1000;
            }
            else if(power_team_ID1 == power_team_ID2) {
                team[team_ID1-1].draws++;
                team[team_ID2-1].draws++;
            }
    }
}
        
    if (key.compare("show") == 0) {
        string kind;
        cin >> kind;
        if (kind.compare("players") == 0) {
            for (int i = 0; i < playerID; i++) {
                cout << i+1 << " : " << player[i].name << endl;
            }
        }
        else if (kind.compare("teams") == 0){
            for (int i = 0; i < teamID; i++) {
                cout << i+1 << " : " << team[i].name << endl;
            }
        }
        
        else if(kind.compare("player") == 0){
            int player_ID;
            cin >> player_ID;
            if (player_ID > playerID) {
                cout << "player doesnt exist" << endl;
            }
            else{
                cout << "player : " << player[player_ID-1].name << endl;
                cout << "price : " << player[player_ID-1].price << endl;
                cout << "state :" << endl;
                cout << "speed : " << player[player_ID-1].speed << endl;
                cout << "finishing : " << player[player_ID-1].finishing << endl;
                cout << "defence : " << player[player_ID-1].defence << endl;
            }
        }
        else if(kind.compare("team") == 0){
            int team_ID;
            cin >> team_ID;
            if (team_ID > teamID) {
                cout << "team doesnt exist" << endl;
            }
            else{
                cout << "team : " << team[team_ID-1].name << endl;
                cout << "money : " << team[team_ID-1].money << endl;
                cout << "players : ";
                for (int i = 0; i < team[team_ID-1].num_player-1 ; i++) {
                    cout << team[team_ID-1].players[i] << " ";
                }
                cout << team[team_ID-1].players[team[team_ID-1].num_player-1] << endl;
                cout << "state :" << endl;
                cout << "win : " << team[team_ID-1].wins << endl;
                cout << "draw : " << team[team_ID-1].draws << endl;
                cout << "loose : " << team[team_ID-1].looses << endl;
            }
        }
        
    }
        
    if (key.compare("rank") == 0) {
        int team_wins[teamID];
        int team_looses[teamID];
        string team_name[teamID];
        for (int i = 0; i < teamID; i++) {
            team_wins[i] = team[i].wins;
            team_looses[i] = team[i].looses;
             team_name[i] = team[i].name;
        }
        bubblesort(team_wins, teamID, team_looses, team_name);
        for (int i = 0; i < teamID; i++) {
            cout << i+1 << ". " << team_name[i] <<endl;
        }
    }
    
    if (key.compare("end") == 0) {
        break;
    }
}
    return 0;
}
