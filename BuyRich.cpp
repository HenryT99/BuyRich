
//=======================================================
//Dự án viết bằng cpp với công cụ là danh sách liên kết đôi
//Ngày bắt đầu 15/01/2019
//Ngày kết thúc 15/02/2019
//Hiện vẫn còn một vài phần chưa xong do thiếu công cụ. 
//Dùng để bán hàng và thanh toán với mật khẩu người mỗi ca làm.


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#define _console_header
#define _console_header

#define ColorCode_Back 0
#define ColorCode_DarkBlue 1
#define ColorCode_DarkGreen 2
#define ColorCode_DarkCyan 3
#define ColorCode_DarkRed 4
#define ColorCode_DarkPink 5
#define ColorCode_DarkYellow 6
#define ColorCode_DarkWhite 7
#define ColorCode_Grey 8
#define ColorCode_Blue 9
#define ColorCode_Green 10
#define ColorCode_Cyan 11
#define ColorCode_Red 12
#define ColorCode_Pink 13
#define ColorCode_Yellow 14
#define ColorCode_White 15

#define default_ColorCode 7

#define key_Up 1072
#define key_Down 1080
#define key_Left 1075
#define key_Right 1077
#define key_none -1
using namespace std;

int inputKey()
{
	if (kbhit())
	{
		int key = getch();

		if (key == 224)	// special key
		{
			key = getch();
			return key + 1000;
		}

		return key;
	}
	else
	{
		return key_none;
	}

	return key_none;
}


//-------------------------Screen-------------------------
void clrscr()
{
	CONSOLE_SCREEN_BUFFER_INFO	csbiInfo;                  
	HANDLE	hConsoleOut;
	COORD	Home = {0,0};
	DWORD	dummy;

	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut,&csbiInfo);

	FillConsoleOutputCharacter(hConsoleOut,' ',csbiInfo.dwSize.X * csbiInfo.dwSize.Y,Home,&dummy);
	csbiInfo.dwCursorPosition.X = 0;
	csbiInfo.dwCursorPosition.Y = 0;
	SetConsoleCursorPosition(hConsoleOut,csbiInfo.dwCursorPosition);
}


//screen: goto [x,y]
void gotoXY (int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}


//screen: get [x]
int whereX()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return csbi.dwCursorPosition.X;
	return -1;
}


//screen: get [y]
int whereY()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return csbi.dwCursorPosition.Y;
	return -1;
}


void TextColor (int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE) , color);
}



enum Kind
{
    NO,
    DRINK,
    FOOD
};

void ConSole()
{
    // system("color 80");
    gotoXY(30, 10);
    //TextColor(8);
    //setFontSize(20);
    cout << "Welcome to Buy For Rich!";
    Sleep(1000);
    clrscr();
    // system("color 70");
    TextColor(8);
    //setFontSize(20);
}

// struct BillList
// {
//     vector<Node> * N;
// 	Node *z;
// };

struct SanPham
{
    string Name;
    unsigned long Price;
    Kind KindFood;
    int OrdNumber;
};

struct Node
{
    SanPham sP;
    Node *pNext;
    Node *pPre;
};

class ListFood
{
  protected:
    Node *pHead;
    Node *pTail;
    SanPham sP;

  public:
    ListFood()
    {
        pHead = pTail = NULL;
        sP.KindFood = NO;
        sP.Name = "0";
        sP.Price = 0;
        sP.OrdNumber = 0;
    }

    ~ListFood()
    {
        while (pHead)
        {
            Node *p = pHead;
            pHead = pHead->pNext;
            delete p;
        }
        pHead = pTail = NULL;
    }

    Node *GetNode(SanPham sp)
    {
        Node *p = new Node;
        if (p == NULL)
        {
            return NULL;
        }
        p->sP = sp;
        p->pNext = NULL;
        p->pPre = NULL;
        return p;
    }


    unsigned long Price_Change()
    {
        unsigned long u = 0;
    ABC:
        string k = "0";
        cout << "\nPrice of Food: ";
        cin >> k;
        for (unsigned long i = 0; i < k.length(); i++)
        {
            if (!(k.at(i) >= 48 && k.at(i) <= 57))
            {
                cout << "\nGia tri khong dung! Nhap lai: ";
                goto ABC;
            }
            else
            {
                u = (unsigned long)(k[i] - '0') + u * 10;
            }
        }
        return u;
    }

