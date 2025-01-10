import sqlite3 as sql
import sys
import math

from recursive_descent import main

connection = sql.connect("./math_expressions.db")
cursor = connection.cursor()

if sys.argv[1]:
    size = int(sys.argv[1])
else:
    size = 1_000_000
cursor.execute(f"""
    SELECT * FROM data
    LIMIT {size};
""")

rows = cursor.fetchall()

connection.close()


with open("log.txt", "w") as f:
    pass

with open("skiped.txt", "w") as f:
    pass

def append_file(path, row, my_result):
    with open(path, "a") as f:
        f.write(f"{row[0]}, {row[1]} didn't work got [{my_result}] expected [{row[2]}] \n")

valid = 0
skip = 0
for row in rows:
    try:
        my_result = main(my_expression=row[1]).eval()
        if math.isclose(my_result,  row[2], rel_tol=1e-5, abs_tol=1e-5):
            valid += 1
        else:
            append_file("log.txt", row, my_result)
    except:
        append_file("log.txt", row, None)

print(f"{valid}/{size - skip} are valid expression")
print(f"{size - skip - valid}/{size - skip} are wrong check the log.txt file for more detaills")
print(f"{skip}/{size} are skiped over")
print(f"my calculator is {(valid / (size - skip)) * 100}% reliable for this data set")
