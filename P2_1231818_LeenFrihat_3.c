
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Tree
{
    int ID;
    char Name[100];
    char Date[100];
    float Duration;
    char State[12];
    struct Tree* Left;
    struct Tree* Right;
};
typedef struct Tree* TNode;

TNode MakeEmpty(TNode T);
TNode loadTasks(TNode T);
TNode Insert (TNode T);
TNode InsertByID (TNode newTask, TNode T);
TNode InsertByName (TNode newTask, TNode T);
TNode InsertByDate ( TNode T, TNode newTask);
TNode InsertByDuration (TNode newTask, TNode T);
TNode DeleteTree(TNode T);
TNode toName(TNode T);
TNode roDate(TNode T);
TNode toDuration(TNode T);
TNode PerformTask(TNode T);
TNode FindMin(TNode T);
TNode SearchNameTree(TNode T, char chosen[100]);
TNode SearchIDTree(TNode T,int ID);
TNode taskList[100]; // here more than 50 since we were asked in the project
int index=0;
int mode=1;
/*
1 = ID
2 = NAME
3 = DATE
4= DURATION
*/

TNode MakeEmpty(TNode T)
{
    if (T!= NULL)
    {
        MakeEmpty (T-> Left);
        MakeEmpty(T->Right);
        free(T);
    }
    return NULL;
}

TNode loadTasks(TNode T)

{
    printf("Loading Tasks from File");
    printf("\n*************************************\n");
    FILE*file;
    file = fopen("tasks.txt", "r");
    if (!file)
    {
        printf("Error: Could not open tasks.txt\n");
        return T;
    }

    T = MakeEmpty(T);
    char line[300];
    index=0;
    while(fgets(line, sizeof(line), file)!=NULL)
    {
        TNode Task = (TNode)malloc(sizeof(struct Tree));
        if (Task==NULL)
        {
            printf("\nOut of memory!\n");
            return NULL;
        }

        Task->  ID = atoi(strtok(line,"#"));
        strcpy(Task->Name,strtok(NULL,"#"));
        strcpy(Task->Date,strtok(NULL,"#"));
        Task->Duration = atof(strtok(NULL,"#"));
        strcpy(Task->State,"unperformed");

        Task->Left = NULL;
        Task->Right = NULL;
        taskList[index] = Task;
        index++;
    }
    fclose(file);
    mode =1;
    for (int i=0; i<index; i++)
    {
        T= InsertByID(taskList[i],T);
    }
    viewUnperformed(T);//since all tasks will be unperformed at this point

    return T;
}

TNode Insert (TNode T)
{
    //first we make sure memory allocation works
    TNode newTask = (TNode)malloc(sizeof(struct Tree));
    if (newTask == NULL)
    {
        printf("our of space!");
        return NULL;
    }
    printf("\nPlease Enter the following info :\n");
    printf("Task ID: ");
    scanf("%d", &(newTask->ID));

    printf("\nTask Name: ");
    while (getchar() != '\n');
    fgets(newTask->Name, sizeof(newTask->Name), stdin);
    newTask->Name[strcspn(newTask->Name, "\n")] = '\0';

    printf("\nTask Date: ");
    scanf("%s", newTask->Date);
    printf("\nTask Duration: ");
    scanf("%f",&(newTask->Duration));

    strcpy(newTask->State,"unperformed");
    newTask->Left = NULL;
    newTask->Right = NULL;

    taskList[index] = newTask;
    index++;

    //here for efficiency, depending on your current mode we will add the task to the list
    switch(mode)
    {
    case 1: //ID
        T = InsertByID(newTask,T);
        break;
    case 2: //Name
        T = InsertByName(newTask,T);
        break;
    case 3://Date
        T = InsertByDate(T,newTask);
        break;
    case 4://Duration
        T = InsertByDuration(newTask,T);
        break;
    }
    printf("\nID: %d Name: '%s' Date: %s Duration: %f hours was added to your Tasks!\n", newTask->ID,newTask->Name,newTask->Date,newTask->Duration);

    return T;
}

TNode InsertByID (TNode newTask, TNode T)
{
    if (T == NULL)
    {
        T = newTask;
    }
    else if (newTask->ID < T->ID)
    {
        T->Left = InsertByID ( newTask, T->Left);
    }
    else if (newTask->ID > T->ID)
    {
        T->Right = InsertByID ( newTask, T->Right);
    }
    return T;

}