    SanPham InPutSp(SanPham &sp)
    {
        cout << "\nName: ";
        fflush(stdin);
        getline(cin, sp.Name);
        cout << "\nKind of Food";
        cout << "\n0. None of Kind";
        cout << "\n1. Drink";
        cout << "\n2. Food";
        int choice;
        cin >> choice;
        if (choice == 1)
        {
            sp.KindFood = DRINK;
        }
        else if (choice == 2)
        {
            sp.KindFood = FOOD;
        }
        else
        {
            sp.KindFood = NO;
        }
        sp.Price = Price_Change();
        return sp;
    }

    void AddTail(Node *p)
    {
        if (pHead == NULL)
        {
            pHead = pTail = p;
        }
        else
        {
            pTail->pNext = p;
            p->pPre = pTail;
            pTail = p;
        }
    }

    void CreateList()
    {
        int n;
        cout << "Input Number of Product: ";
        cin >> n;
        ofstream oFile("SanPham_1.txt");

        for (int i = 0; i < n; i++)
        {
            sP = InPutSp(sP);
            sP.OrdNumber = i + 1;
            AddTail(GetNode(sP));
        }
        oFile << n;
        oFile.close();
        SaveData();
    }

    void confirmList()
    {
        ifstream iFile;
        int n;
        iFile.open("SanPham_1.txt");
        iFile >> n;
        iFile.close();
        if (n == 0)
        {
            cout << "\nYou haven't had any Product. Please insert ";
            CreateList();
            cout << "\nCreate List success";
        }
    }

    void addFoodinList(){
        int n, t;
        ifstream iFile("SanPham_1.txt");
        iFile >> n;
        iFile.close();
        cout << "\nAmount of Food: ";
        cin >> t;
        for(int i = 1; i <= t; i++){
            n += 1;
            sP=InPutSp(sP);
            sP.OrdNumber = n;
            AddTail(GetNode(sP));
        }
        ofstream oFile("SanPham_1.txt");
        oFile << n;
        oFile.close();
        SaveData();
    }

    void LoadList()
    {
        ifstream iFile;
        iFile.open("SanPham_1.txt");
        int n;
        iFile >> n;
        iFile.close();
        iFile.open("SanPham.txt");
        for (int i = 0; i < n; i++)
        {
            iFile >> sP.Name;
            iFile >> sP.Price;
            int t;
            iFile >> t;
            switch (t)
            {
            case 0:
                sP.KindFood = NO;
                break;
            case 1:
                sP.KindFood = DRINK;
                break;
            case 2:
                sP.KindFood = FOOD;
                break;
            }
            iFile >> sP.OrdNumber;

            AddTail(GetNode(sP));
        }
        iFile.close();
    }

    SanPham LoadData(SanPham &sp)
    {
        ifstream iFile;
        iFile.open("SanPham.txt");
        iFile >> sp.Name;
        int t;
        iFile >> t;
        switch (t)
        {
        case 0:
            sp.KindFood = NO;
            break;
        case 1:
            sp.KindFood = DRINK;
            break;
        case 2:
            sp.KindFood = FOOD;
            break;
        }
        iFile >> sp.Price;
        iFile.close();
        return sp;
    }

    //

    void SaveData()
    {
        ofstream oFile("SanPham.txt");
        for (Node *p = pHead; p; p = p->pNext)
        {

            oFile << p->sP.Name << endl;
            oFile << p->sP.Price << endl;
            oFile << p->sP.KindFood << endl;
            oFile << p->sP.OrdNumber << endl;
        }
        oFile.close();
    }

    void OutPut()
    {
        for (Node *p = pHead; p; p = p->pNext)
        {
            cout << p->sP.Name << endl;
            cout << p->sP.KindFood;
            cout << endl;
            cout << p->sP.Price << endl;
        }
    }
};

class ID_User : public ListFood
{
  protected:
    string ID, Password;

  public:
    ID_User()
    {
        ID = "0";
        Password = "0";
    }

    //Hàm xóa phần tử mảng
    void deleteElementArray(char a[10], int Delete)
    {
        char *aptr = a;
        if (Delete == 0)
        {
            *(aptr) = '\0';
        }
        else
        {
            *(aptr + Delete) = '\0';
        }
    }

