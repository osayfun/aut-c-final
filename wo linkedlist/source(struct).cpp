#include <iostream>
#include <cstring>
#include <cstdio>
#include <conio.h>
#include <stdlib.h>
#include <ctime>
#include <sys/stat.h>
#define DATABASE "database.txt"
#define MAX1 100
#define MAX2 1000
#define MAX3 8
#define MAX4 12
#define MAX5 4
using namespace std;

struct Person{
  char name[MAX1]; //1
  char major[MAX1]; //2
  char address[MAX2]; //3
  char id[MAX3]; //4
  char number[MAX4]; //5
  bool gender;// Male = true //6
  char age[MAX5]; //7
};

void printPerson(Person person, int record = 0){
  if( record )
    cout << "Record:\t\t" << record << endl;
  cout << "ID:\t\t" << person.id << endl;
  cout << "Name:\t\t" << person.name << endl;
  cout << "Major:\t\t" << person.major << endl;
  cout << "Number:\t\t" << person.number << endl;
  if( person.gender ){

    cout << "Gender:\t\tMale\n";
  }else{
    cout << "Gender:\t\tFemale\n";
  }
  cout << "Age:\t\t" << person.age << endl;
  cout << "Address:\t" << person.address << endl;
  cout << "\n";
}

int isNumeric(char input[]){
  for(int i = 0; i < strlen(input); i++){
    if( (int)input[i] < 48 || (int)input[i] > 57 ){

      return 0;
    }
  }
  return 1;
}

int stringCompare(char value[], char key[]){
  for(int i = 0; i < strlen(key); i++){
    if( value[i] != key[i] ){

      return 0;
    }
  }
  return 1;
}

int compare(Person person, int type, char input[], bool gender){
  switch (type) {
    case 1:
      if( stringCompare(person.name, input) ){

        return 1;
      }
      break;
    case 2:
      if( stringCompare(person.major, input)){

        return 1;
      }
      break;
    case 3:
      if( stringCompare(person.address, input) ){

        return 1;
      }
      break;
    case 4:
      if( stringCompare(person.id, input) ){

        return 1;
      }
      break;
    case 5:
      if( stringCompare(person.number, input) ){

        return 1;
      }
      break;
    case 6:
      if( person.gender == gender ){

        return 1;
      }
    case 7:
      if( stringCompare(person.age, input) ){

        return 1;
      }
  }
  return 0;
}

int countAll(){
  int count = 0;
  Person person;
  FILE *f = fopen(DATABASE, "ab+");
  while(fread(&person, sizeof(struct Person), 1, f)){
    count++;
  }
  fclose(f);
  return count;
}

int countSearch(int type, char input[], bool gender){
  int count = 0;
  Person person;
  FILE *f = fopen(DATABASE, "ab+");
  while(fread(&person, sizeof(struct Person), 1, f)){
    if( compare(person, type, input, gender) ){

      count++;
    }
  }
  fclose(f);
  return count;
}

void listContacts(){
  int count = 0;
  int allCount = countAll();
  Person person;
  FILE *f = fopen(DATABASE, "ab+");
  if( allCount ){

    while(fread(&person, sizeof(struct Person), 1, f)){
      count++;
      cout << "\t\t\tThe Following Users are found. " << allCount - count << " users left" << endl << endl;
      printPerson(person, count);
      if(allCount - count){

        cout << "Press any key to see next user...";
      }else{
        cout << "Press any key to go back to menu...";
      }
      getch();
      system("cls");
    }
  }else{
    cout << "\t\t\tNo User to show. Please add some to database.\n\n";
    cout << "Press any key to go back to menu...";
    getch();
  }
  fclose(f);
}