TNode InsertByName (TNode newTask, TNode T)
{
    if (T == NULL)
    {
        T = newTask;
    }
    int result = strcmp(newTask->Name,T->Name);
    if (result<0)
    {
        T->Left = InsertByName ( newTask, T->Left);
    }
    else if (result>0)
    {
        T->Right = InsertByName ( newTask, T->Right);
    }
    return T;
}

TNode InsertByDate(TNode T, TNode newTask)
{
    if (T == NULL)
    {
        return newTask;
    }

    int day1, month1, year1; // for the current task
    int day2, month2, year2; //for the new task

    char date1[11];
    strcpy(date1, T->Date);
    char* token = strtok(date1, "/");
    day1 = atoi(token);
    token = strtok(NULL, "/");
    month1 = atoi(token);
    token = strtok(NULL, "/");
    year1 = atoi(token);

    char date2[11];
    strcpy(date2, newTask->Date);
    token = strtok(date2, "/");
    day2 = atoi(token);
    token = strtok(NULL, "/");
    month2 = atoi(token);
    token = strtok(NULL, "/");
    year2 = atoi(token);


    if(year1 > year2)
    {
        T->Left = InsertByDate(T->Left, newTask);
    }
    else if (year1 < year2)
    {
        T->Right = InsertByDate(T->Right, newTask);
    }
    else
    {
        if (month1 > month2)
        {
            T->Left = InsertByDate(T->Left, newTask);
        }
        else if (month1 < month2)
        {
            T->Right = InsertByDate(T->Right, newTask);
        }
        else
        {
            if (day1 > day2)
            {
                T->Left = InsertByDate(T->Left, newTask);
            }
            else if (day1 < day2)
            {
                T->Right = InsertByDate(T->Right, newTask);
            }
        }
    }
    return T;
}

TNode InsertByDuration (TNode newTask, TNode T)
{
    if (T == NULL)
    {
        T = newTask;
    }
    else if (newTask->Duration < T->Duration)
    {
        T->Left = InsertByDuration ( newTask, T->Left);
    }
    else if (newTask->Duration > T->Duration)
    {
        T->Right = InsertByDuration ( newTask, T->Right);
    }
    return T;

}

TNode DeleteTree(TNode T)
{
    //the difference from MakeEmpty is that here we don't free the memory from the nodes, so i can still
    //access them in my array

    if (T == NULL)
    {
        return NULL;
    }


    DeleteTree(T->Left);
    T->Left = NULL;
    DeleteTree(T->Right);
    T->Right = NULL;
    return NULL;
}


TNode toID (TNode T)
{
    T = DeleteTree(T);
    for (int i = 0; i<index; i++)
    {
        T = InsertByID(taskList[i],T);

    }
    mode = 1;
    return T;
}
TNode toName (TNode T)
{
    T = DeleteTree(T);
    for (int i = 0; i<index; i++)
    {
        T = InsertByName(taskList[i],T);
    }
    mode = 2;
    return T;
}
TNode toDate (TNode T)
{
    T = DeleteTree(T);
    for (int i = 0; i<index; i++)
    {
        T = InsertByDate(T,taskList[i]);

    }
    mode = 3;
    return T;
}

TNode toDuration (TNode T)
{
    T = DeleteTree(T);
    for (int i = 0; i<index; i++)
    {
        T = InsertByDuration(taskList[i],T);

    }
    mode = 4;
    return T;
}
TNode FindMin( TNode T )
{

    if( T != NULL)
    {

        while( T->Left != NULL)
        {

            T = T->Left;
        }
    }
    return T;

}
TNode DeleteTask(TNode T, int ID)
{
    if (T == NULL)
    {
        printf("Task doesn't exist!\n");
        return NULL; // Return NULL if task doesn't exist
    }

    TNode temp;
    if (ID < T->ID)
    {
        T->Left = DeleteTask(T->Left, ID);
    }
    else if (ID > T->ID)
    {
        T->Right = DeleteTask(T->Right, ID);
    }
    else
    {
        if (T->Left && T->Right)
        {
            temp = FindMin(T->Right);
            T->ID = temp->ID;
            T->Right = DeleteTask(T->Right, temp->ID);
        }
        else
        {
            temp = T;
            if (T->Left == NULL)
            {
                T = T->Right;
            }
            else if (T->Right == NULL)
            {
                T = T->Left;
            }
            free(temp);
        }
    }
    return T;
}

