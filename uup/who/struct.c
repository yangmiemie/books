struct student {
  int a;
  int b;
  int c;
};

void func(struct student *ptr)
{
  printf("%d, %d, %d\n", ptr->a, ptr->b, ptr->c);
  // printf("%p, %p, %p\n", ptr, &(ptr->b), &(ptr->c));
}

void func2(double* ptr)
{
  printf("%f\n", *ptr);
}

int main(int argc, char const *argv[])
{
  struct student s = {1,2,3};

  printf("%d, %d, %d\n", s.a, s.b, s.c);
  printf("%p, %p, %p\n", &s, &(s.b), &(s.c));
  printf("%d, %d, %d\n", &s, &(s.b), &(s.c));

  func(&s);

  float f = 0.1;
  double d = f;
  func2(&d);
  return 0;
}