
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct children{
    struct Member *child;
    struct children *nxt;
};

struct Member{
    int id;
    int parent_id;
    char Name[50];
    char gender;
    struct children *first_child;    // linked-list for children of a node
    int level;
}*root=NULL;

struct queue{
    struct Member *member;
    struct queue *nxt;
}*head=NULL;

int auto_id=1;      //private value should secured

// FILE *fptr;
// fprintf(fptr,"id,name\n");

void enqueue(struct Member *node){
    struct  queue *temp;
    if(head==NULL){
        head = (struct queue*)calloc(1,sizeof(struct queue));
        head->nxt=NULL;
        head->member = node;
    }
    else{
        temp = head;
        while(temp->nxt!=NULL){
            temp = temp->nxt;
        }
        temp->nxt = (struct queue*)calloc(1,sizeof(struct queue));
        temp->nxt->member = node;
        temp->nxt->nxt = NULL;
    }
}

void dequeue(){
    if(head!=NULL){
        struct queue *temp = head;
        head = temp->nxt;
        free(temp);
    }
}

void emptyqueue(){
    while(head!=NULL){
        dequeue();
    }
}

//To get the member address thourgh thier IDs
struct Member *FetchMemberWithId(int id){
    //if parent_id exists retrun its address, otherwise return 0 to signify parent doesn't exists
    // if(head==NULL){
        // struct Member *temp = root;
        enqueue(root);
        // if(root->id==parent_id){
            //     return root;
            // }
            // return NULL;
            struct queue *temp = head;
            while(temp!=NULL){
                if(temp->member->id==id){       //
                    // dequeue();
                    emptyqueue();
                    return temp->member;
                }
                else{
                    struct children *templ = temp->member->first_child;
                    while(templ!=NULL){
                        enqueue(templ->child);
                        templ = templ->nxt;
                    }
                    dequeue();
                }
                temp = temp->nxt;
            }
            // }
    // else{
        // }
        
    }
    
// To insert a member
void InsertMember(int parent_id){
        struct Member *member = (struct Member*)calloc(1,sizeof(struct Member));
        struct children *temp;
        // int lvl=1;
        member->id = auto_id;
        // printf("%d\n",auto_id);
        auto_id+=1;
        printf("enter child name: ");
        scanf("%s",&member->Name);
        printf("enter gender: ");
        scanf("%s",&member->gender);
        member->first_child=NULL;
        member->level = 1;
        FILE *fptr;
        if(root!=NULL){
            struct Member *parent = FetchMemberWithId(parent_id);
            // printf("root NOT NULL\n");
            // printf("parent->(%d,%s)\n",parent->id,parent->Name);     // To know the parent of the current inserted child
            
            if(parent->first_child==NULL){
                parent->first_child = (struct children*)calloc(1,sizeof(struct children));
                parent->first_child->child = member;
                parent->first_child->nxt=NULL;
                member->level = parent->level + 1;
            }
            else{
                temp = parent->first_child;
                while(temp->nxt!=NULL){
                    temp=temp->nxt;
                }
                temp->nxt = (struct children*)calloc(1,sizeof(struct children));
                temp->nxt->child = member;
                temp->nxt->nxt=NULL;
                member->level = parent->level + 1;
            }
            member->parent_id = parent->id;
            fptr = fopen("metadata.csv","a");
            fprintf(fptr,"%d,%s,%c,%d\n",member->id,member->Name,member->gender,member->parent_id);
            fclose(fptr);
            // printf("member level: %d\n",member->level);
        }
        else{
            fptr = fopen("metadata.csv","w");
            fprintf(fptr,"id,name,gender,parent_id\n");
            // printf("root NULL\nmember level: %d\n",member->level);
            member->parent_id = 0;
            root = member;
            fprintf(fptr,"%d,%s,%c,%d\n",member->id,member->Name,member->gender,member->parent_id);
            fclose(fptr);
        }
}