TNode DeleteOP(TNode T)
{
    printf("Do you want to delete by 1- ID or 2- Name?\n");
    int choice, id;
    char name[100];
    TNode chosen = NULL;
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        printf("Enter ID: ");
        scanf("%d", &id);
        chosen = SearchIDTree(T, id);
        break;
    case 2:
        printf("Enter Name: ");
        while (getchar() != '\n'); // Clear buffer
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0'; // Remove newline character
        chosen = SearchNameTree(T, name);
        break;
    default:
        printf("\nINVALID CHOICE\n");
        return T;
    }

    if (chosen == NULL)
    {
        printf("Task not found!\n");
        return T;
    }
    T = DeleteTask(T, chosen->ID);

    for (int i = 0; i < index; i++)
    {
        if (taskList[i] == chosen)
        {
            // shift
            for (int j = i; j < index - 1; j++)
            {
                taskList[j] = taskList[j + 1];
            }
            index--;
            break;
        }
    }

    return T;
}
TNode SearchNameTree(TNode T, char chosen[100])
{
    if (T == NULL)
    {
        return NULL;
    }
    //here for efficiency, if the tree was already ordered by name, do binary Search
    //but if it was in any other mode, search node by node since restructuring and BST will take way more time
    if (mode ==2)
    {
        int result = strcmp(T->Name, chosen);
        if (result > 0)
        {
            return SearchNameTree(T->Left, chosen);
        }
        else if (result < 0)
        {
            return SearchNameTree(T->Right, chosen);
        }
        else
        {
            return T;
        }
    }
    else //mode is 1,3 or 4
    {
        if (strcmp(T->Name,chosen)==0)
        {
            return T;
        }
        else
        {
            //search all the left side then go right
            TNode result= SearchNameTree(T->Left,chosen);
            if (result == NULL)
            {
                result= SearchNameTree(T->Right,chosen);
            }
            return result;

        }
    }
}

TNode SearchIDTree(TNode T,int ID)
{

    if (T == NULL)
    {
        return NULL;
    }

    //same as SearchNameTree(), here for efficiency, if the tree was already ordered by ID, do binary Search
    //but if it was in any other mode, search node by node since restructuring and BST will take way more time
    if (mode ==1)
    {
        if (ID< T->ID )
        {
            return SearchIDTree(T->Left, ID);
        }
        else if (ID> T->ID)
        {
            return SearchIDTree(T->Right, ID);
        }
        else
        {
            return T;
        }
    }
    else
    {
        if (T->ID == ID)
        {
            return T;
        }
        else
        {
            TNode result= SearchIDTree(T->Left,ID);
            if (result == NULL)
            {
                result= SearchIDTree(T->Right,ID);
            }
            return result;

        }
    }
}
TNode PerformTask(TNode T)
{
    TNode chosenTask= NULL;
    printf("\n How do you want to perform? \n"
           "1 - by ID\n"
           "2 - by Name\n"
           "Enter : "
          );
    int choice,id;
    char name[100];
    scanf("%d",&choice);
    switch (choice)
    {
    case 1:
        printf("\n Enter ID : ");
        scanf("%d",&id);
        chosenTask=SearchIDTree(T,id);
        break;
    case 2:
        printf("\n Enter name: ");
        while (getchar() != '\n');
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';

        chosenTask = SearchNameTree(T,name);
        break;
    default:
        printf("Invalid choice! \n");
        break;
    }

    return chosenTask;
}


void viewUnperformed(TNode T)
{
    if (T==NULL)
    {
        return;
    }
    viewUnperformed(T->Left);
    if (strcmp(T->State,"unperformed")==0)
    {
        printf("\nID: %d\n", T->ID);
        printf("Name: %s\n", T->Name);
        printf("Date: %s\n", T->Date);
        printf("Duration: %f hours\n", T->Duration);

    }
    viewUnperformed(T->Right);
}

