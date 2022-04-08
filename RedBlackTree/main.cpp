/*
    Sakine Busra Gocmez
    150190064
    gocmez19@itu.edu.tr
    7.1.22
    Analysis of Algorithms / Assignment 3

    run at VS Code, Windows 10, by:
    g++ -std=c++11 -Wall -Werror main.cpp -o main
    ./main 
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>

using namespace std;

// Node class, tree's nodes.
class Node
{

private:
    // private attributes
    string publisher;
    int year;
    float NA_sales;
    float EU_sales;
    float other_sales;

public:
    // i misunderstand at first, and think that all sales sum will be asked.
    float sum;
    // children & parent
    Node *left;
    Node *right;
    Node *parent;
    // color -> 0: black, 1: red
    int color;
    // default constructor (parametrized)
    Node(string s, int i, float f1, float f2, float f3, float f4, Node *n1, Node *n2, Node *n3, int c)
    {
        this->publisher = s;
        this->year = i;
        this->NA_sales = f1;
        this->EU_sales = f2;
        this->other_sales = f3;
        this->sum = f4;
        this->left = n1;
        this->right = n2;
        this->parent = n3;
        this->color = c;
    }
    // getters
    string getpublisher() { return this->publisher; }
    int getYear() { return this->year; }
    float getNAs() { return this->NA_sales; }
    float getEUs() { return this->EU_sales; }
    float getOthers() { return this->other_sales; }
    // setters
    void setNAs(float toadd) { this->NA_sales = toadd; }
    void setEUs(float toadd) { this->EU_sales = toadd; }
    void setOthers(float toadd) { this->other_sales = toadd; }
    void setpublisher(string s) { this->publisher = s; }
    void setSum() { this->sum = this->EU_sales + this->NA_sales + this->other_sales; }
    // print function, i used at the beginning of my work, not used for assignment now
    void print()
    {
        cout << "publisher name: " << this->publisher << " sum: " << this->sum << "  color: " << this->color << endl;
    }
    // operator= overloading for assigning string line to the node object
    Node operator=(const vector<string> &vec)
    {
        this->publisher = vec[3];
        this->NA_sales = stof(vec[4]);
        this->EU_sales = stof(vec[5]);
        this->other_sales = stof(vec[6]);
        this->year = stoi(vec[2]);
        this->sum = this->NA_sales + this->EU_sales + this->other_sales;
        return *this;
    }
    // deconstructor
    ~Node() {}
};

// getting the dept for printing - (indentation)
// this is a recursive function to find depth
int getDepth(Node *node, string publisher, int depth)
{
    // if node is null, then the depth is 0. but in this assignment we dont need that
    if (node == NULL)
        return 0;

    // if node's publisher is equal to the parameter of the function, we will return the depth, exit the function.
    if (node->getpublisher() == publisher)
        return depth;

    // we are moving down, since we started from root at most beginning, then in recursive calls, node->left will be changed ofc.
    int down_depth = getDepth(node->left, publisher, depth + 1);

    // if the down (leaf) node is not 0, then we can return the down depth.
    if (down_depth != 0)
        return down_depth;

    // we are moving down with right subtree
    down_depth = getDepth(node->right, publisher, depth + 1);

    // return the down at the end as depth
    return down_depth;
}

// Red-Black-Tree class

class RBT
{
    //private attributes: root and empty Node class pointers
private:
    Node *root;
    Node *empty;

public:
    // default constructor
    RBT()
    {
        this->empty = new Node("", 0, 0, 0, 0, 0, nullptr, nullptr, nullptr, 0);
        // declaring the empty node, color is black (0) initially
        empty->color = 0;
        empty->left = nullptr;
        empty->right = nullptr;
        // assigning to the root
        this->root = empty;
    }
    // insert node
    vector<float> insertNode(Node *toadd);
    // fix the tree afther the insertion
    void fixing(Node *node);
    // rotation operations
    void rotateR(Node *node);
    void rotateL(Node *node);
    // getters
    Node *getroot() { return this->root; }
    // preorder traversal to print the tree at the end
    void preorder(Node *no)
    {
        // if the current node is empty (until it reaches, we will recursively call the preorder, and print)
        if (no != this->empty)
        {
            // get the depth of current node to print tires (-)
            int number_of_tires = getDepth(this->root, no->getpublisher(), 1) - 1;
            // convert integer-string for colors
            string color = (no->color == 0) ? "( BLACK )" : "( RED )";
            // identation printing with (-)
            for (int i = 0; i < number_of_tires; i++)
            {
                cout << "-";
            }
            //print color and publisher name
            cout << color << " " << no->getpublisher() << endl;

            // preorder traversal for left subtree first.
            preorder(no->left);
            // preorder traversal for right subtree then.
            preorder(no->right);
        }
    };
};

// rotation functions
// rotate to the right
void RBT::rotateR(Node *node)
{
    // take the left child and make it the right child.
    Node *iter = node->left;
    node->left = iter->right;

    // if right child is not empty, mean, we are not for root.
    // than make the parent the current node

    // rotation for both Rs: right, left-right
    if (iter->right != this->empty)
        iter->right->parent = node;

    iter->parent = node->parent;
    if (node->parent == nullptr)
        this->root = iter;
    else if (node == node->parent->right)
        node->parent->right = iter;
    else
        node->parent->left = iter;
    // we made the rotation
    iter->right = node;
    node->parent = iter;
}

// rotate to the left
void RBT::rotateL(Node *node)
{
    // take the right child and make it the left child.
    Node *iter = node->right;
    node->right = iter->left;

    // rotation for both Ls: left, right-left
    if (iter->left != this->empty)
        iter->left->parent = node;

    iter->parent = node->parent;
    if (node->parent == nullptr)
        this->root = iter;
    else if (node == node->parent->left)
        node->parent->left = iter;
    else
        node->parent->right = iter;
    // we made the rotation
    iter->left = node;
    node->parent = iter;
}

void recolor(Node *temp, Node *node)
{
    temp->color = 0;
    node->parent->color = 0;
    node->parent->parent->color = 1;
}

// fixing tree after the insertion
void RBT::fixing(Node *node)
{
    Node *temp;
    // our newly inserted node color is red, so we will reorganize the tree while we have 2 consecutive reds.
    // parent is red also?
    while (node->parent->color == 1)
    {
        // then, if parent is the right node of the grandparent
        if (node->parent == node->parent->parent->right)
        {
            // uncle will be left node of the grandparent
            temp = node->parent->parent->left;
            // if uncle is also red
            if (temp->color == 1)
            {
                // then recolor
                recolor(temp, node);
                // after the recoloring, node will be point to the grandparent
                node = node->parent->parent;
            }

            // if uncle is black, then rotation comes to the stage
            else
            {
                // if node is the left child
                if (node == node->parent->left)
                {
                    // point to the parent
                    node = node->parent;
                    // and rotate Right around the parent
                    this->rotateR(node);
                }
                // else, recolor parent as black and grandparent as red
                node->parent->color = 0;
                node->parent->parent->color = 1;
                // and rotate Left around the grandparent
                this->rotateL(node->parent->parent);
            }
        }
        // if parent is left node of the grandparent
        else
        {
            // then, uncle will be right node of the grandparent
            temp = node->parent->parent->right;

            // if uncle is red
            if (temp->color == 1)
            {
                // then recolor
                recolor(temp, node);
                // after the recoloring, node will be point to the grandparent
                node = node->parent->parent;
            }
            // if uncle is black, then rotation comes to the stage
            else
            {
                // if node is right child
                if (node == node->parent->right)
                {
                    // point to the parent
                    node = node->parent;
                    // and rotate left around the parent
                    this->rotateL(node);
                }
                // else, recolor parent as black and grandparent as red
                node->parent->color = 0;
                node->parent->parent->color = 1;
                // and rotate Right around the grandparent
                this->rotateR(node->parent->parent);
            }
        }
        // if node is root, break up the while loop
        if (node == this->root)
            break;
    }
    // recolor root as black, it will always be black
    this->root->color = 0;
}

// insert node to the Red-Black-Tree
vector<float> RBT::insertNode(Node *toadd)
{
    // stl container to store the sales for each region, function returns that so we use this vector to print the best seller
    vector<float> sales = {toadd->getEUs(), toadd->getNAs(), toadd->getOthers()};

    // reorganizing the node attributes, setting color red, childs point to the empty pointer of RBT
    toadd->color = 1;
    toadd->left = this->empty;
    toadd->right = this->empty;
    toadd->parent = nullptr;

    //cout << root == nullptr << endl;
    // if the RBT is empty, insert new node. it is black
    /*if (this->root == TNULL)
    {
        cout << "iam there " << endl;
        this->root = toadd;
        this->root->color = 0; //black
        return;
    }*/

    // if not empty, loop over

    // after the insertion, tree fixing will be done only for the newly named nodes. not will be fixed after the update
    bool needtofix = true;

    // node pointers for looping over
    Node *temp = nullptr;
    Node *iter = this->root;

    while (iter != this->empty)
    {
        // if the root is not same publisher with new node, i will add new node as newly node.
        //cout << " i am here, publisher: " << toadd->getpublisher() << endl;
        temp = iter;
        //cout << (toadd->getpublisher() < iter->getpublisher()) << " for toadd publisher : " << toadd->getpublisher() << " < iter publisher: " << iter->getpublisher();

        // current node: iter
        // basic cpp comparision operator, if newly publisher is aphabetically after the current node, update current node as left of itself
        if (toadd->getpublisher() < iter->getpublisher())
            iter = iter->left;
        // if newly publisher is just same as the current node, then update
        else if (toadd->getpublisher() == iter->getpublisher())
        {
            // update sales
            iter->setEUs(iter->getEUs() + toadd->getEUs());
            iter->setNAs(iter->getNAs() + toadd->getNAs());
            iter->setOthers(iter->getOthers() + toadd->getOthers());
            iter->setSum();
            // assign updated sales to the sale vector container for return
            sales[0] = iter->getEUs();
            sales[1] = iter->getNAs();
            sales[2] = iter->getOthers();
            // no need to fix tree, just updated, no new nodes
            needtofix = false;
            // return sales
            return sales;
        }
        // else, it is alphabetically before the current node, then update current node as right of itself
        else
            iter = iter->right;
    }

    // after the while loop
    // make node parent as temp, since it is just pointing to the iter->parent
    toadd->parent = temp;

    // if it is null, mean, RBT is empty, assign newly node to the root
    // then, simply recolor it as black, return sales
    if (temp == nullptr)
    {
        this->root = toadd;
        toadd->color = 0;
        return sales;
    }

    // if it will be left of the iter->parent (temp), make it left child
    else if (toadd->getpublisher() < temp->getpublisher())
        temp->left = toadd;
    // if it will be right of the iter->parent (temp), make it right child
    else
        temp->right = toadd;

    // if grandparent doesnt exist, since root is black, newly node is child of it and being red is no matter, no need to fix tree, return sales
    if (toadd->parent->parent == nullptr)
        return sales;

    // if we need to fix the tree, fix it
    if (needtofix)
        fixing(toadd);

    // return sales
    return sales;
}

