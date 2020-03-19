#ifndef _TREE_H
#define _TREE_H
#include <vector>
#include <iostream>
using namespace std;

class Rectangle {// rectangle class
	friend class TwoDimTreeNode;//this class use the functions of the rectangle class 
	friend class TwoDimTree;//this class use the functions of the rectangle class
	friend bool y_intersect2(Rectangle& lhs, Rectangle& rhs);
	friend bool x_intersect2(Rectangle& lhs, Rectangle& rhs);
public:
	Rectangle(int top, int left, int bottom, int right) { //constructor for rectangle object
		Top = top;
		Left = left;
		Bottom = bottom;
		Right = right;
	}
	Rectangle() { //default constructor for the rectangle object
		Top = 0;
		Left = 0;
		Bottom = 0;
		Right = 0;
	}
	//int center_x_coord = (Left + Right) / 2     ,     center_y_coord = (Top + Bottom) / 2;
	int center_x_coord() { return(Left + Right) / 2; }//horizontal center of the rectangle
	int center_y_coord() { return (Top + Bottom) / 2; }//vertical center of the rectangle object
	Rectangle topleft_rect() { return Rectangle(Top, Left, (Top + Bottom) / 2, (Left + Right) / 2); } //top left rectangle of the current rectangle
	Rectangle topright_rect() { return Rectangle(Top, ((Left + Right) / 2) + 1, (Top + Bottom) / 2, Right); } //top right rectangle of the current rectangle
	Rectangle bottomleft_rect() { return Rectangle(((Top + Bottom) / 2) + 1, Left, Bottom, (Left + Right) / 2); } //bottom left rectangle of the current rectangle
	Rectangle bottomright_rect() { return Rectangle(((Top + Bottom) / 2) + 1, ((Left + Right) / 2) + 1, Bottom, Right); } //bottom right rectangle of the current rectangle
	int getTop() { return Top; }
	int	getLeft() { return Left; }
	int getBot() { return Bottom; }
	int getRight() { return Right; }
private:
	int Top; // y coordinate of the upper edge
	int Left; // x coordinate of the left edge
	int Bottom; // y coordinate of the bottom edge
	int Right; // x coordinate of the right edge
};

bool y_intersect2(Rectangle& lhs, Rectangle& rhs) {//if rhs rectangle object intersect the lhs vertical center the function return true
	if (rhs.Left <= lhs.center_x_coord() && rhs.Right >= lhs.center_x_coord())
		return true;
	else
		return false;
}
bool x_intersect2(Rectangle& lhs, Rectangle& rhs) {//if rhs rectangle object intersect the lhs horizontal center the function return true
	if (rhs.Top <= (lhs).center_y_coord() && rhs.Bottom >= (lhs).center_y_coord())
		return true;
	else
		return false;
}

class TwoDimTreeNode {
	friend class TwoDimTree;
public:
	TwoDimTreeNode(Rectangle& rect) :Extent(rect), TopRight(NULL), TopLeft(NULL), BottomLeft(NULL), BottomRight(NULL) {}//defult constructor of node object
	void insert(Rectangle& Rectt) {//recursive node inserting
		if (y_intersect2(Extent, Rectt)) {//if there is an intersect on vertical, current object will push back the vertical vector 
			(*this).Vertical.push_back(Rectt);
		}
		else if (x_intersect2(Extent, Rectt)) {//if there is an intersect on horizontal, current object will push back the horizontal vector 
			(*this).Horizontal.push_back(Rectt);
		}
		else if (Rectt.center_x_coord() > Extent.center_x_coord() && Rectt.center_y_coord() > Extent.center_y_coord()) { //creating new bot right node
			if ((*this).BottomRight == NULL) {
				BottomRight = new TwoDimTreeNode(Extent);
				BottomRight->Extent = Extent.bottomright_rect();//recalling the this function
			}
			BottomRight->insert(Rectt);
		}
		else if (Rectt.center_x_coord() < Extent.center_x_coord() && Rectt.center_y_coord() > Extent.center_y_coord()) { //creating new bot left node //updatedd line!!!!!!!!!!
			if ((*this).BottomLeft == NULL) {
				BottomLeft = new TwoDimTreeNode(Extent);
				BottomLeft->Extent = Extent.bottomleft_rect();
			}
			BottomLeft->insert(Rectt);
		}
		else if (Rectt.center_x_coord() < Extent.center_x_coord() && Rectt.center_y_coord() < Extent.center_y_coord()) {//creating new bot left node
			if ((*this).TopLeft == NULL) {
				TopLeft = new TwoDimTreeNode(Extent);
				TopLeft->Extent = Extent.topleft_rect();
			}
			TopLeft->insert(Rectt);
		}
		else if (Rectt.center_x_coord() > Extent.center_x_coord() && Rectt.center_y_coord() < Extent.center_y_coord()) {//creating new top right node
			if ((*this).TopRight == NULL) {
				TopRight = new TwoDimTreeNode(Extent);
				TopRight->Extent = Extent.topright_rect();
			}
			TopRight->insert(Rectt);
		}
	}
private:
	Rectangle Extent;
	vector<Rectangle> Vertical, Horizontal;//two vector will keep the intersects rectangles
	TwoDimTreeNode* TopLeft, *TopRight, *BottomLeft, *BottomRight; //4 arms of tree node
};


