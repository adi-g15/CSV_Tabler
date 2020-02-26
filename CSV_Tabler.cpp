/*
LEARNT
============

1. Using a pointer to a class object of which the class is declared later, but you cant define it before, since that too uses a pointer to this class... this was a problem i faced in the Attendance_Register
Solution - We can just 'declare' a structure, ie. that there will be a structure with so and so name... i really didn't knew before that this is possible ;D
        Note- But it shows "Class_name has Incomplete type", if you use it with a name in a method, but havent defined the class by then
*/

/*
This just started as a Casual Program to read the CSV file of contacts, only the classes and the methods end up to be a 183 line program ;D 'O'
*/

#include<iostream>
#include<string>
#include<vector>

using namespace std;

string getString(int num){
    string in,out;
    while (num>0)   //get digits in string
    {
        in.push_back(num%10);
    }
    out.reserve(in.size());
    for (int i = in.size()-1; i >= 0; i--)  //just reverse the string
    {
        out.push_back(in[i]);
    }
    return out;
}

struct head_box;
struct contain_box;

struct id_box{
    private:
        string id;
    public:
//        head_box *ptr;    //NO DIRECT CONNECTION B/W HEAD AND ID_bOX
        const string getID(){
            return this->id;
        }
        id_box(string id){
            this->id = id;
        }
};

struct head_box{
    string title;
    int content_length;
    const contain_box* at(int);
    void addContBox(contain_box);
    head_box(){
        content_length = 0;
    }
    private:
//        string id;  //i preferred not to use a pointer to an id_box, bcz it just didnt feel ;D
            //Removed it, since What use would it serve if it changes here, but can't be changed in the id list...
        vector<contain_box> containers; //Note- Can simply use a vector of strings
};

struct contain_box  //LIMITATION - Currently all content boxes, irrespective of the title, will be ALL same
{
    string content;
};

class CSV_Tabler{
private:
    vector<id_box> ids;
    int hlenght;    //how many heading/title boxes
    int clength;    //how many container boxes under each heading
        //NOTE- Keep verifying after every operations
public:
    vector<head_box> titles;
    bool verifyLen();
    head_box* retBox(string id);    //takes id, returns pointer to the header, here, this class, and the two others will contact each other to finally update a head_box...
            //LIMITATION - This removes chances of having multiple columns with same title (though, not much necessaryy, but then this uniqueid stuff doesnt make much sense)
    void addContentBox(string,int); //take ALL args needed for the box
    void addHeadBox(string); //take ALL args needed for the box
    CSV_Tabler();
    ~CSV_Tabler();
};

CSV_Tabler::CSV_Tabler()
{
    hlenght = clength = 0;
}

CSV_Tabler::~CSV_Tabler()
{   //All containers do this already
}

int main(){
    CSV_Tabler All_Contacts;
//    All_Contacts.addHeadBox("Names"); //Causing std::bad_alloc
    All_Contacts.addContentBox("AdityaG",0);
    cout<<"Ran good!\n";
    return 0;
}

bool CSV_Tabler::verifyLen(){
    bool h_check = false;   //Can find a wrong case here too |D
    bool c_check = true;    //just finding a wrong case
    if(ids.size() == hlenght){
        if(titles.size() == hlenght){
            h_check = true;
        }
        else return false;
    }
    else return false;

    for (int i = 0; i < hlenght; i++)
    {
        if(titles[i].content_length != clength){
            c_check = false;
            return false;
        }
    }

    return true;    //This line removes need of c_check but that seems better   
}

void head_box::addContBox(contain_box newContBox){    //Not private, but still difficult to access a particular box
        containers.push_back(newContBox);
        ++content_length;
}

const contain_box* head_box::at(int index){
                            //1st index is 0
    if(index<0 || index>=containers.size()){
        cout<<"\n\nInvalid Index... Returning NULL. Be Careful or Quit.\n\n";
        return NULL;
    }
    else return &(containers[index]);
}


head_box* CSV_Tabler::retBox(string id){
    int index=titles.size();    //ie. after end of the vector
    for(int i=0; i<ids.size(); ++i){
        if(ids[i].getID() == id){
            index = i;
            break;
        }
    }
    if(index == titles.size())  return NULL;
    else    return &titles[index];
}

void CSV_Tabler::addContentBox(string content, int index){  //this index then links to the unique id, then the head_box which will add this
    contain_box c_box;
    if(titles[index].content_length > clength ){   //ie. difference of number of boxes from neighbourhood boxes already greater than 1 but triying to add one more... So to balance out, empty boxes will be added to other columns
        vector<int> indices;

        for(int i=0; i<hlenght; ++i){   //for maintaining same number of rows in all titles
            if(i == index)  continue;
            if(titles[i].content_length == clength){   //That is those columns that are lagging in number of rows
                   //NOTE - Not doing anything from other than case of 0 and 1 difference, though thr function will ensure that doesn't even happen, but anyways for now, it lacks backup for managing with such cases
                indices.push_back(i);
            }
        }

        for (int i = 0; i < indices.size(); i++)
        {
            c_box.content = "";
            retBox(ids[indices[i]].getID())->addContBox(c_box);            
        }

        ++clength;  //finally changed clength, after solving the case of lagging columns
    }                                                    //1st index is 0

    c_box.content = content;
    //we can't directly access to which title to add this block
    retBox(ids[index].getID())->addContBox(c_box);
}

void CSV_Tabler::addHeadBox(string title){
    head_box hbox;
    string newId = string("AdiG15_") + getString(hlenght+1); //create the IDs with some RANDOM strings
    id_box newIdB(newId);
//    hbox.id = newId;
    hbox.title = title;
    ++hlenght;
    if(!verifyLen()){
        cout<<"\n\n\n\n\n\nJust QUIT the Program... The data has gone haywire!!!!\n\n\n\n\n\n";
    }
}