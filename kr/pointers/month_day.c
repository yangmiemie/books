#include <stdio.h>

char daytab[2][13] = {
  {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
  {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

char* month_name(int n);
int month_day(int year, int yearday, int *pmonth, int *pday);
int day_of_year(int year, int month, int day);

int main(int argc, char const *argv[])
{
  int month, day;
  int yearday = 230;
  int year = 2013;

  month_day(year, yearday, &month, &day);
  printf("%d in %d is %s. %d\n", yearday, year, month_name(month), day);

  printf("%s. %d in %d is No. %d day\n", month_name(month), day, year, day_of_year(year, month, day));
  return 0;
}

char* month_name(int n)
{
  static char* names[] = {
    "Illegal Month",
    "January", "Febuary", "March", "April", "May",
    "July", "June", "Augest", "September", "October",
    "November", "December"
  };

  return (n < 1 || n > 12) ? names[0] : names[n];
}

int month_day(int year, int yearday, int *pmonth, int *pday)
{
  int leap, i;

  if (yearday < 1)
  {
    return -1;
  }

  leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);

  // for (i = 0; yearday > daytab[leap][i]; ++i)
  // {
  //  yearday -= daytab[leap][i];
  // }

  for (i = 0; yearday > *(*(daytab + leap) + i); ++i)
  {
    yearday -= *(*(daytab + leap) + i);
  }

  *pmonth = i;
  *pday = yearday;
}

int day_of_year(int year, int month, int day)
{
  int leap, i;

  if (month > 12 || month < 1 || day < 1)
  {
    return -1;
  }
  leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);

  for (i = 0; i < month; ++i)
  {
    day += *(*(daytab + leap) + i);
  } 

  return day;
}