void searchContact(int type, char input[], bool gender = true){
  int count = 0;
  int allCount = countSearch(type, input, gender);
  bool found = false;
  Person person;
  FILE *f = fopen(DATABASE, "ab+");
  while(fread(&person, sizeof(struct Person), 1, f)){
    if( compare(person, type, input, gender) ){
      count++;
      cout << "\t\t\tThe Following Users are found. " << allCount - count << " users left" << endl << endl;
      found = true;
      printPerson(person, count);
      if(allCount - count){

        cout << "Press any key to see next user...";
      }else{
        cout << "Press any key to go back to menu...";
      }
      getch();
      system("cls");
    }
  }
  fclose(f);
  if( !found ){

    cout << "No User is found\n\n";
    cout << "Press any key to go back to menu...";
    getch();
  }
}

int isExists(char id[]){
  bool found = false;
  int index = 0;
  Person person;
  FILE *f = fopen(DATABASE, "ab+");
  while(fread(&person, sizeof(struct Person), 1, f) && !found){
    index++;
    if( !found && strcmp(person.id, id) == 0 ){
      found = true;
    }
  }
  fclose(f);
  if( found ){

    return index;
  }else{
    return 0;
  }
}

void deleteContact(int type, char input[], bool gender = true){
  int count = 0;
  bool found = false;
  Person person;
  FILE *f = fopen(DATABASE, "ab+");
  FILE *temp = fopen("temp.txt", "ab+");
  while(fread(&person, sizeof(struct Person), 1, f)){
    if( compare(person, type, input, gender) ){

      found = true;
      count++;
    }else{
      fwrite(&person, sizeof(struct Person), 1, temp);
    }
  }
  fclose(f);
  fclose(temp);
  remove(DATABASE);
  rename("temp.txt", DATABASE);
  cout << "\t\t\tDelete Contact\n\n";
  if( !found ){

    cout << "No User is found\n\n";
  }else{
    cout << "User deleted successfully! " << count << " record(s) is affected\n\n";
  }
  cout << "Press any key to go back to menu...";
  getch();
}

void saveAfter(Person input, char id[]){
  if( isExists(id) ){

    bool done = false;
    int total = countAll();
    int index = isExists(id);
    FILE *f = fopen(DATABASE, "ab+");
    if( total == index ){

      if( fwrite(&input, sizeof(struct Person), 1, f) ){

        cout << "Contact is successfully added to database\n\n";
      }else{
        cout << "Failed to add contact to database\n\n";
      }
      fclose(f);
    }else{
      int added;
      FILE *temp = fopen("temp.txt", "ab+");
      Person person;
      while(fread(&person, sizeof(struct Person), 1, f)){
        if( strcmp(person.id, id) && !done){

          done = true;
          added = fwrite(&input, sizeof(struct Person), 1, temp);
        }
        fwrite(&person, sizeof(struct Person), 1, temp);
      }
      fclose(f);
      fclose(temp);
      if( added && remove(DATABASE) && rename("temp.txt", DATABASE) ){

        cout << "Contact is successfully added to database\n\n";
      }else{
        cout << "Failed to add contact to database\n\n";
      }
    }
  }else{
    cout << "User is not found\n\n";
  }
}

void showMenu(){
  system("cls");
  cout << "\t\t\t Welcome to Omiid's PhoneBook! Select what you want to do:\n";
  cout << "1. Add Contact\n";
  cout << "2. Search Contact\n";
  cout << "3. List Contacts\n";
  cout << "4. Delete Contact\n";
  cout << "5. Restore/Save Backup\n";
  cout << "6. Exit\n\n";
  cout << "Enter What you want to do: ";
}