void viewPerformed(TNode T)
{
    if (T==NULL)
    {
        return;
    }
    viewPerformed(T->Left);
    if (strcmp(T->State,"performed")==0)
    {
        printf("ID: %d Name: %s Date: %s Duration: %f hours \n", T->ID,T->Name,T->Date,T->Duration);
    }
    viewPerformed(T->Right);
}
void printTree(TNode T)
{
    if (T==NULL)
    {
        return;
    }
    printTree(T->Left);

    printf("ID: %d Name: %s Date: %s Duration: %f hours \n", T->ID,T->Name,T->Date,T->Duration);

    printTree(T->Right);
}
TNode Restructure(TNode T)
{
    printf("How do you want to Re-Order your list?\n"
           "1- By Name\n"
           "2- By Date\n"
           "3- By Duration\n"
           "Enter the Number of your choice: "
          );
    int choice =0;
    scanf("%d",&choice);
    switch(choice)
    {
    case 1:
        return toName(T);
        break;
    case 2:
        return toDate(T);
        break;
    case 3:
        return toDuration(T);
        break;
    default:
        printf("\ninvalid choice!\n");
        return T;
    }
}

int Height(TNode T)
{
    if (T== NULL)
        return 0;
    int left = Height(T->Left);
    int right = Height(T->Right);
    int max=0;
    if (left>right)
    {
        max= left;
    }
    else
    {
        max = right;
    }
    return max+1;
}

int leaves(TNode T)
{
    if (T == NULL)
    {
        return 0;
    }
    if(T->Left == NULL && T->Right == NULL)
    {
        return 1;
    }
    int left = leaves(T->Left);
    int right = leaves(T->Right);
    return left+right;
}
int internalNodes(TNode T)
{
    if (T == NULL)
    {
        return 0;
    }
    if(T->Left != NULL || T->Right != NULL)
    {
        return 1 + internalNodes(T->Left) +internalNodes(T->Right);;
    }
    else
    {
        return 0;
    }
}

void Menu()
{
    printf("\n*************************************\n");
    printf("        Task Management System       \n");
    printf("\n*************************************\n");
    printf("1- Load Tasks from File\n");
    printf("2- Add a New Task\n");
    printf("3- Delete a Task\n");
    printf("4- Search for a Task(by name)\n");
    printf("5- Perform a Task \n");
    printf("6- View Unperformed Tasks (ordered by name) \n");
    printf("7- View Performed Tasks\n");
    printf("8- Restructure the Tasks \n");
    printf("9- View the Tasks BST Info\n");
    printf("10- Exit\n");
}
int main()
{
    TNode T = NULL;
    TNode Task = NULL;
    char name[100];
    int op=0;
    while (op!=10)
    {
        Menu();
        printf(" Enter the number of the Operation: ");
        scanf("%d",&op);
        printf("\n*************************************\n");
        switch(op)
        {
        case 1:
            T= loadTasks(T);
            break;
        case 2:
            T = Insert(T);
            break;
        case 3:
            T = DeleteOP(T);
            break;
        case 4:
            printf("\n %d What is the name of Task you're searching for?\n",mode);
            scanf("%s",name);
            Task =SearchNameTree(T,name);
            if (Task==NULL)
            {
                printf("not found!\n");
            }
            else
            {
                printf("Task name %s was found",Task->Name);
            }
            break;
        case 5:
            Task = PerformTask(T);
            if (Task!= NULL)
            {
                strcpy(Task->State,"performed");
                printf("Task ID: %d Name: %s Has been performed! Great Job!",Task->ID,Task->Name);
            }
            else
            {
                printf("task doesn't exist!\n");
            }

            break;
        case 6:
            T = toName(T);
            viewUnperformed(T);
            break;
        case 7:
            T = toID(T);
            viewPerformed(T);
            break;
        case 8:
            T = Restructure(T);
            printTree(T);
            break;
        case 9:
            printf("\n BST INTO \n"
                   "Height: %d \nSize: %d \nLeaves %d\n"
                   "Internal nodes: %d\n",Height(T),(leaves(T)+internalNodes(T)),leaves(T),internalNodes(T));
            break;
        case 10 :
            printf("exit program");
            return 0;
        default :
            printf("\n INVALID CHOICE ! \n");

        }
    }


    return 0;
}

