# Performance Comparison

Below are the runtime performance comparisons between the C++ and Python implementations of the KNN algorithm using various datasets and input configurations:

| Implementation | Filename    | Delimiter | K   | Input Data                                   | Predicted Class | Runtime        |
|----------------|-------------|-----------|-----|----------------------------------------------|-----------------|----------------|
| C++            | data.csv    | ,         | 4   | 6,190,92,0,0,35.5,0.278,66                   | 0               | 1 Milliseconds        |
| C++            | data.csv    | ,         | 4   | 2,88,58,26,16,28.4,0.766,22                  | 0               | 1 Milliseconds        |
| C++            | data.csv    | ,         | 4   | 9,170,74,31,0,44,0.403,43                    | 1               | 1 Milliseconds        |
| C++            | data.csv    | ,         | 4   | 9,89,62,0,0,22.5,0.142,33                    | 0               | 1 Milliseconds        |
| C++            | data.csv    | ,         | 4   | 10,101,76,48,180,32.9,0.171,63               | 0               | 1 Milliseconds        |
| C++            | data.csv    | ,         | 4   | 2,122,70,27,0,36.8,0.34,27                   | 0               | 1 Milliseconds        |
| C++            | data.csv    | ,         | 4   | 5,121,72,23,112,26.2,0.245,30                | 0               | 1 Milliseconds        |
| Python         | data.csv    | ,         | 4   | 1,126,60,0,0,30.1,0.349,47                   | 1               | 16 Milliseconds       |
| Python         | data.csv    | ,         | 4   | 1,93,70,31,0,30.4,0.315,23                   | 0               | 16 Milliseconds       |

*Note: The runtime values are provided in both milliseconds and seconds for consistency.*
