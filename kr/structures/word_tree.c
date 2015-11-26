#include <stdio.h>
// #include <ctype.h>

#define MAXWORD 64
#define MAXWORDNUM 512
#define isspace(x) ((x) == ' ')
#define isalpha(x) (((x >= 'a') && (x <= 'z')) || ((x >= 'A') && (x <= 'Z')))
#define isalnum(x) (((x >= 'a') && (x <= 'z')) || ((x >= 'A') && (x <= 'Z')) || ((x >= '0') && (x <= '9')))
struct tnode *addtree(struct tnode *, char*);
struct tnode *talloc();
void treeprint(struct tnode*);
int getword(char*, int);
char *strdup(const char* s);
int strcmp(const char*, const char*);
int strlen(const char*);
void strcpy(char* dest, const char* src);

char buf[MAXWORD];
char* bufp = buf;
int getch();
void ugetch(int c);

char allocbuf[MAXWORD * MAXWORDNUM];
char* allocp = allocbuf;
char* alloc(int size);

struct tnode
{
  char* word;
  int count;
  struct tnode* left;
  struct tnode* right;
};

int main(int argc, char const *argv[])
{
  char word[MAXWORD];
  struct tnode *root = NULL;

  while(getword(word, MAXWORD) >= 0)
  {
    if (isalpha(word[0]))
    {
      // printf("%s\n", word);
      // printf("%s\n", word);
      root = addtree(root, word);
    }
  } 

  treeprint(root);

  return 0;
}

void treeprint(struct tnode* root)
{
  if (root)
  {
    treeprint(root->left);
    printf("%4d %s\n", root->count, root->word);
    treeprint(root->right);
  }
}

struct tnode *addtree(struct tnode *root, char* word)
{
  int cmp;
  if (root == NULL)
  {
    root = talloc();
    root->word = strdup(word);
    root->left = NULL;
    root->right = NULL;
    root->count = 1;
  }
  else if (!(cmp = strcmp(root->word, word)))
  {
    ++root->count;
  }
  else if (cmp > 0)
  {
    root->left = addtree(root->left, word);
  }
  else 
  {
    root->right = addtree(root->right, word);
  }

  return root;
}

int strlen(const char *s)
{
  int i = 0;
  while(s[i] != '\0')
  {
    ++i;
  }

  return i;
}

void strcpy(char* dest, const char* src)
{
  while(*dest++ = *src++)
    ;
}

int strcmp(const char* s1, const char* s2)
{
  while(*s1 == *s2)
  {
    if (*s1 == '\0')
    {
      return 0;
    }
    ++s1;
    ++s2;
  }

  return *s1 - *s2;
}

char *strdup(const char* s)
{
  char* ptr = NULL;

  if (s == NULL)
  {
    return NULL;
  }

  ptr = alloc(strlen(s) + 1);

  if (ptr)
  {
    strcpy(ptr, s);
  }

  return ptr;
}

struct tnode *talloc()
{
  return (struct tnode*)alloc(sizeof(struct tnode));
}

char* alloc(int size)
{
  if (allocp - allocbuf + size > MAXWORDNUM * MAXWORD)
  {
    printf("Out of Alloc Memory\n");
    return NULL;
  }

  allocp += size;
  return allocp - size;
}

int getword(char* word, int size)
{
  int i, c;

  while(isspace(c = getch()))
    ;

  if (c == EOF)
  {
    return -1;
  }

  if (!isalpha(c))
  {
    *word = '\0';
    return 0;
  }

  *word++ = c;
  i = 1;
  for (; i < size; ++i)
  {
    c = getch();
    if (!isalnum(c))
    {
      ugetch(c);
      break;
    }
    else
    {
      *word++ = c;
    }
  }

  *word = '\0';
  return i;
}

void ugetch(int c)
{
  if (bufp - buf >= MAXWORD)
  {
    printf("The buf is full now\n");
    return;
  }

  *bufp++ = c;
}

int getch()
{
  char c = (bufp == buf) ? getchar() : *--bufp;
  return c;
}