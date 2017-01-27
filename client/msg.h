#define PASSWORD_MAX 20
#define NAME_MAX  40

typedef struct LOGIN
{
	int userid;
	char password[PASSWORD_MAX];
}LOGIN;

typedef struct REGISTER
{
	char username[NAME_MAX];
	char password[PASSWORD_MAX];
}REGISTER;
