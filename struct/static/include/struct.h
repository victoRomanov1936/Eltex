#define MAX_SIZE_STRUCT 100
#define MAX_LENGHT_LAST_NAME 50
#define MAX_LENGHT_FIRST_NAME 50
#define MAX_LENGHT_PHONE 50

struct SubscriberList {
  char first_name[MAX_LENGHT_FIRST_NAME];
  char last_name[MAX_LENGHT_LAST_NAME];
  char phone[MAX_LENGHT_LAST_NAME];
};

int AddSubscriber(struct SubscriberList *array_of_struct);
int SearchSubscriber(struct SubscriberList *array_of_struct);
void DeleteSubscriber(struct SubscriberList *array_of_struct,int index);
void DisplaySubscriber(struct SubscriberList *array_of_struct);
int menu(int* menu_item);
void ShowOneSubscriber(struct SubscriberList *array_of_struct, int index);
void CleanStructur(struct SubscriberList *array_of_struct);

