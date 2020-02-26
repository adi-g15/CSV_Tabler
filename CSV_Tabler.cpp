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

//OPINION - Consider changing the 'heading vector' to std::list, since - "The vector<T>::erase operation could be expensive and if it is frequently used the user should consider using std::list "

#include<iostream>
#include<fstream>
#include<string>
#include<vector>

using namespace std;

string getString(int num){
    string in,out;
    while (num>0)   //get digits in string
    {
        in.push_back(num%10);
        num/=10;
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

struct id_box{  //NO DATA CAN BE INSERTED TO ANY COLUMN UNTIL YOU HAVE ITS ID! ;D (Maybe Low Level Protection, but for me It's really GOOD)
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
    private:
//        string id;  //i preferred not to use a pointer to an id_box, bcz it just didnt feel ;D
            //Removed it, since What use would it serve if it changes here, but can't be changed in the id list...
        vector<contain_box> containers; //Note- Can simply use a vector of strings
    public:
        string title;
        unsigned int content_length;
        const contain_box* at(size_t);
        void addContBox(contain_box);
        head_box(){
            content_length = 0;
        }
};

struct contain_box  //LIMITATION - Currently all content boxes, irrespective of the title, will be ALL same
{
    string content;
};

class CSV_Tabler{
private:
    vector<id_box> ids;
    unsigned int clength;    //how many container boxes under each heading
        //NOTE- Keep verifying after every operations
        //IMP NOTE- clength is updated only after all columns have reached that many rows
public:
    unsigned int hlength;    //how many heading/title boxes
        //NOTE - It was private earlier (made public since the headings themselves are already public), so many places it is used, BUT 'hlength' and 'titles.size()' are interchangeable
    vector<head_box> titles;
    bool verifyLen(void);
    head_box* retBox(string id);    //takes id, returns pointer to the header, here, this class, and the two others will contact each other to finally update a head_box...
            //LIMITATION - This removes chances of having multiple columns with same title (though, not much necessaryy, but then this uniqueid stuff doesnt make much sense)
    void addContentBox(string,int); //take ALL args needed for the box
    void addHeadBox(string); //take ALL args needed for the box
    void addEmpBoxes(void); //stabilizes the difference between row size in different columns
    void removeEmptyColumns(void);
    void display(void);
    CSV_Tabler(void);
    ~CSV_Tabler(void);
};

void CSV_Tabler::removeEmptyColumns(){
    bool isEmpty=true;
    for(int i=0; i<hlength; i++){
        for(int j=0; j<titles[i].content_length; j++){
            if(titles[i].at(j)->content != ""){
                isEmpty = false;
            }
        }
        if(isEmpty){
            titles.erase(titles.begin()+i);
            ids.erase(ids.begin()+i);
            --hlength;
        }
        isEmpty = true;
    }
}

void CSV_Tabler::display(){
    int i,j;
    cout<<'|';
    for (i = 0; i < titles.size(); i++) //TODO - for proper spacing, have a data member having count of max length that any element of it has in that column
    {
        cout<<' '<<titles[i].title<<" | ";
    }
    this->addEmpBoxes();    //Just to ensure accessing content doesn't give out of bound
    cout<<'\n';
    for(i=0; i<titles[i].content_length; ++i){
        for(j=0; j<hlength; j++ ){
            cout<<' '<<titles[j].at(i)->content<<"  |";
        }
        cout<<endl;
    }
}

void CSV_Tabler::addEmpBoxes(){ //Whenever this is called from addContentBox() then the difference will be 1 or 0 only
    int i, max_clen = 0;
    contain_box c_box;
    c_box.content = "";

    for(i=0; i<hlength; ++i){   //taking maximum number of rows
        if(titles[i].content_length > max_clen) max_clen=titles[i].content_length;
    }

    for(i=0; i<hlength; ++i){   //for maintaining same number of rows in all titles
        if(titles[i].content_length < max_clen){   //That is those columns that are lagging in number of rows
            retBox(ids[i].getID())->addContBox(c_box);
            i--;    //to check the same column again and again until it isn't balanced to the longest column
        }
    }
    clength = max_clen;  //finally changed clength, after solving the case of lagging columns
}

CSV_Tabler::CSV_Tabler()
{
    hlength = clength = 0;
}

CSV_Tabler::~CSV_Tabler()
{   //All containers do this already
}

int main(){
    CSV_Tabler All_Contacts, NITP_Contacts;
  
    struct node
    {
        string title;
        int index;
        void set(string title, int index){
            this->title = title;
            this->index = index;
        }
        node(){
            this->title = "";
            this->index = 0;
        }
    };
    
    vector<node> tmp_vec;   //temporary; Just for simply using this and not ids in main, to KISS
    string filein;
    filein = "NITP_Contacts.csv";
    ifstream fin(filein,ios::in);
    fin.seekg(0,ios::beg);
    if(!fin.eof()){
        char c;
        string title;
        string content;
        int tempindex = 0;
        node tmpnode;
        
        while(c != '\n' && !fin.eof()){
            fin>>noskipws>>c;
            if(c == ','){   //no need to push this to string
                //add heading 'title'
                tmpnode.set(title, tempindex);
                tmp_vec.push_back(tmpnode);
                ++tempindex;
                NITP_Contacts.addHeadBox(title);
                title.erase();  //position and n being default
            }
            else
            {
                title.push_back(c);
            }
        }

        cout<<endl;

        while(!fin.eof()){    //This one for reading the content rows
            tempindex=0;
            while(c != '\n' || !fin.eof()){
                fin>>noskipws>>c;
                if(c == ','){   //no need to push this to string
                    //add heading 'title'
                    tmpnode.set(title, tempindex);
                    tmp_vec.push_back(tmpnode);
                    ++tempindex;
                    if(tempindex == NITP_Contacts.hlength)  tempindex=0;
                        //Though, ideally the above condition should never meet since the loop ends at newline
                    NITP_Contacts.addContentBox(content,tempindex);
                    content.erase();  //position and n being default
                }
                else
                {
                    content.push_back(c);
                }
            }
            cout<<endl;
        }
    }
//    NITP_Contacts.display();
    NITP_Contacts.removeEmptyColumns();
    NITP_Contacts.display();
    cout<<"Ran good!\n";
    return 0;
}

bool CSV_Tabler::verifyLen(){
    bool h_check = false;   //Can find a wrong case here too |D
    bool c_check = true;    //just finding a wrong case
    if(ids.size() == hlength){
        if(titles.size() == hlength){
            h_check = true;
        }
        else{
            cout<<"Length of Title vector different than headerlength";
            cout<<endl;
            return false;}
    }
    else{
        cout<<"Length of ID vector different than headerlength";
        cout<<endl;
        return false;   }

    for (int i = 0; i < hlength; i++)
    {
        if(titles[i].content_length != clength){
            cout<<"Length of Content vector at column index "<<i<<" different than clength";
            cout<<endl;
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

const contain_box* head_box::at(size_t index){
                            //1st index is 0
    if(index<0 || index >= containers.size()){
        cout<<"\n\nInvalid Index... Returning NULL. Be Careful or Quit.\n\n";
        return NULL;
    }
    else return &(containers[index]);
}


head_box* CSV_Tabler::retBox(string id){
    size_t index=titles.size();    //ie. after end of the vector
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
    if(index >= titles.size() || index < 0){
        cerr<<"Invalid Index";
        return;
    }
    if(titles[index].content_length > clength ){   //ie. difference of number of boxes from neighbourhood boxes already greater than 1 but triying to add one more... So to balance out, empty boxes will be added to other columns
        this->addEmpBoxes();
    }                                                    //1st index is 0

    contain_box c_box;
    c_box.content = content;
    //we can't directly access to which title to add this block
    retBox(ids[index].getID())->addContBox(c_box);
}

void CSV_Tabler::addHeadBox(string title){
    head_box hbox;
    string newId = string("AdiG15_") + getString(hlength+1); //create the IDs with some RANDOM strings
    id_box newIdB(newId);
//    hbox.id = newId;
    hbox.title = title;
    ++hlength;
    ids.push_back(newIdB);
    titles.push_back(hbox);
    if(!verifyLen()){
        cout<<"\n\n\n\n\n\nJust QUIT the Program... The data has gone haywire!!!!\n\n\n\n\n\n";
        cout<<endl;
    }
}