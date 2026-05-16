#include <stdio.h>

#define MAX 100

struct Student
{
    int id;
    char name[20];
    int age;
    char gender[5];
    float score;
};

struct Student students[MAX];
int student_count = 0;

void addStudent()
{
    printf("输入学号：");
    scanf("%d",&students[student_count].id);  
    // 这里&students是用于获取学生结构体数组中当前学生的地址，以便将输入的学号存储在正确的位置。

    printf("输入姓名：");
    scanf("%s",students[student_count].name);

    printf("输入年龄：");
    scanf("%d",&students[student_count].age);

    printf("输入性别：");
    scanf("%s",students[student_count].gender);

    printf("输入成绩：");
    scanf("%f", &students[student_count].score);

    student_count++;
    printf("学生信息添加成功！\n");
    
}

void showStudents()
{
    int i;
    if (student_count == 0)
    {
        printf("没有学生信息！\n");
        return;
    }

    printf("学生信息列表：\n");
    for(i=0;i<student_count;i++)
    {
        printf(
            "学号：%d，姓名：%s，年龄：%d，性别：%s，成绩：%.2f\n",
            students[i].id,
            students[i].name,
            students[i].age,
            students[i].gender,
            students[i].score
        );
    }
}


void searchStudent()
{
    int id,i,found=0;
    printf("输入要查询的学号：");
    scanf("%d",&id);

    for(i=0;i<student_count;i++)
    {
        if(students[i].id==id)
        {
            printf(
                "学号：%d，姓名：%s，年龄：%d，性别：%s，成绩：%.2f\n",
                students[i].id,
                students[i].name,
                students[i].age,
                students[i].gender,
                students[i].score
            );
            found = 1;

            break;
        }
    }
    if(!found)
    {
        printf("未找到学号为 %d 的学生信息！\n", id);
    }
}

int main()
{
    int choice;

    while(1)
    {
        printf("学生管理系统\n");
        printf("1. 添加学生信息\n");
        printf("2. 显示学生信息\n");
        printf("3. 查询学生信息\n");
        printf("4. 退出\n");
        printf("请输入选择：");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                addStudent();
                break;
            case 2:
                showStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                printf("退出系统！\n");
                return 0;
            default:
                printf("无效的选择，请重新输入！\n");
        }
    }
}