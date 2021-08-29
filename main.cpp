#include <iostream>
#include <fstream>

using namespace std;                        // SORRY (--_--)

class Account{
	private:
        char name[10];
		int accountNumber;
		float balance;
	public:
		void set_data();
        void show_data();
		void read_data();
        void write_data();
        void check_balance();
        void add_to_balance();
        void transfer_between();
        void delete_acc();
};

void Account::set_data(){
	cout << "Enter person's name" << endl;
    cin.ignore();
	cin >> name;
	cout << "Enter accountNumber" << endl;
	cin >> accountNumber;
    cout << "Enter balance" << endl;
    cin >> balance;
}

void Account::show_data(){
    cout << "Client Name: " << name << endl; 
    cout << "Account Number: " << accountNumber << endl;
    cout << "Current Balance: "<< balance <<endl;
    cout << "-------------------------------" << endl;
}

void Account::write_data() {
    ofstream outData;
    outData.open("data.txt", ios::binary|ios::app);
    set_data();
    outData.write(reinterpret_cast<char *>(this), sizeof(*this));
    outData.close();
}

void Account::read_data() {
    ifstream inData;
    inData.open("data.txt", ios::binary);
    if(!inData) {
        cout << "Error in Opening! File Not Found!!" << endl;
        return;
    }
    cout << "----- Data from file -----" << endl;
    while(!inData.eof()) {
        if(inData.read(reinterpret_cast<char*>(this), sizeof(*this))) {
            show_data();
        }
    }
    inData.close();
}

void Account::check_balance() {
    int n;
    fstream inData;
    inData.open("data.txt", ios::in|ios::binary);
    if(!inData) {
        cout << "Error in opening! File Not Found!!"<<endl;
        return;
    }
    inData.seekg(0, ios::end);
    int count = inData.tellg()/sizeof(*this);
    cout << "There are " << count << " accounts in the file" << endl;
    cout << "Which account balance you wanna check" << endl;
    cin >> n;
    inData.seekg((n-1) * sizeof(*this));
    inData.read(reinterpret_cast<char*>(this), sizeof(*this));
    cout << name << " has " << balance << '$' << endl;
}

void Account::add_to_balance() {
    int addBalance, accountNum;
    fstream inData;
    inData.open("data.txt", ios::in|ios::binary);
    if(!inData) {
        cout << "Error in opening! File Not Found!!" << endl;
        return;
    }
    inData.seekg(0, ios::end);
    int count = inData.tellg()/sizeof(*this);
    cout << "There are " << count << " accounts in the file" << endl;
    cout << "Enter to which account add cash" << endl;
    cin >> accountNum;
    inData.seekg((accountNum - 1) * sizeof(*this));
    inData.read(reinterpret_cast<char*>(this), sizeof(*this));
    cout << "Current balance is - " << balance << endl;
    inData.close();
    inData.open("data.txt", ios::out|ios::in|ios::binary);
    inData.seekp((accountNum - 1) * sizeof(*this));
    cout << "How much you wanna add ?" << endl;
    cin >> addBalance;
    balance = balance + addBalance;
    inData.write(reinterpret_cast<char*>(this), sizeof(*this));
}

void Account::transfer_between() {
    int tranferAmount, accountNum, addBalance;
    fstream inData;
    inData.open("data.txt", ios::in|ios::binary);
    if(!inData) {
        cout << "Error in opening! File Not Found!!" << endl;
        return;
    }
    inData.seekg(0, ios::end);
    int count = inData.tellg()/sizeof(*this);
    cout << "There are " << count << " accounts in the file" << endl;
    cout << "Select from which one you want to tranfer" << endl;
    cin >> accountNum;
    inData.seekg((accountNum - 1) * sizeof(*this));
    inData.read(reinterpret_cast<char*>(this), sizeof(*this));
    cout << "Current balance is - " << balance << endl;
    inData.close();
    inData.open("data.txt", ios::out|ios::in|ios::binary);
    inData.seekp((accountNum - 1) * sizeof(*this));
    cout << "How much you want to transfer ?" << endl;
    cin >> tranferAmount;
    if(tranferAmount > balance) {
        cout << "Not enough balance. Do you want to add balance ?" << endl;
        cout << "1. Yes please" << endl << "2. No no, thanks!" << endl;
        cin >> addBalance;
        if(addBalance == 1) {
            Account::add_to_balance();
            return;
        }
        return;
    }
    balance -= tranferAmount;
    inData.write(reinterpret_cast<char*>(this), sizeof(*this));
    inData.close();
    inData.open("data.txt", ios::out|ios::in|ios::binary);
    cout << "Select account to wich will transfer" << endl;
    cin >> accountNum;
    inData.seekg((accountNum - 1) * sizeof(*this));
    inData.read(reinterpret_cast<char*>(this), sizeof(*this));
    inData.seekp((accountNum - 1) * sizeof(*this));
    balance += tranferAmount;
    inData.write(reinterpret_cast<char*>(this), sizeof(*this));
}

void Account::delete_acc() {
    int accountNum;
    ifstream inData;
    inData.open("data.txt", ios::binary);
    if(!inData) {
        cout << "Error in opening! File Not Found!!" << endl;
        return;
    }
    inData.seekg(0,ios::end);
    int count = inData.tellg()/sizeof(*this);
    cout << "There are " << count << " accounts in the file" << endl;
    cout << "Enter Account Number to Delete: " << endl;
    cin >> accountNum;
    fstream tmpData;
    tmpData.open("tmpData.txt", ios::out|ios::binary);
    inData.seekg(0);
    for(int i = 0; i < count; i++) {
        inData.read(reinterpret_cast<char*>(this),sizeof(*this));
        if(i == (accountNum - 1)) continue;
        tmpData.write(reinterpret_cast<char*>(this), sizeof(*this));
    }
    inData.close();
    tmpData.close();
    remove("data.txt");
    rename("tmpData.txt", "data.txt");
}

int main() {
    int choice;
	Account person;
    do {
        cout << "~~~~~~~~~~ Bank Recording System ~~~~~~~~~~" << endl;
        cout << "Please choose one of these variants" << endl;
        cout << "1. Create new account" << endl;
        cout << "2. Check account's balance" << endl;
        cout << "3. Add money to account" << endl;
        cout << "4. Transfer money between accounts" << endl;
        cout << "5. See all accounts" << endl;
        cout << "6. Delete account" << endl;
        cin >> choice;
        switch (choice) {
            case 1:
                person.write_data();
                break;
            case 2:
                person.check_balance();
                break;
            case 3:
                person.add_to_balance();
                break;
            case 4:
                person.transfer_between();
                break;
            case 5:
                person.read_data();
                break;
            case 6:
                person.delete_acc();
                break;
        default:
            cout << "error occurred" << endl;
            break;
        }
    } while (choice < 7 && choice > 0);
    system("pause");
	return 0;
}