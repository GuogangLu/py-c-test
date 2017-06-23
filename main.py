import sys 
import spam
#tst=123;
tst = "remain test";
def test():
    print("pyprint");

r=spam.system("ls -l", "2345", tst, test);
print("r=", r);

spam.lg="lgg";
tmp=spam.remain();
spam.lg="lugg";

print(spam.__dict__);