    //Hàm nhập password có kiểu trả về là string
    string InPass(string &t)
    {
        t = "0";
        char x = '0';
        char *PW = new char[10];
        ifstream iFile;
        int k;
        iFile.open("ID_User_1.txt");
        iFile >> k;
        iFile.close();
        gotoXY(5, 14);
        cout << "Password: ";
        int size = 0;
    Thai1:

        while (x != 13)
        {
            x = getch();
            if (x == 13)
            {
                if (k == 1)
                    break;
                else if (k == 0 && size < 9)
                {
                    cout << "\nPassword Has 10 Character. Try Again";
                    cout << "\nPassword: ";
                    size = 0;
                    x = '0';
                    goto Thai1;
                }
            }
            else if (x == 8 && size >= 0)
            {

                deleteElementArray(PW, size);
                if (size > 0) //Cần trọn vị trí để n = 0 không tác dụng xóa dòng PASSWORD
                {
                    cout << "\b \b";
                    size--;
                }
            }

            else
            {
                PW[size] = x;
                size++;
                cout << '*';
            }
        }
        PW[10] = '\0';
        for (int i = 0; i < 10; i++)
        {
            if (i == 0)
            {
                t.at(i) = PW[i];
            }
            else
            {
                t.insert(t.end(), PW[i]);
            }
        }
        delete PW;
        return t;
    }

    //Hàm xác định có mật khẩu hay chưa
    void ConfirmExistID()
    {
        ifstream iFile;
        iFile.open("ID_User_1.txt");
        int t;
        iFile >> t;
        iFile.close();

        if (t == 0)
        {
            cout << "You haven't had ID. Please register" << endl;
            RegisterUser(ID, Password);
            ofstream oFile("ID_User_1.txt");
            oFile << '1' << endl;
            oFile.close();
            cout << "\nCreate ID Success!";
            confirmList();
        }
        else
        {
            cout << "Get Login";
        }
    }

    //Hàm nhập ID và Pass vào file

    void RegisterUser(string &t, string &h)
    {
        ofstream oFile("ID_User.txt");
        gotoXY(5, 10);
        cout << "ID User: ";
        cin >> t;
        oFile << t << endl;
        h = InPass(Password);
        oFile << h;
        oFile.close();
    }

    //Hàm nhập ID và Pass

    void Input(string &Id, string &PassWord)
    {
        gotoXY(5, 12);
        cout << "ID: ";
        cin >> Id;
        PassWord = InPass(Password);
        system("cls");
    }

    bool SignIn()
    {
        system("cls");
        gotoXY(5, 10);
        cout << "Get Login: \n";
        bool t = false;
        string t1, h1;
        ifstream iFile;
        iFile.open("ID_User.txt");
        Input(ID, Password);
        iFile >> t1;
        iFile >> h1;
        iFile.close();
        int i = 0;
        while (t1 != ID || h1 != Password)
        {
            gotoXY(5, 10);
            cout << "ID or Password is wrong, please again!";
            Input(ID, Password);
            i++;
            if (i == 3)
            {
                break;
            }
        }
        if (i == 3)
        {
            t = false;
        }
        else
        {
            t = true;
        }
        return t;
    }
};

class Purchasing : public ID_User
{
  private:
    unsigned long sum;
    Node *bHead, *bTail;

  public:
    Purchasing()
    {
        // ListFood();
        sum = 0;
        //sumPrice_Bill = 0;
        bHead = bTail = NULL;
    }

    ~Purchasing()
    {
        while (bHead)
        {
            Node *p = bHead;
            bHead = bHead->pNext;
            delete p;
        }
        bHead = bTail = NULL;
    }

    void addbTail(Node *p)
    {
        if (bHead == NULL)
        {
            bHead = bTail = p;
        }
        else
        {
            bTail->pNext = p;
            p->pPre = bTail;
            bTail = p;
        }
    }

    Node *searchFoodbyName() //Kiểu trả về là Node *.
    {
        string t;
        cout << "\nName Food: ";
        cin >> t;
        Node *q = new Node;
        //LoadList();
        for (Node *p = pHead; p; p = p->pNext)
        {
            if (p->sP.Name == t)
            {
                q->sP = p->sP;
                q->pNext = NULL;
                q->pPre = NULL;
                return q;
            }
        }
        return NULL;
    }

