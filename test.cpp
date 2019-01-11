#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <conio.h>
using namespace std;

struct Data{
  char name[20];
  char id[10];
};

struct Person{
  Data data;
  Person* next;
};

Person* head = NULL;
Person* last = NULL;

void printALL(){
  Person* course = head;
  while( course != NULL ){
    cout << "Me: " << course << endl;
    cout << "Name: " << course->data.name << endl;
    cout << "ID: " << course->data.id << endl;
    cout << "Next: " << course->next << endl;
    course = course->next;
    cout << endl;
  }
}

void saveToFile(){
  remove("database2.txt");
  FILE *f = fopen("database2.txt", "ab+");
  Person* temp = head;
  int i = 0;
  printALL();
  while( temp ){
    i++;
    fwrite(&(temp->data), sizeof(struct Data), 1, f);
    temp = temp->next;
  }
  cout << i;
  fclose(f);
}

void initFile(){
  Data data;
  FILE *f = fopen("database2.txt", "ab+");
  int i = 0;
  while( fread(&data, sizeof(struct Data), 1, f) ){
    Person* temp = (Person *)malloc(sizeof(Person));
    temp->next = NULL;
    i++;
    temp->data = data;
    if( head == NULL ){

      head = temp;
    }else if( head->next == NULL ){

      head->next = temp;
    }
    if( last != NULL ){
      last->next = temp;
    }
    last = temp;
  }
  cout << i;
  fclose(f);
}

void deletePerson(char id[]){
  bool deleted = false;
  Person* temp = head;
  Person* lastInDelete = head;
  while( temp ){
    if( deleted ){

      lastInDelete->next = temp;
      deleted = false;
    }
    if( strcmp(temp->data.id, id) == 0 ){

      if( temp == head ){

         head = temp->next;
      }else if( temp == last){
        last = lastInDelete;
        lastInDelete->next = NULL;
      }else{
        deleted = true;
      }
    }else{
      lastInDelete = temp;
    }
    temp = temp->next;
  }
}

void saveAfter(Person *input, char id[]){
  bool flag = true;
  Person* temp = head;
  while( flag && temp ){
    if( strcmp(temp->data.id, id) == 0 ){
      if( temp == last ){

        temp->next = input;
        last = input;
      }else{
        input->next = temp->next;
        temp->next = input;
      }
      flag = false;
    }
    temp = temp->next;
  }
  printALL();
  cout << "Head: " << head << endl;
  cout << "Last: " << last << endl;
  getch();
}

int main(){
  initFile();
  bool flag = true;
  char function[2];
  while(flag){
    system("cls");
    printALL();
    cout << "Enter Function:(a: add, d: delete, f: save after,) ";

    gets(function);
    if( function[0] == '*' ){

      flag = false;
      saveToFile();

    }else if( function[0] == 'a' ){
      char name[20];
      char id[10];
      cout << "Enter Name: ";
      gets(name);
      cout << "Enter ID: ";
      gets(id);
      Person* temp = (Person*)malloc(sizeof(Person));
      temp->next = NULL;
      strcpy(temp->data.name, name);
      strcpy(temp->data.id, id);
      if( head == NULL ){

        head = temp;
      }else if( head->next == NULL ){

        head->next = temp;
      }
      if( last != NULL ){
        last->next = temp;
      }
      last = temp;
    }else if( function[0] == 'd' ){
      char id[10];
      cout << "Enter ID: ";
      gets(id);
      deletePerson(id);
    }else if(function[0] == 'f' ){
      char name[20];
      char id[10];
      char after[10];
      cout << "Enter Name: ";
      gets(name);
      cout << "Enter ID: ";
      gets(id);
      Person* temp = (Person*)malloc(sizeof(Person));
      temp->next = NULL;
      strcpy(temp->data.name, name);
      strcpy(temp->data.id, id);
      cout << "Enter After ID: ";
      gets(after);
      saveAfter(temp, after);
    }
  }
  // printALL();
  return 0;
}
