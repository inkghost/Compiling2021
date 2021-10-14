FROM g++:10
WORKDIR /app/
COPY main.cpp ./
RUN g++ main.cpp -o main