void secondPage(int type){
  system("cls");
  char id[MAX3];
  if( type == 1 ){

    cout << "\t\t\tChoose which field you want to search over:\n\n";
  }else if( type == 2 ){
    cout << "\t\t\tChoose which field you want to delete over:\n\n";
  }
  cout << "\t\t\t1. Name\t\t2. Major\t3. Address\n";
  cout << "\t\t\t4. ID\t\t5. Number\t6. Gender\n";
  cout << "\t\t\t7. Age\n";
  cout << "Enter Field: ";
  char fields[100000];
  gets(fields);
  while( strlen(fields) == 0 || fields[0] < '0' || fields[0] > '7' ){


    system("cls");
    if( type == 1 ){

      cout << "\t\t\tChoose which field you want to search over:\n\n";
    }else if( type == 2 ){
      cout << "\t\t\tChoose which field you want to delete over:\n\n";
    }
    cout << "\t\t\t1. Name\t\t2. Major\t3. Address\n";
    cout << "\t\t\t4. ID\t\t5. Number\t6. Gender\n";
    cout << "\t\t\t7. Age\n";
    cout << "Enter Field: ";
    gets(fields);
  }
  system("cls");
  char input[MAX1];
  char input2[MAX2];
  char input3[MAX3];
  char input4[MAX4];
  char input5[MAX5];
  bool gender = false;
  int check;
  int field = (int)fields[0] - 48;
  switch (field) {
    case 1:
      cout << "Enter Name: ";
      gets(input);
      while( strlen(input) > MAX1 - 1 ){

        memset(input, 0, sizeof input);
        system("cls");
        cout << "\t\t\tMaximum character exceeded.\n";
        cout << "Enter Name: ";
        gets(input);
      }
      system("cls");
      if( type == 1 ){

        searchContact(1, input);
      }else if( type == 2){
        deleteContact(1, input);
      }
      break;

    case 2:
      cout << "Enter Major: ";
      gets(input);
      while( strlen(input) > MAX1 - 1 ){

        memset(input, 0, sizeof input);
        system("cls");
        cout << "\t\t\tMaximum character exceeded.\n";
        cout << "Enter Major: ";
        gets(input);
      }
      system("cls");
      if( type == 1 ){

        searchContact(2, input);
      }else if( type == 2){
        deleteContact(2, input);
      }
      break;

    case 3:
      cout << "Enter Address: ";
      gets(input2);
      while( strlen(input2) > MAX2 - 1 ){

        memset(input2, 0, sizeof input2);
        system("cls");
        cout << "\t\t\tMaximum character exceeded.\n";
        cout << "Enter Address: ";
        gets(input2);
      }
      system("cls");
      if( type == 1 ){

        searchContact(3, input2);
      }else if( type == 2){
        deleteContact(3, input2);
      }
      break;

    case 4:
      cout << "Enter ID: ";
      gets(input3);
      while( strlen(input3) > MAX3 - 1 ){

        memset(input3, 0, sizeof input3);
        system("cls");
        cout << "\t\t\tMaximum character exceeded.\n";
        cout << "Enter ID: ";
        gets(input3);
      }
      system("cls");
      if( type == 1 ){

        searchContact(4, input3);
      }else if( type == 2){
        deleteContact(4, input3);
      }
      break;

    case 5:
      cout << "Enter Number: ";
      gets(input4);
      while( strlen(input4) > MAX4 - 1 ){

        memset(input4, 0, sizeof input4);
        system("cls");
        cout << "\t\t\tMaximum character exceeded.\n";
        cout << "Enter Number: ";
        gets(input4);
      }
      while( !isNumeric(input4) ){
        memset(input4, 0, sizeof input4);
        system("cls");
        cout << "\t\t\tBad input is given.\n";
        cout << "Enter Number: ";
        gets(input4);
      }
      system("cls");
      if( type == 1 ){

        searchContact(5, input4);
      }else if( type == 2){
        deleteContact(5, input4);
      }
      break;

    case 6:
      cout << "Enter Gender:(M: Male, F: Female) ";
      char ch;
      cin >> ch;
      check = ch == 'm' || ch == 'M' || ch == 'f' || ch == 'F';
      while( !check ){

        system("cls");
        cout << "\t\t\tBad Input was given.\n";
        cout << "Enter Gender:(M: Male, F: Female) ";
        cin >> ch;
        check = ch == 'm' || ch == 'M' || ch == 'f' || ch == 'F';
      }
      if( ch == 'm' || ch == 'M' ){

        gender = true;
      }
      system("cls");
      if( type == 1 ){

        searchContact(6, input, gender);
      }else if( type == 2){
        deleteContact(6, input, gender);
      }
      break;

    case 7:
      cout << "Enter Age: ";
      gets(input5);
      while( strlen(input5) > MAX5 - 1 || input5[0] < '0' || !isNumeric(input5) ){

        memset(input5, 0, sizeof input5);
        system("cls");
        cout << "\t\t\tBad Input was given.\n";
        cout << "Enter Age: ";
        gets(input5);
      }
      system("cls");
      if( type == 1 ){

        searchContact(7, input5);
      }else if( type == 2){
        deleteContact(7, input5);
      }
      break;
  }
}