    Node *searchFoodbyPrice(unsigned long t)
    {
        Node *q = new Node;
        //LoadList();
        for (Node *p = pHead; p; p = p->pNext)
        {
            if (p->sP.Price == t)
            {
                q = p;
                q->pNext = NULL;
                q->pPre = NULL;
                return q;
            }
        }
        return NULL;
    }

    void acceptProduct()
    {
        int choice;
        system("cls");
        cout << "1. Add\n";
        cout << "2. Not\n";
        cin >> choice;
        while (choice == 1)
        {
            Node *p = searchFoodbyName();
            if (p != NULL)
            {
                addbTail(p);
                cout << "\nAdd Success";
            }
            else
            {
                cout << "\nProduct is not existed!";
            }
            system("cls");
            cout << "1. Add\n";
            cout << "2. Not\n";
            cin >> choice;
        }
    }

    void viewListFood() // Hiện danh sách sản phẩm của shop
    {
        system("cls");
        //LoadList();
        gotoXY(3, 0);
        cout << "NameFood";
        gotoXY(23, 0);
        cout << "Kind Food";
        gotoXY(43, 0);
        cout << "Price";
        int temp = 1;
        for (Node *p = pHead; p; p = p->pNext)
        {
            gotoXY(0, 3 * temp);
            cout << p->sP.OrdNumber;
            gotoXY(3, 3 * temp);
            cout << p->sP.Name;
            int choice = p->sP.KindFood;
            gotoXY(23, 3 * temp);
            switch (choice)
            {
            case 1:
                cout << "Drink";
                break;
            case 2:
                cout << "Food";
                break;
            case 0:
                cout << "None";
                break;
            }
            gotoXY(43, 3 * temp);
            cout << p->sP.Price;
            temp++;
        }
        cout << endl;
        cout << "Input Oder Number. Press '0' to end\n";
        cout << "Order Number of Food: \n";
    }

    void disLoadList()
    {
        while (pHead)
        {
            Node *p = pHead;
            pHead = pHead->pNext;
            delete p;
        }
        pHead = pTail = NULL;
    }

    void addFoodinBill()
    {

        viewListFood();
        int temp, h;
        ifstream iFile;
        iFile.open("SanPham_1.txt");
        iFile >> temp;
        iFile.close();
        while (1)
        {
            fflush(stdin);
            cin >> h;
            if (h > temp)
            {
                cout << "Food is not existed!";
            }
            else if (h == 0)
            {
                break;
            }
            else
            {
                for (Node *p = pHead; p; p = p->pNext)
                {
                    if (p->sP.OrdNumber == h)
                    {
                        Node *q = new Node;
                        q->sP.KindFood = p->sP.KindFood;
                        q->sP.OrdNumber = p->sP.OrdNumber;
                        q->sP.Name = p->sP.Name;
                        q->sP.Price = p->sP.Price;
                        q->pNext = NULL;
                        q->pPre = NULL;
                        addbTail(q);
                        break;
                    }
                }
            }
        }
        //disLoadList();
    }

    void payBill(unsigned long &t)
    {
        system("cls");
        //ifstream iFile;
        int temp = 1;
        cout << "Kind of Food";

        gotoXY(20, 0);
        cout << "Food Name";
        gotoXY(40, 0);
        cout << "Price";
        //cout << endl;
        int choice;
        for (Node *p = bHead; p; p = p->pNext)
        {
            choice = p->sP.KindFood;
            switch (choice)
            {
            case 1:
                //gotoXY(50, temp);
                cout << "\nDrink";
                break;
            case 2:
                //gotoXY(50, temp);
                cout << "\nFood";
                break;
            case 0:

                cout << "\nNone";
                break;
            }
            gotoXY(20, temp);
            cout << p->sP.Name;
            gotoXY(40, temp);
            t += p->sP.Price;
            cout << p->sP.Price;
            choice = p->sP.KindFood;
            temp++;
        }
        gotoXY(8, temp);
        cout << "\nSum of Bill: " << t;
        //Purchasing();
        cout << "\nTake Payment Success!";
        while (bHead)
        {
            Node *p = bHead;
            bHead = bHead->pNext;
            //bHead -> pPre = NULL;
            delete p;
        }
        bHead = bTail = NULL;
    }