// To get the id by their names
int LocateId(char target_name[]){

    FILE *file = fopen("metadata.csv", "r");
    char name[100];
    int id;
    // printf("\n%s\n",target_name);
    // char target_name[50] = "Sanjay";

    if (!file) {
        printf("Error opening file.\n");
        // return;
    }

    fscanf(file, "%*[^\n]\n"); // Skip header

    while (fscanf(file, "%d,%[^,],%*[^,],%*d%*[\r\n]", &id, name) == 2) {
        if (strcmp(name, target_name) == 0) {
            // printf("Found: ID = %d, Name = %s, Gender = %s, Parent ID = %d\n", id, name, gender, parent_id);
            fclose(file);
            return id;
        }
    }

    fclose(file);
    return -1;

}

// To get the relationship between two persons
void FindingRelationship(char name1[],char name2[]){

    // char name1[50],name2[50];

    // printf("Enter the names one after another : ");
    // scanf("%s%s",&name1,&name2);
    // printf("\n");

    int members_id[2];

    members_id[0] = LocateId(name1);
    members_id[1] = LocateId(name2);

    if(members_id[0]==members_id[1]){
        printf("can't find relationship between identical nodes,\nEnter distinct id's");
        return;
    }
    struct Member *member0,*member1;
    int level_diff, id_diff = members_id[0] - members_id[1];


    if(id_diff>=0){                        //members_id[0] is smaller level and members_id[1] is larger level
        // id_diff = members_id[0] - members_id[1];
        members_id[0]-=id_diff;
        members_id[1]+=id_diff;
    }
    else{
        id_diff = id_diff * (-1);
    }
    
    member0 = FetchMemberWithId(members_id[0]); //elder member
    member1 = FetchMemberWithId(members_id[1]); //younger member
    level_diff = member1->level - member0->level;

    if(level_diff>2){
        int t_factor = level_diff - 2;
        printf("%s is ",member0->Name);
        while(t_factor!=0){
            printf("great ");
            t_factor--;
        }
        switch(member0->gender){
            case 'M':
            printf("grand father");
            break;
            case 'F':
            printf("grand Mother");
            break;
        }
        printf(" of %s",member1->Name);
    }
    else{
        if(level_diff==2){
            printf("%s is ",member0->Name);
            switch(member0->gender){
                case 'M':
                printf("grand father");
                break;
                case 'F':
                printf("grand Mother");
                break;
            }
            printf(" of %s",member1->Name);
        }
        else{
            if(level_diff == 1){
                if(member1->parent_id == member0->id){ 
                    printf("%s is ",member0->Name);
                    switch(member0->gender){
                        case 'M':
                        printf("father");
                        break;
                        case 'F':
                        printf("Mother");
                        break;
                    }
                    printf(" of %s",member1->Name);
                }
                else{
                    printf("%s is ",member0->Name);
                    switch(member0->gender){
                        case 'M':
                        printf("Uncle");
                        break;
                        case 'F':
                        printf("Aunt");
                        break;
                    }
                    printf(" of %s",member1->Name);
                }
            }
            else{  
                if(member0->parent_id == member1->parent_id){
                    printf("%s is ",member0->Name);
                    switch(member0->gender){
                        case 'M':
                        printf("brother");
                        break;
                        case 'F':
                        printf("sister");
                        break;
                    }
                    printf(" of %s",member1->Name);
                }
                else{
                    printf("%s is ",member0->Name);
                    switch(member0->gender){
                        case 'M':
                        printf("cousin brother");
                        break;
                        case 'F':
                        printf("cousin sister");
                        break;
                    }
                    printf(" of %s",member1->Name);
                }
            }
        }
    }
}

