import sqlite3 as sql
import sys

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

def append_file(row, my_result):
    with open("log.txt", "a") as f:
        f.write(f"{row[0]}, {row[1]} didn't work got [{my_result}] expected [{row[2]}] \n")

valid = 0
skip = 0
for row in rows:
    if "ln" in row[1]:
        skip += 1
        continue
    if "exp" in row[1]:
        skip += 1
        continue
    if "log" in row[1]:
        skip += 1
        continue
    try:
        my_result = main(my_expression=row[1]).eval()
        if abs(my_result - row[2]) < 1e-5:
            valid += 1
        else:
            append_file(row, my_result)
    except:
        append_file(row, None)

print(f"{valid}/{size - skip} are valid expression")
print(f"{size - skip - valid}/{size - skip} are wrong check the log.txt file for more detaills")
print(f"{skip}/{size} are skiped over")
print(f"my calculator is {(valid / (size - skip)) * 100}% reliable for this data set")
