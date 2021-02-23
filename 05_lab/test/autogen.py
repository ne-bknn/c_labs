import random
from string import printable
import sys

printable = printable[:-6]

def gen_string(l):
    return "".join(random.choices(printable, k=l))


with open(sys.argv[1], "w") as f:
    lines = random.randint(3, 20)
    lines_list = []
    for i in range(lines):
        new_line = " ".join([gen_string(random.randint(4, 100)) for _ in range(random.randint(1, 100))])
        lines_list.append(new_line)

    testcase = "\n".join(lines_list) 
    f.write(testcase)

