answers = []

with open("answers.txt") as file_:
    for line in file_:
        line = line.strip()
        line = line.split("'")
        answers.append(line[1])

with open("answers.txt", 'w') as file_:
    for answer in answers:
        file_.write(answer + "\n")



