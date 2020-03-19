#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "tree.h"
#include <vector>
using namespace std;
struct pointpos
{
	int x = 0;
	int y = 0;
};
int main() {
	string filename = "rectdb.txt";
	fstream inFile;

	inFile.open(filename.c_str());
	if (inFile.is_open())//if file can open
	{
		string line;
		getline(inFile, line);
		istringstream ss(line);
		int top_cor, left_cor, bottom_cor, right_cor;
		ss >> top_cor >> left_cor >> bottom_cor >> right_cor;//taking the boundries of the rectangles
		Rectangle real_extent(top_cor, left_cor, bottom_cor, right_cor);//creating boundries
		TwoDimTree newtree(real_extent);
		while (!inFile.eof()) {
			getline(inFile, line);
			istringstream sss(line);
			int top_cor = 0, left_cor = 0, bottom_cor = 0, right_cor = 0;
			sss >> top_cor;//inserting top
			if (top_cor < 0)//if the top value smaller than finish inserting
				break;
			sss >> left_cor >> bottom_cor >> right_cor;//inserting left, bottom and right
			Rectangle current_rect(top_cor, left_cor, bottom_cor, right_cor);
			newtree.treeinserter(current_rect);//inserting rectangle to the tree
		}
		vector<int> countvector;//count vector
		vector<vector<Rectangle>> rectvector;//rectangle vectors
		vector<pointpos> posvector;
		while (true) {//taking the iputs
			vector <Rectangle> current;
			int x = 0, y = 0, count = 0;
			cin >> x >> y;;
			if (x == -1)//if x value is -1 stop taking inputs
				break;
			pointpos newpos;
			newpos.x = x;
			newpos.y = y;
			posvector.push_back(newpos);
			newtree.find(x, y, count, current);
			countvector.push_back(count);
			rectvector.push_back(current);
		}

		for (int i = 0; i < countvector.size(); i++) {// printing all values 
			cout << posvector[i].x << " " << posvector[i].y << endl;
			cout << countvector[i] << endl;
			for (int j = 0; j < rectvector[i].size(); j++)
			{
				cout << rectvector[i][j].getTop() << " " << rectvector[i][j].getLeft() << " " << rectvector[i][j].getBot() << " " << rectvector[i][j].getRight() << " " << endl;
			}
		}
	}
	return 0;
}