int main(){
  bool flag = true;
  while(flag){
    char in[10000];
    int function;
    showMenu();
    gets(in);

    if( cin.fail() ){

      cin.clear();
      cin.ignore();
      showMenu();
    }else if( in[1] == '\0' && (int)in[0] < 55 && (int)in[0] > 48 ){// one to six functions
      function = (int)in[0] - 48;
      if( function == 1 ){

        Person person;
        system("cls");
        cout << "\t\t\tAdd New Contact\n";
        cout << "Enter ID: ";
        gets(person.id);
        while( strlen(person.id) > MAX3 - 1){
          system("cls");
          cout << "\t\t\tAdd New Contact\n";
          cout << "\t\t\tBad Input is given.\n";
          cout << "Enter ID: ";
          gets(person.id);
        }
        while( strlen(person.id) == 0 ){
          system("cls");
          cout << "\t\t\tAdd New Contact\n";
          cout << "\t\t\tBad Input is given.\n";
          cout << "Enter ID: ";
          gets(person.id);
        }
        int found = isExists(person.id);
        system("cls");
        cout << "\t\t\tAdd New Contact\n";
        cout << "Enter Name: ";
        gets(person.name);
        while( strlen(person.name) > MAX1 - 1 ){
          system("cls");
          cout << "\t\t\tAdd New Contact\n";
          cout << "\t\t\tBad Input is given.\n";
          cout << "Enter Name: ";
          gets(person.name);
        }
        system("cls");
        cout << "\t\t\tAdd New Contact\n";
        cout << "Enter Major: ";
        gets(person.major);
        while( strlen(person.major) > MAX1 - 1 ){
          system("cls");
          cout << "\t\t\tAdd New Contact\n";
          cout << "\t\t\tBad Input is given.\n";
          cout << "Enter Major: ";
          gets(person.major);
        }
        system("cls");
        cout << "\t\t\tAdd New Contact\n";
        cout << "Enter Number: ";
        gets(person.number);
        while( strlen(person.number) > MAX4 - 1 || !isNumeric(person.number) ){
          system("cls");
          cout << "\t\t\tAdd New Contact\n";
          cout << "\t\t\tBad Input is given.\n";
          cout << "Enter Number: ";
          gets(person.number);
        }
        person.gender = false;
        system("cls");
        cout << "\t\t\tAdd New Contact\n";
        cout << "Enter Gender:(M: Male, F: Female) ";
        char ch;
        cin >> ch;
        int check = ch == 'm' || ch == 'M' || ch == 'f' || ch == 'F';
        while( !check ){
          system("cls");
          cout << "\t\t\tAdd New Contact\n";
          cout << "\t\t\tBad Input is given.\n";
          cout << "Enter Gender:(M: Male, F: Female) ";
          cin.clear();
          cin.ignore();
          cin >> ch;
          check = ch == 'm' || ch == 'M' || ch == 'f' || ch == 'F';
        }
        if( ch == 'm' || ch == 'M' ){
          person.gender = true;
        }
        system("cls");
        cout << "\t\t\tAdd New Contact\n";
        cout << "Enter Age: ";
        cin.ignore();
        gets(person.age);
        while( strlen(person.age) > MAX5 - 1 || !isNumeric(person.age) ){
          system("cls");
          cout << "\t\t\tAdd New Contact\n";
          cout << "\t\t\tBad Input is given.\n";
          cout << "Enter Age: ";
          gets(person.age);
        }
        system("cls");
        cout << "\t\t\tAdd New Contact\n";
        cout << "Enter Address: ";
        gets(person.address);
        while( strlen(person.address) > MAX2 - 1){
          cout << "\t\t\tAdd New Contact\n";
          cout << "\t\t\tBad Input is given.\n";
          cout << "Enter Address: ";
          cin.clear();
          cin.ignore();
          gets(person.address);
        }
        system("cls");
        cout << "\t\t\tAdd New Contact\n";
        cout << "\t\t\tContact Details\n";
        printPerson(person, 0);
        char add;
        cout << "Do You want to add contact?(Y: YES, N: NO) ";
        cin >> add;
        if( add == 'y' || add == 'Y' ){


          if( found ){

            system("cls");
            cout << "\t\t\tAdd New Contact\n";
            FILE *f = fopen(DATABASE, "ab+");
            if( fwrite(&person, sizeof(struct Person), 1, f) ){

              cout << "Contact is successfully added to database\n\n";
            }else{
              cout << "Failed to add contact to database\n\n";
            }
            cout << "Press any key to go back to menu...";
            getch();
            fclose(f);
          }else{
            cout << "Where do you want to place the new contact?\n";
            cout << "Enter ID:(0 to place at the end) ";
            char placeID[MAX3];
            cin.ignore();
            gets(placeID);
            int found = isExists(placeID);

            while( strlen(placeID) > MAX3 - 1 || ( strlen(placeID) > 1 && !found ) ){
              system("cls");
              cout << placeID << ":" <<found << endl;
              cout << "\t\t\tAdd New Contact\n";
              cout << "\t\t\tContact Details\n";
              printPerson(person, 0);
              cout << "Where do you want to place the new contact?\n";
              cout << "Bad Input was given\n";
              cout << "Enter ID:(0 to place at the end) ";
              gets(placeID);
              found = isExists(placeID);
            }
            if( (strlen(placeID) == 1 && placeID[0] == '0') || strlen(placeID) == 0 ){

              system("cls");
              cout << "\t\t\tAdd New Contact\n";
              FILE *f = fopen(DATABASE, "ab+");
              if( fwrite(&person, sizeof(struct Person), 1, f) ){

                cout << "Contact is successfully added to database\n\n";
              }else{
                cout << "Failed to add contact to database\n\n";
              }
              cout << "Press any key to go back to menu...";
              getch();
              fclose(f);
            }else{
              system("cls");
              cout << "\t\t\tAdd New Contact\n";
              saveAfter(person, placeID);
              cout << "Press any key to go back to menu...";
              getch();

            }
          }
        }
      }else if( function == 2 ){

        secondPage(1);
      }else if( function == 3 ){

        system("cls");
        listContacts();
      }else if( function == 4 ){

        secondPage(2);
      }else if( function == 5 ){

        system("cls");
        cout << "\t\t\tWhat Do you want to do?\n";
        cout << "\t\t\t1. Restore\t2. Save\n";
        cout << "Enter action: ";
        char action[2];
        gets(action);
        while( action[0] < 49 || action[0] > 50 ){
          system("cls");
          cout << "\t\t\tWhat Do you want to do?\n";
          cout << "\t\t\t1. Restore\t2. Save\n";
          cout << "Enter action: ";
          char action[2];
          gets(action);
        }
        system("cls");
        if( action[0] == 49 ){

          cout << "\t\t\tRestore\n";
        }else if( action[0] == 50 ){

          cout << "\t\t\tSave\n";
        }
        cout << "Enter Path: ";
        char path[10000];
        gets(path);
        FILE *output = fopen(path, "ab+");
        FILE *source = fopen(DATABASE, "ab+");
        if( action[0] == 49 && output ){
          Person person;
          if ( fread(&person, sizeof(struct Person), 1, output) ){

            rewind(output);
            system("cls");
            struct stat filestat;
            stat(path, &filestat);
            struct tm tm = *localtime(&filestat.st_mtime);
            cout << "File was last modified on: " << tm.tm_mon + 1 << "/" << tm.tm_mday << "/" << tm.tm_year + 1900 << " " << tm.tm_hour << ":" << tm.tm_min << endl;
            cout << "Do want to restore?(Y: Yes, N: No) ";
            char con[2];
            gets(con);
            if( con[0] == 'Y' || con[0] == 'y' ){

              char a = fgetc(source);
              rewind(source);
              if( source && a != EOF ){

                system("cls");
                struct stat filestat;
                stat(DATABASE, &filestat);
                struct tm tm = *localtime(&filestat.st_mtime);
                cout << "\tWhat do you want to do with current databse?\n";
                cout << "It was last modified on: " << tm.tm_mon + 1 << "/" << tm.tm_mday << "/" << tm.tm_year + 1900 << " " << tm.tm_hour << ":" << tm.tm_min << endl;
                cout << "Overwrite?(Y: Yes, N: No) ";
                gets(con);
                if( con[0] == 'Y' || con[0] == 'y' ){

                  fclose(source);
                  remove(DATABASE);
                  FILE *source = fopen(DATABASE, "ab+");
                  Person person;
                  while (fread(&person, sizeof(struct Person), 1, output)){
                    fwrite(&person, sizeof(struct Person), 1, source);
                  }
                  system("cls");
                  cout << "Restored Successfully!\n\n";
                  cout << "Press any key to go back to menu...";
                  getch();
                }
              }else{
                fclose(source);
                remove(DATABASE);
                FILE *source = fopen(DATABASE, "ab+");
                Person person;
                while (fread(&person, sizeof(struct Person), 1, output)){
                  fwrite(&person, sizeof(struct Person), 1, source);
                }
                system("cls");
                cout << "Restored Successfully!\n\n";
                cout << "Press any key to go back to menu...";
                getch();
              }
            }
          }else{
            system("cls");
            cout << "File is not a database!\n\n";
            cout << "Press any key to go back to menu...";
            getch();
          }
        }else if( action[0] == 50 ){
          fclose(output);
          remove(path);
          FILE *output = fopen(path, "ab+");
          int i = 1;
          Person person;
          while (fread(&person, sizeof(struct Person), 1, source)){
            fwrite(&person, sizeof(struct Person), 1, output);
          }
          system("cls");
          cout << i;
          cout << "Saved Successfully!\n\n";
          cout << "Press any key to go back to menu...";
          getch();
        }else{
          system("cls");
          cout << "\tFile is not found. Try again.\n";
          cout << "Press any key to go back to menu...";
          getch();
        }
        fclose(source);
        fclose(output);
      }else if( function == 6 ){

        flag = false;
        system("cls");
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        cout << "\t\t\t\tFundamentals of Programming Final Project\n";//41
        cout << "\t\t\t\t         Professor: Dr.Nazerfard\n";//23
        cout << "\t\t\t\t    Teacher Assistant: Y.MirMohammad\n";//32
        cout << "\t\t\t\t        Developed By: Omid Seyfan\n";//25
        cout << "\t\t\t\t              Fall 2018-19\n";//12
        cout << "\t\t\t\t    Thank you for using my PhoneBook\n\n";//32
        cout << "Last Save on: " << tm.tm_mon + 1 << "/" << tm.tm_mday << "/" << tm.tm_year + 1900 << " " << tm.tm_hour << ":" << tm.tm_min << endl;
        cout << endl;
        cout << "Press any key to exit program...";
        getch();
      }
    }
  }
  return 0;
}