// To fetch data from the metadata
void InsertingMember(){
    if(root==NULL){
        printf("No member present\n\n");
        InsertMember(0);
    }
    else{
        
        FILE *file = fopen("metadata.csv", "r");
        char name[50], gender[10],parent_name[50];
        int id, parent_id;

        if (!file) {
            printf("Error opening file.\n");
            // return 1;
        }

        // Skip the header line
        fscanf(file, "%*[^\n]\n");
        
        printf("-------------------------------------");
        printf("\n\nExisting Members in Family Tree:\n\n");
        while (fscanf(file, "%d,%[^,],%[^,],%d%*[\r\n]", &id, name, gender, &parent_id) == 4) {
            // printf("ID: %d, Name: %s, Gender: %s, Parent ID: %d\n", id, name, gender, parent_id);
            printf("ID:%d->Name:%s\n",id,name);
        }
        printf("-------------------------------------");
        fclose(file);

        printf("\nEnter Parent name:");
        scanf("%s",&parent_name);

        parent_id = LocateId(parent_name);
        // printf("\nparent_id:%d\n",parent_id);
        if(parent_id != -1){
            InsertMember(parent_id);
        }
        
    }
}

void FindBloodRelation(){
    char name1[50],name2[50];

    printf("Enter the persons name one after another:");
    scanf("%s %s",&name1,&name2);

    int member_id[2],diff;

    member_id[0] = LocateId(name1);         //must be larger id  4
    member_id[1] = LocateId(name2);         //must be smaller id 6

    if(member_id[0]==member_id[1]){
        printf("Can't find relation with self");
        return;
    }

    if(member_id[0]>member_id[1]){
        diff = member_id[0]-member_id[1];
        member_id[0] -= diff;   
        member_id[1] += diff;  
    }

    struct Member *src = FetchMemberWithId(member_id[0]); //robb
    struct Member *dest = FetchMemberWithId(member_id[1]);  //arya
    struct Member *src_connections[10];
    struct Member *dest_connections[10];
    int src_count=0,dest_count=0;

    while(src->parent_id!=0){
        printf("%s->",src->Name);
        src_connections[src_count] = src;
        src_count++;
        src = FetchMemberWithId(src->parent_id);
    }
    src_connections[src_count]=src;
    src_count++;
    printf("\nsrc_count:%d\n",src_count);
    // printf("%s->",src->Name);
    // free(src);
    printf("\n");
    while(dest->parent_id !=0){
        printf("%s->",dest->Name);
        dest_connections[dest_count] = dest;
        dest_count++;
        dest = FetchMemberWithId(dest->parent_id);
    }
    dest_connections[dest_count] = dest;
    dest_count++;
    printf("\ndest_count:%d\n",dest_count);

    struct Member *BloodLine[10];
    // BloodLine[0]=src_connections[0];
    // printf("\nbloodline[0]->name:%s\n",BloodLine[0]->Name);
    int bloodline_count=0;
    for(int i=0;i<src_count;i++){
        for(int j=0;j<dest_count;j++){
            if(src_connections[i]==dest_connections[j]){
                for(int k=j;k>=0;k--){
                    BloodLine[bloodline_count] = dest_connections[k];
                    bloodline_count++;
                }
                goto out1;
            }
            else if(j==dest_count-1){
                BloodLine[bloodline_count]=src_connections[i];
                bloodline_count++;
            }
        }
    }

    out1:

    for(int i=0;i<bloodline_count;i++){
        printf("%s->",BloodLine[i]->Name);
    }

    for(int i=0;i<bloodline_count-1;i++){
        printf("\n");
        FindingRelationship(BloodLine[i]->Name,BloodLine[i+1]->Name);
    }

}

void main(){

    
    while(1){
        printf("\n\n--------------------------");
        printf("\n1.Insert Member\n2.Fetch Relationship\n3.Exit\n4.Display Bloodline path\n");
        printf("--------------------------\n");
        int input;
        char name1[50];
        char name2[50];
        printf("Enter choice: ");
        scanf("%d",&input);
        switch(input){
            case 1:
                InsertingMember();
                break;
            case 2:
                // name1="Sanjay";
                // name2 = "Amay";
                printf("\nEnter name one after another: ");
                scanf("%s %s",&name1,&name2);
                FindingRelationship(name1,name2);
                break;
            case 3:
                goto out;
            case 4:
                FindBloodRelation();
                break;
            default:
                printf("invalid input\n");
                break;
        }
    }

    out:

}