class TwoDimTree {
public:
	TwoDimTree(Rectangle& Rect) : firstroot(new TwoDimTreeNode(Rect)) {}//constructor
																		//	int number_of_find = 0; //	vector<Rectangle> found_rect;
	void treeinserter(Rectangle& rect) {//inserting first node to the the insert function's tree
		firstroot->insert(rect);
	}
	void find(int x, int y, int& count, vector<Rectangle>& list_of_rect) {//finding the rectangles that have the points that taken by the users
		TwoDimTreeNode* temp = firstroot;//temporary node pointer
		while (true) {
			if (!temp->Horizontal.empty()) {//checking the horizatnel vector whether it is emty or not, if not searching the rectangles
				for (int i = 0; i < temp->Horizontal.size(); i++) {
					if (temp->Horizontal[i].Top <= y && y < temp->Horizontal[i].Bottom && temp->Horizontal[i].Left <= x && x < temp->Horizontal[i].Right) {
						count++;
						list_of_rect.push_back(temp->Horizontal[i]);
					}
				}
			}
			if (!temp->Vertical.empty()) {//checking the vertical vector whether it is emty or not, if not searching the rectangles
				for (int i = 0; i < temp->Vertical.size(); i++) {
					if (temp->Vertical[i].Top <= y && y < temp->Vertical[i].Bottom && temp->Vertical[i].Left <= x && x < temp->Vertical[i].Right) {
						count++;
						list_of_rect.push_back(temp->Vertical[i]);
					}
				}
			}
			if (temp->TopLeft == NULL && temp->TopRight == NULL && temp->BottomLeft == NULL && temp->BottomRight == NULL)//if the node is a leaf break the loop
				break;
			if (temp->TopLeft != NULL && temp->TopLeft->Extent.Top <= y && y < temp->TopLeft->Extent.Bottom && temp->TopLeft->Extent.Left <= x && x < temp->TopLeft->Extent.Right) { //if the points inside the topleft rectangles of current extent
				temp = temp->TopLeft;
			}
			else if (temp->TopRight != NULL && temp->TopRight->Extent.Top <= y && y < temp->TopRight->Extent.Bottom && temp->TopRight->Extent.Left <= x && x < temp->TopRight->Extent.Right) {//if the points inside the topright rectangles of current extent
				temp = temp->TopRight;
			}
			else if (temp->BottomLeft != NULL && temp->BottomLeft->Extent.Top <= y && y < temp->BottomLeft->Extent.Bottom && temp->BottomLeft->Extent.Left <= x && x < temp->BottomLeft->Extent.Right) {//if the points inside the bootomleft rectangles of current extent
				temp = temp->BottomLeft;
			}
			else if (temp->BottomRight != NULL && temp->BottomRight->Extent.Top <= y && y < temp->BottomRight->Extent.Bottom && temp->BottomRight->Extent.Left <= x && x < temp->BottomRight->Extent.Right) {//if the points inside the bottomright rectangles of current extent
				temp = temp->BottomRight;
			}
			else {
				break;//if the point is not inside the child of the extent
			}
		}
	}
private:
	TwoDimTreeNode* firstroot;//the root of the tree
};

#endif