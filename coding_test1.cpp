#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<algorithm>
#include<fstream>
#include<sstream>

using namespace std;

struct Player
{
	string id;
	string handlename;
	int score;
};

void getEntlyData(string& file_name, unordered_map<string, string>& ently_data);
void getScoreData(string& file_name, unordered_map<string, int> score_data, const unordered_map<string, string>& ently_data, vector<Player>& player);
void sortPlayerScore(vector<Player>& player);
void showRanking(vector<Player>& player);

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cerr << "入力引数の数が不正です。";
		exit(EXIT_FAILURE);
	}

	string ently_file_name = argv[1];
	string score_log_name = argv[2];

	unordered_map<string, string> ently_data;
	unordered_map<string, int> score_data;
	vector<Player> player;

	getEntlyData(ently_file_name, ently_data);

	getScoreData(score_log_name, score_data, ently_data, player);

	sortPlayerScore(player);

	showRanking(player);

	return 0;

}

void getEntlyData(string& file_name, unordered_map<string, string>& ently_data)
{
	ifstream ently_file;
	ently_file.open(file_name);
	if (!ently_file)
	{
		cerr << "ファイル名:" + file_name + "　ファイルのオープンに失敗しました。";
		exit(EXIT_FAILURE);
	}

	//ヘッダーを読み飛ばす
	string line;
	getline(ently_file, line);

	while (getline(ently_file, line))
	{
		istringstream iss(line);
		string player_id;
		string player_name;
		string checkbuff;

		if (getline(iss, player_id, ',') && getline(iss, player_name, ','))
		{
			if (getline(iss, checkbuff, ','))
			{
				cerr << "ファイル名:" + file_name + "　ファイルの仕様が異なります。";
				exit(EXIT_FAILURE);
			}

			ently_data[player_id] = player_name;
		}
		else
		{
			cerr << "ファイル名:" + file_name + "　ファイルの仕様が異なります。";
			exit(EXIT_FAILURE);
		}
	}

	ently_file.close();
}

void getScoreData(string& file_name, unordered_map<string, int> score_data, const unordered_map<string, string>& ently_data, vector<Player>& player)
{
	ifstream score_log_file(file_name);
	if (!score_log_file)
	{
		cerr << "ファイル名:" + file_name + "　ファイルのオープンに失敗しました。";
		exit(EXIT_FAILURE);
	}

	//ヘッダーを読み飛ばす
	string line;
	getline(score_log_file, line);
	while (getline(score_log_file, line))
	{
		istringstream iss(line);
		string playerid, timestamp, checkbuff;
		int score;

		if (getline(iss, timestamp, ',') && getline(iss, playerid, ',') && iss >> score)
		{
			if (getline(iss, checkbuff, ','))
			{
				cerr << "ファイル名:" + file_name + "　ファイルの仕様が異なります。";
				exit(EXIT_FAILURE);
			}

			if (ently_data.find(playerid) != ently_data.end())
			{
				score_data[playerid] = max(score_data[playerid], score);
			}
		}
		else
		{
			cerr << "ファイル名:" + file_name + "　ファイルの仕様が異なります。";
			exit(EXIT_FAILURE);
		}
	}

	for (const auto& ently : score_data)
	{
		player.push_back({ ently.first,ently_data.at(ently.first),ently.second });
	}

	score_log_file.close();
}

void sortPlayerScore(vector<Player>& player)
{
	sort(player.begin(), player.end(), [](const Player& a, const Player& b) {
		if (a.score != b.score)
		{
			return a.score > b.score;
		}
		else {
			return a.id < b.id;
		}
		});
}

void showRanking(vector<Player>& player)
{
	int prevscore = 0;
	int rank = 1;
	int count = 1;

	cout << "rank,player_id,handle_name,score";

	for (const auto& ently_player : player)
	{
		if (rank <= 10)
		{
			cout << endl;
			cout << rank << ',' << ently_player.id << ',' << ently_player.handlename << ',' << ently_player.score;

			if (prevscore != ently_player.score)
			{
				rank++;
			}
			prevscore = ently_player.score;
			count++;
		}
		else
		{
			break;
		}

	}
}