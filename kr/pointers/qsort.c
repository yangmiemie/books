void swap(int v[], int i, int j)
{
  int temp = v[i];
  v[i] = v[j];
  v[j] = temp;
}

void qsort(int v[], int left, int right)
{
  int i, last;

  if (left >= right)
  {
    return;
  }

  // swap(v, left, (right + left) / 2);
  last = left;
  for (i = left+1; i <= right; ++i)
  {
    if (v[i] < v[left])
    {
      swap(v, ++last, i);
    }
  }

  swap(v, left, last);
  qsort(v, left, last-1);
  qsort(v, last+1, right);
}
 
int main(int argc, char const *argv[])
{
  // int str[] = {11, 3, 2, 6, 23, 98, 45};
  int str[] = {9, 8, 7, 6, 5, 4, 3};
  int i;

  qsort(str, 0, 6);

  for (i = 0; i < 7; ++i)
  {
    printf("%d ", str[i]);
  }
  printf("\n");

  return 0;
}