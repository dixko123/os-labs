#include <stdio.h>
#include <string.h>

int cuentaEstudiantes=0;

struct STUDENT
{
	int ID;
	char name[50] ;
	int age;
} ;

struct STUDENT ArregloStudents[10];


void Add(int ID,char name[50], int age){
    struct STUDENT newStudent;
    newStudent.ID=ID;
    strcpy(newStudent.name,name);
    newStudent.age=age;
    
    ArregloStudents[cuentaEstudiantes++]=newStudent;
}


void List(void){
    for(int i=0;i<10;i++){
        printf("ID: %i name: %s; age: %i \n", ArregloStudents[i].ID,ArregloStudents[i].name, ArregloStudents[i].age);
    }
}
int main()
{
    Add(1229152,"Jose Antonio Lopez Cruz",22);
    Add(1226587,"Jorge Fernando Saldana",20);
    Add(1638945,"Alejandro Godoy Lee)",20);
    Add(1364558,"Obed munoz",27);
    
    
    List();

    return 0;
}





