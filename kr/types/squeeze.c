// Write an alternate version of squeeze(s1,s2) that deletes each character in the string s1
// that matches any character in the string s2 .

#include <stdio.h>
#define MAXLEN 1024

int getIndexOfString(char target, char str[]);
void squeeze(char s1[], char s2[]);
void copy_string(char const from[], char to[]);
void test();

char s1[MAXLEN] = {0};
char s2[MAXLEN] = {0};

int main(int argc, char const *argv[])
{
  test();
  // if (argc != 3)
  // {
  //  printf("The input should include two arguments\n");
  //  return -1;
  // }  

  // copy_string(argv[1], s1);
  // copy_string(argv[2], s2);

  // squeeze(s1, s2);

  // printf("%s\n", s1);
  return 0;
}

void test()
{
  char *leftstr[] = {
                      "",
                      "a",
                      "antidisestablishmentarianism",
                      "beautifications",
                      "characteristically",
                      "deterministically",
                      "electroencephalography",
                      "familiarisation",
                      "gastrointestinal",
                      "heterogeneousness",
                      "incomprehensibility",
                      "justifications",
                      "knowledgeable",
                      "lexicographically",
                      "microarchitectures",
                      "nondeterministically",
                      "organizationally",
                      "phenomenologically",
                      "quantifications",
                      "representationally",
                      "straightforwardness",
                      "telecommunications",
                      "uncontrollability",
                      "vulnerabilities",
                      "wholeheartedly",
                      "xylophonically", /* if there is such a word :-) */
                      "youthfulness",
                      "zoologically"
                    };
  char *rightstr[] =
                    {
                    "",
                    "a",
                    "the",
                    "quick",
                    "brown",
                    "dog",
                    "jumps",
                    "over",
                    "lazy",
                    "fox",
                    "get",
                    "rid",
                    "of",
                    "windows",
                    "and",
                    "install",
                    "linux"
                    };


  int leftLen = sizeof(leftstr) / sizeof(char*);
  int rightLen = sizeof(rightstr) / sizeof(char*);

  int i = 0;

  while((i < leftLen) && (i < rightLen))
  {
      copy_string(leftstr[i], s1);
      copy_string(rightstr[i], s2);

      squeeze(s1, s2);

      printf("s1 = %s, s2 = %s, squeeze = %s\n", leftstr[i], s2, s1);

      ++i;
  }

}
void copy_string(char const from[], char to[])
{
  int i;
  for (i = 0; from[i] != '\0'; ++i)
  {
    to[i] = from[i];
  }

  to[i] = '\0';
}

void squeeze(char s1[], char s2[])
{
  int  i, j;

  j = 0;
  for (i = 0; s1[i] != '\0'; ++i)
  {
    if (getIndexOfString(s1[i], s2) == -1)
    {
      s1[j++] = s1[i];
    }
  }

  s1[j] = '\0';
}

int getIndexOfString(char target, char str[])
{
  int i;

  for (i = 0; str[i] != '\0'; ++i)
  {
    if (target == str[i])
    {
      return i;
    }
  }

  return -1;
}