// printing best sellers for given decade, year and sales
void print_best_seller(int d, int y, float na, string nap, float eu, string eup, float o, string op)
{
    string decade;
    if (d == 1)
        decade = "1st decade";
    else if (d == 2)
        decade = "2nd decade";
    else if (d == 3)
        decade = "3rd decade";
    else
        decade = "4th decade";
    cout << decade << " - (1980 - end of " << y << ")" << endl;
    //cout << "End of the " << y << " Year" << endl;
    cout << "Best seller in North America: " << nap << " - " << na << " million" << endl;
    cout << "Best seller in Europe: " << eup << " - " << eu << " million" << endl;
    cout << "Best seller rest of the World: " << op << " - " << o << " million" << endl;
}

int main(int argc, char const *argv[])
{
    // create Red-Black-Tree
    RBT *rbt = new RBT();

    // create delimeter
    char delimiter = ',';
    // counter and size will be used when file is reading.
    int counter = 0;
    int size = 16200;

    // string to store each token splitted with ','
    string data;
    // string to get line from file into
    string contents;

    // stl containers to store the line and the nodes
    vector<string> current_line;
    vector<Node> dataVector;

    // filename
    string filename = argv[1];

    // file object created
    fstream myFile(filename, ios::in);
    // if file is opened
    if (myFile.is_open())
    {
        // loop through the file, get the lines until reach the end
        while (getline(myFile, contents) && counter <= size)
        {
            // get contents into string stream "line", it will be used
            stringstream line(contents);
            // construct Node object
            Node node = Node("", 0, 0, 0, 0, 0, nullptr, nullptr, nullptr, 0);
            // loop through the line, split it wrt to delimeter
            while (getline(line, data, delimiter))
                // add each token (data) to the current line vector as distinct strings
                current_line.push_back(data);
            // if we are not at the beginning (column titles place), go through the line vector
            if (counter != 0)
            {
                // assign line items to the node object (please refer to the operator= overloading)
                node = current_line;
                // add node object to the vector of Node objects
                dataVector.push_back(node);
            }
            // clear the current line
            current_line.clear();
            // increment counter by one, next line
            counter++;
        }
    }
    // if not opened, throw an error
    else
        cerr << "File cannot be opened." << endl;

    // floats to store the best sales for each region
    float best_NA = 0;
    float best_EU = 0;
    float best_other = 0;

    // strings to store the bestsellers for each sale
    string best_NA_publisher = "";
    string best_EU_publisher = "";
    string best_other_publisher = "";

    // for printing each decade should be printed only once, take a bool
    bool already_printed_90 = false;
    bool already_printed_00 = false;
    bool already_printed_10 = false;
    bool already_printed_20 = false;

    // loop over the dataVector, insert to the RBT
    for (unsigned int i = 0; i < dataVector.size(); i++)
    {
        // before the insertion, check if the newly will be added node's year, if it passes the target year, prints will be done
        // if it is passing 1990
        if (dataVector[i].getYear() > 1990 && !already_printed_90)
        {
            // print best seller of the 1980-1990
            print_best_seller(1, 1990, best_NA, best_NA_publisher, best_EU, best_EU_publisher, best_other, best_other_publisher);
            // this boolean will be true since all years will be > than 1990 after now, but we only print at the first target exceed.
            already_printed_90 = true;
        }
        // if it is passing 2000
        if (dataVector[i].getYear() > 2000 && !already_printed_00)
        {
            // print best seller of the 1980-2000
            print_best_seller(2, 2000, best_NA, best_NA_publisher, best_EU, best_EU_publisher, best_other, best_other_publisher);
            // this boolean will be true since all years will be > than 2000 after now, but we only print at the first target exceed.
            already_printed_00 = true;
        }
        // if it is passing 2010
        if (dataVector[i].getYear() > 2010 && !already_printed_10)
        {
            // print best seller of the 1980-2010
            print_best_seller(3, 2010, best_NA, best_NA_publisher, best_EU, best_EU_publisher, best_other, best_other_publisher);
            // this boolean will be true since all years will be > than 2010 after now, but we only print at the first target exceed.
            already_printed_10 = true;
        }
        // if it is passing 2020, actually it wont, we will print this when we reach the end of file
        if (dataVector[i].getYear() > 2020 && !already_printed_20)
        {
            // print best seller of the 1980-2020
            print_best_seller(4, 2020, best_NA, best_NA_publisher, best_EU, best_EU_publisher, best_other, best_other_publisher);
            // this boolean will be true since all years will be > than 2020 after now, but we only print at the first target exceed.
            already_printed_20 = true;
        }
        // stl container to store the sales
        vector<float> temp = {0, 0, 0};
        // insert nodes, update sales
        temp = rbt->insertNode(&dataVector[i]);

        // temp[0] is EU sales, if it is greater than current best EUs, then it will be updated
        if (temp[0] > best_EU)
        {
            // update best EUs
            best_EU = temp[0];
            // update best EU's publisher name
            best_EU_publisher = dataVector[i].getpublisher();
        }
        // temp[1] is NA sales, if it is greater than current best NA, then it will be updated
        if (temp[1] > best_NA)
        {
            // update best NAs
            best_NA = temp[1];
            // update best NA's publisher name
            best_NA_publisher = dataVector[i].getpublisher();
        }
        // temp[1] is other sales, if it is greater than current best other sales, then it will be updated
        if (temp[2] > best_other)
        {
            // update best other
            best_other = temp[2];
            // update best others publisher name
            best_other_publisher = dataVector[i].getpublisher();
        }
    }
    // if until the 2020, 4th decade is not printed (we are end of the file but 2021 didnt come)
    if (!already_printed_20)
        // print best seller of the 1980-2020
        print_best_seller(4, 2020, best_NA, best_NA_publisher, best_EU, best_EU_publisher, best_other, best_other_publisher);

    // at the end, print the preorder traversal of Red-Black-Tree
    rbt->preorder(rbt->getroot());

    // end
    return 0;
}