    void deleteProduct(Node *p)
    {
        if (p == NULL)
        {
            cout << "Product is not existed";
        }
        else
        {
            if (p->sP.Name == bHead->sP.Name)
            {
                Node *q = bHead;
                bHead = bHead->pNext;
                bHead->pPre = NULL;
                delete q;
                return;
            }
            else if (p->sP.Name == bTail->sP.Name)
            {
                Node *q = bTail;
                bTail = bTail->pPre;
                bTail->pNext = NULL;
                delete q;
                return;
            }
            else
            {
                for (Node *q = bHead; q; q = q->pNext)
                {
                    if (q->sP.Name == p->sP.Name)
                    {
                        Node *pre = q->pPre;
                        Node *next = q->pNext;
                        pre->pNext = next;
                        next->pPre = pre;
                        delete q;
                        return;
                    }
                }
            }
        }
    }

    void cancelBill()
    {
        while (bHead)
        {
            Node *p = bHead;
            bHead = bHead->pNext;
            //bHead -> pPre = NULL;
            delete p;
        }
        bHead = bTail = NULL;
        cout << "\nCancel Success!";
    }

    void viewBill()
    {
    AddMore:
        system("cls");
        cout << "Your Bill Has";
        //ifstream iFile;
        int temp = 1;
        int t = 0;
        gotoXY(0, 1);
        cout << "Kind of Food";

        gotoXY(20, 1);
        cout << "Food Name";
        gotoXY(40, 1);
        cout << "Price";
        //cout << endl;
        int choice;
        for (Node *p = bHead; p; p = p->pNext)
        {
            choice = p->sP.KindFood;
            switch (choice)
            {
            case 1:
                //gotoXY(50, temp);
                cout << "\nDrink";
                break;
            case 2:
                //gotoXY(50, temp);
                cout << "\nFood";
                break;
            case 0:

                cout << "\nNone";
                break;
            }
            gotoXY(20, temp + 1);
            cout << p->sP.Name;
            gotoXY(40, temp + 1);
            t += p->sP.Price;
            cout << p->sP.Price;
            choice = p->sP.KindFood;
            temp++;
        }
        gotoXY(8, temp + 1);
        cout << "\nSum of Bill: " << t;
        cout << "\n\n1.Pay Bill";
        //cout << "\n\n2.Add";

        cout << "\n\n2.Add more";
        cout << "\n\n3.Cancel Bill\n";
        cout << "\n4.Delete Food";
        cin >> choice;
        switch (choice)
        {
        case 1:
            payBill(sum);
            //disLoadList();
            break;
        case 2:
            addFoodinBill();
            goto AddMore;
            break;
        case 3:
            cancelBill();
            //disLoadList();
            break;
        case 4:
            deleteProduct(searchFoodbyName());
            goto AddMore;
            break;
        }
        cout << endl;
        getch();
    }

    void listToDo()
    {
        while (1)
        {
            ConfirmExistID();
            if (SignIn() == true)
            {
                gotoXY(8, 10);
                cout << "Sign In success!";
                Sleep(2000);
                LoadList();
                system("cls");
                while (1)
                {
                    fflush(stdin);
                    cout << "Start your shift";
                    cout << "\nAdd Food\n";

                    switch (getch())
                    {
                    case 49:
                        while (1)
                        {
                            fflush(stdin);
                            system("cls");
                            cout << "Sell";
                            cout << "\nEnd your shift";
                            if (getch() == 49)
                            {
                                fflush(stdin);
                                addFoodinBill();
                                viewBill();
                            }
                            else if (getch() == 50)
                            {
                                cout << "\nEnd your Shift is Successed!\n";
                                break;
                            }
                        }
                        break;
                    case 50:
                        addFoodinList();
                        break;
                    }
                }
            }
            else
            {
                gotoXY(8, 10);
                cout << "Sign In Failed 3 times, you can't Sign in";
                Sleep(2000);
                break;
            }
        }
    }
};

// class allBill{

// protected:
// Node* heads[101];
// Node *z;

// public:
//     allBill(){
//         z = new Node;
//         z->pNext = z;
//         for(int i = 0;i < 101; i++){
//             heads[i]= z;
//         }
//     }

//     void addBill(Node *p){
//         z->sP.Name = p->sP.Name;
//     }
// };



int main()
{
    ConSole();
    Purchasing pC;
    //pC.listToDo();
    pC.listToDo();
    getch();
    return 0;
}