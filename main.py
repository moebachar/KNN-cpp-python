import csv
import math
from collections import Counter
import time


def print_table_head(table, n, m):
    for i in range(min(n, 5)):
        for j in range(m):
            print(table[i][j], end=" ")
        print()


def is_numerical(s):
    if not s:
        return False
    return all(c.isdigit() or c == "." for c in s)


class CsvReader:
    def __init__(self, filename, delimiter=","):
        self.filename = filename
        self.delimiter = delimiter
        self.raw_table = []
        self.text_table = []
        self.numerical_table = []
        self.n = 0
        self.m = 0
        self.m_text = 0
        self.m_numerical = 0
        self.unique = set()

        with open(filename, "r") as file:
            reader = csv.reader(file, delimiter=self.delimiter)
            for line in reader:
                self.raw_table.append(line)
                self.n += 1
                self.m = max(self.m, len(line))

        self.read_raw()
        self.split_table()

        for i in range(self.n):
            self.unique.add(self.raw_table[i][-1])

    def read_raw(self):
        with open(self.filename, "r") as file:
            reader = csv.reader(file, delimiter=self.delimiter)
            self.raw_table = [row for row in reader]

    def split_table(self):
        for i in range(self.n):
            text_row = []
            numerical_row = []
            for j in range(len(self.raw_table[i])):
                if is_numerical(self.raw_table[i][j]):
                    numerical_row.append(float(self.raw_table[i][j]))
                else:
                    text_row.append(self.raw_table[i][j])
            self.text_table.append(text_row)
            self.numerical_table.append(numerical_row)

        self.m_text = max(len(row) for row in self.text_table)
        self.m_numerical = max(len(row) for row in self.numerical_table)

    def get_raw_table(self):
        return self.raw_table

    def get_text_table(self):
        return self.text_table

    def get_numerical_table(self):
        return self.numerical_table


class KNN:
    def __init__(self, k, filename, delimiter=","):
        self.k = k
        self.filename = filename
        self.reader = CsvReader(filename, delimiter)
        self.delimiter = delimiter

    def predict(self, line):
        text = []
        numerical = []

        parts = line.strip().split(self.delimiter)
        for part in parts:
            if is_numerical(part):
                numerical.append(float(part))
            else:
                text.append(part)

        n = self.reader.n
        m_text = self.reader.m_text
        m_numerical = self.reader.m_numerical

        if not is_numerical(self.reader.get_raw_table()[0][-1]):
            m_text -= 1
        else:
            m_numerical -= 1

        distances = []
        for i in range(n):
            d = 0
            if m_text > 0:
                d += self.text_distance(text, i, m_text)
            if m_numerical > 0:
                d += self.euclidean_distance(numerical, i, m_numerical)
            distances.append((self.reader.get_raw_table()[i][-1], d))

        distances.sort(key=lambda x: x[1])

        votes = [pair[0] for pair in distances[: self.k]]
        label = Counter(votes).most_common(1)[0][0]

        return label

    def text_distance(self, text, i, m_text):
        occ = sum(
            1 for j in range(m_text) if self.reader.get_text_table()[i][j] == text[j]
        )
        return (m_text - occ) / m_text

    def euclidean_distance(self, numerical, i, m_numerical):
        distance = sum(
            (self.reader.get_numerical_table()[i][j] - numerical[j]) ** 2
            for j in range(m_numerical)
        )
        maximum = max(
            abs(self.reader.get_numerical_table()[i][j]) for j in range(m_numerical)
        )
        return math.sqrt(distance) / maximum


if __name__ == "__main__":
    start = time.time()

    filename = input("Enter filename (example: data.csv) : ")
    delimiter = input("Enter delimiter character (example: , ) : ")

    print("Enter the K: ", end="")
    k = int(input())

    knn_classifier = KNN(k, filename, delimiter)

    line = input("Enter line to predict: ")
    print("Predicted class:", knn_classifier.predict(line))

    end = time.time()

    print(f"Run time : {end - start} seconds")
    """
    To test (data.csv):

    6,190,92,0,0,35.5,0.278,66      1
    2,88,58,26,16,28.4,0.766,22     0
    9,170,74,31,0,44,0.403,43       1
    9,89,62,0,0,22.5,0.142,33       0
    10,101,76,48,180,32.9,0.171,63  0
    2,122,70,27,0,36.8,0.34,27      0
    5,121,72,23,112,26.2,0.245,30   0
    1,126,60,0,0,30.1,0.349,47      1
    1,93,70,31,0,30.4,0.315,23      0
    """
