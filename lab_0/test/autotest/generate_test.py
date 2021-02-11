import random

from jinja2 import Environment, PackageLoader
env = Environment(loader=PackageLoader('generate_test', '.'))

template = env.get_template('template.jinja')

def answer(n):
    s = 0
    p = 0
    n = str(n)
    for c in n:
        if int(c) % 2 == 1:
            s += 1
            if p == 0:
                p = int(c)
            else:
                p = p * int(c)

    return s, p

def gen_test():
    n = random.randint(1, 10000)
    ans = answer(n)
    return {"x": n, "a": ans[0], "b": ans[1]}


tests = [gen_test() for _ in range(1000)]
for i, test in enumerate(tests):
    test["i"] = i

with open("autotest.c", "w") as f:
    render = template.render(tests=tests)
    f.write(render)
